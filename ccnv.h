#ifndef CCNV_H
#define CCNV_H

#include <QMainWindow>

#include <QString>
#include <QPointF>

#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "mapConv.h"

namespace Ui {
class ccnv;
}

class ccnv : public QMainWindow
{
   Q_OBJECT

public:
   explicit ccnv(QWidget *parent = 0);
   ~ccnv();

private:
   Ui::ccnv *ui;

   bool flgGpsInput;

   QRegularExpression reLatDM;
   QRegularExpression reLonDM;
   QRegularExpression reLatDg;
   QRegularExpression reLonDg;
   QRegularExpressionValidator vreLatDM;
   QRegularExpressionValidator vreLonDM;
   QRegularExpressionValidator vreLatDg;
   QRegularExpressionValidator vreLonDg;

   void SetGpsValidators(void);

   void FillPxToLoc(QPointF pxCoor, quint8 z);
   QPointF GetLocFromUi(void);

private slots:
   void on_btnGpsFormat_clicked();

   void on_btnFromGps_clicked();
   void on_btnFromPx_clicked();
   void on_btnFromTile_clicked();

   void on_spnZoom_editingFinished();
};

#endif // CCNV_H

/*===== EOF ========================================================*/
