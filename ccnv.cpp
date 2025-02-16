#include "ccnv.h"
#include "ui_ccnv.h"
// comment
ccnv::ccnv(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::ccnv)
{
   ui->setupUi(this);
   setFixedSize(size());

   flgGpsInput = 0;
   SetGpsValidators();
}

ccnv::~ccnv()
{
   delete ui;
}

void ccnv::on_spnZoom_editingFinished()
{
   int zoom  = ui->spnZoom->value();
   int pxMax = conv::MapSize(zoom);

   ui->spnPxX->setMaximum(pxMax - 1);
   ui->spnPxY->setMaximum(pxMax - 1);

   ui->spnTileX->setMaximum((1 << zoom) - 1);
   ui->spnTileY->setMaximum((1 << zoom) - 1);
}

void ccnv::SetGpsValidators(void)
{
   // DDMM.mmm
   reLatDM.setPattern("^-?(?:[0-7]\\d|8[0-4])[0-5]\\d\\.\\d{3}$");
   vreLatDM.setRegularExpression(reLatDM);
   ui->txtLatDDMM->setValidator(&vreLatDM);

   reLonDM.setPattern("^-?(?:0?\\d{2}|1[0-7]\\d)[0-5]\\d\\.\\d{3}$");
   vreLonDM.setRegularExpression(reLonDM);
   ui->txtLonDDMM->setValidator(&vreLonDM);

   // DD.ddddd
   reLatDg.setPattern("^-?(?:[0-7]\\d|8[0-4])\\.\\d{5}$");
   vreLatDg.setRegularExpression(reLatDg);
   ui->txtLatDeg->setValidator(&vreLatDg);

   reLonDg.setPattern("^-?(?:0?\\d{2}|1[0-7]\\d)\\.\\d{5}$");
   vreLonDg.setRegularExpression(reLonDg);
   ui->txtLonDeg->setValidator(&vreLonDg);
}

void ccnv::on_btnGpsFormat_clicked()
{
   flgGpsInput = !flgGpsInput;

   if (0 == flgGpsInput)
      {
      ui->txtLatDeg->setEnabled(false);
      ui->txtLonDeg->setEnabled(false);

      ui->txtLatDDMM->setEnabled(true);
      ui->txtLonDDMM->setEnabled(true);
      }
   else
      {
      ui->txtLatDeg->setEnabled(true);
      ui->txtLonDeg->setEnabled(true);

      ui->txtLatDDMM->setEnabled(false);
      ui->txtLonDDMM->setEnabled(false);
      }
}

QPointF ccnv::GetLocFromUi(void)
{
qreal   lat, lon;
QPointF locDDMM, locDeg;
QString strLoc;

   if (0 == flgGpsInput)
      {
      lat = ui->txtLatDDMM->text().toFloat();
      lon = ui->txtLonDDMM->text().toFloat();

      locDDMM = QPointF(lon, lat);
      locDeg  = conv::DDMM_Deg(locDDMM);

      strLoc.sprintf("%09.5f", locDeg.x());
      ui->txtLonDeg->setText(strLoc);

      strLoc.sprintf("%08.5f", locDeg.y());
      ui->txtLatDeg->setText(strLoc);
      }
   else
      {
      lat = ui->txtLatDeg->text().toFloat();
      lon = ui->txtLonDeg->text().toFloat();

      locDeg  = QPointF(lon, lat);
      locDDMM = conv::Deg_DDMM(locDeg);

      strLoc.sprintf("%09.3f", locDDMM.x());
      ui->txtLonDDMM->setText(strLoc);

      strLoc.sprintf("%08.3f", locDDMM.y());
      ui->txtLatDDMM->setText(strLoc);
      }

   return locDeg;
}

void ccnv::on_btnFromGps_clicked()
{
   if (0 == flgGpsInput)
      {
      if (("" == ui->txtLatDDMM->text())|
          ("" == ui->txtLonDDMM->text()))    return;
      }
   else
      {
      if (("" == ui->txtLatDeg->text())|
          ("" == ui->txtLonDeg->text()))     return;
      }

   QPointF locDeg = GetLocFromUi();

   quint8 zoom = ui->spnZoom->value();

   //-----------------------------------------------------------------
   // GPS to Pixel
   QPointF pxCoor = conv::Coor_PxXY(locDeg, zoom);

   ui->spnPxX->setValue((int)pxCoor.x());
   ui->spnPxY->setValue((int)pxCoor.y());

   //-----------------------------------------------------------------
   // Pixel to tile + tile name
   QString strTile;

   QPoint tileCoor = conv::PxXY_TileXY(pxCoor);

   ui->spnTileX->setValue(tileCoor.x());
   ui->spnTileY->setValue(tileCoor.y());

   strTile.sprintf("%1d-%1d-%1d", zoom, tileCoor.x(), tileCoor.y());
   ui->txtTileStr->setText(strTile);

   //-----------------------------------------------------------------
   // Offset from Tile's top left x,y
   QPointF tileCoorTL = conv::TileXY_PxXY(tileCoor);

   ui->spnTilePxX->setValue((int)pxCoor.x() - (int)tileCoorTL.x());
   ui->spnTilePxY->setValue((int)pxCoor.y() - (int)tileCoorTL.y());
}

void ccnv::on_btnFromPx_clicked()
{
   qreal pxX = (qreal)ui->spnPxX->value();
   qreal pxY = (qreal)ui->spnPxY->value();

   QPointF pxCoor = QPointF(pxX, pxY);

   quint8  zoom = ui->spnZoom->value();

   // Tile XY
   QPoint tileCoor = conv::PxXY_TileXY(pxCoor);

   ui->spnTileX->setValue(tileCoor.x());
   ui->spnTileY->setValue(tileCoor.y());

   //Tile Name
   QString strTile;
   strTile.sprintf("%1d-%1d-%1d", zoom, tileCoor.x(), tileCoor.y());
   ui->txtTileStr->setText(strTile);

   //-----------------------------------------------------------------
   // Offset from Tile's top left x,y
   QPointF tileCoorTL = conv::TileXY_PxXY(tileCoor);

   ui->spnTilePxX->setValue((int)pxCoor.x() - (int)tileCoorTL.x());
   ui->spnTilePxY->setValue((int)pxCoor.y() - (int)tileCoorTL.y());

   //-----------------------------------------------------------------
   FillPxToLoc(pxCoor, zoom);
}

void ccnv::on_btnFromTile_clicked()
{
   int tileX = ui->spnTileX->value();
   int tileY = ui->spnTileY->value();

   QPoint tileCoor = QPoint(tileX, tileY);

   quint8 zoom = ui->spnZoom->value();

   //Tile Name
   QString strTile;
   strTile.sprintf("%1d-%1d-%1d", zoom, tileCoor.x(), tileCoor.y());
   ui->txtTileStr->setText(strTile);

   // Tile to Px
   QPointF pxCoor = conv::TileXY_PxXY(tileCoor);

   ui->spnPxX->setValue((int)pxCoor.x());
   ui->spnPxY->setValue((int)pxCoor.y());

   //Tile to PxTopLeft
   ui->spnTilePxX->setValue(0);
   ui->spnTilePxY->setValue(0);

   //-----------------------------------------------------------------
   FillPxToLoc(pxCoor, zoom);
}

void ccnv::FillPxToLoc(QPointF pxCoor, quint8 z)
{
QString strLoc;

   // Px to Lat/lon DD.ddddd
   QPointF locDeg = conv::PxXY_Coor(pxCoor, z);

   strLoc.sprintf("%09.5f", locDeg.x());
   ui->txtLonDeg->setText(strLoc);

   strLoc.sprintf("%08.5f", locDeg.y());
   ui->txtLatDeg->setText(strLoc);

   //  Lat/lon DD.ddddd to DDMM.mmm
   QPointF locDDMM = conv::Deg_DDMM(locDeg);

   strLoc.sprintf("%09.3f", locDDMM.x());
   ui->txtLonDDMM->setText(strLoc);

   strLoc.sprintf("%08.3f", locDDMM.y());
   ui->txtLatDDMM->setText(strLoc);
}

/*===== EOF ========================================================*/