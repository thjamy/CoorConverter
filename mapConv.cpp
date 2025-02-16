#include "mapConv.h"

QPointF conv::Deg_DDMM(QPointF loc_Deg)
{
   qreal x      = loc_Deg.x();
   int   x_DD   = (int)x;
   qreal x_DDMM = (x_DD * 100) + ((x - x_DD) * 60);

   qreal y      = loc_Deg.y();
   int   y_DD   = (int)y;
   qreal y_DDMM = (y_DD * 100) + ((y - y_DD) * 60);

   return QPointF(x_DDMM, y_DDMM);
}

QPointF conv::DDMM_Deg(QPointF loc_DDMM)
{
   qreal x      = loc_DDMM.x();
   int   x_i    = (int)x / 100;
   qreal x_f    = (x - (x_i * 100)) / 60;
   qreal x_deg  = (qreal)x_i + x_f;

   qreal y      = loc_DDMM.y();
   int   y_i    = (int)y / 100;
   qreal y_f    = (y - (y_i * 100)) / 60;
   qreal y_deg  = (qreal)y_i + y_f;

   return QPointF(x_deg, y_deg);
}

qreal conv::Distance(QPointF loc2, QPointF loc1)
{
   qreal lat2 = Deg_Rad(loc2.y());
   qreal lon2 = Deg_Rad(loc2.x());

   qreal lat1 = Deg_Rad(loc1.y());
   qreal lon1 = Deg_Rad(loc1.x());

   qreal dLat = lat2 - lat1;
   qreal dLon = lon2 - lon1;

   qreal cAng = ((1 - cos(dLat)) / 2) +
                 (cos(lat2) * cos(lat1) * ((1 - cos(dLon)) / 2));

   qreal dist = 2 * EARTH_RAD * asin(sqrt(cAng));

   return dist;
}

qreal conv::Bearing(QPointF dst, QPointF src)
{
   qreal lat2 = Deg_Rad(dst.y());
   qreal lon2 = Deg_Rad(dst.x());

   qreal lat1 = Deg_Rad(src.y());
   qreal lon1 = Deg_Rad(src.x());

   qreal dLon = lon2 - lon1;

   qreal dY = sin(dLon) * cos(lat2);

   qreal dX = cos(lat1) * sin(lat2) -
              sin(lat1) * cos(lat2) * cos(dLon);

   qreal bearing = Rad_Deg(atan2(dY, dX));

   if (bearing <   0) bearing += 360;  // these 2 lines added
   if (bearing > 360) bearing -= 360;

   return bearing;
}

quint32 conv::MapSize(quint8 zm)
{
   return (quint32)TSIZE << zm;   // TILE_SIZE * (2 ^ zoom)
}

qreal conv::MapGndRes(qreal lat, quint8 zm)
{
   return cos(Deg_Rad(lat)) * (2 * M_PI * EARTH_RAD) / MapSize(zm);
}

qreal conv::MapScale(qreal lat, quint8 zm)
{
   return MapGndRes(lat, zm) * SCRN_DPI / 0.0254;
}

QPointF conv::Coor_PxXY(QPointF loc, quint8 zm)
{
   int size = MapSize(zm);

   qreal x = ((loc.x() + 180) / 360) * size;

   qreal sinLat = sin(Deg_Rad(loc.y()));
   qreal flt_y  = 0.5 - log((1 + sinLat)/(1 - sinLat)) / (4 * M_PI);
   qreal y      = flt_y * size;

   qreal px_X = x + 0.5;
   qreal px_Y = y + 0.5;

   return QPointF(px_X, px_Y);
}

QPointF conv::PxXY_Coor(QPointF px, quint8 zm)
{
   int size = MapSize(zm);

   qreal lon = (px.x() * 360 / size) - 180;

//   qreal expTerm = exp(4 * M_PI * (0.5 - (float)px.y() / size));
//   qreal lat     = Rad_Deg(asin((expTerm - 1) / (expTerm + 1)));

   qreal y   = (px.y() / size) - 0.5;
   qreal lat = 90 - 360 * atan(exp(2 * M_PI * y)) / M_PI;

   return QPointF(lon, lat);
}

QPoint conv::PxXY_TileXY(QPointF px)
{
   return QPoint(int(px.x() / 256), int(px.y() / 256));
}

QPointF conv::TileXY_PxXY(QPoint tXY)
{
   return QPointF(tXY.x() * 256, tXY.y() * 256);
}

QString conv::TileXY_TileStr(QPoint tXY, quint8 zm)
{
   QString tStr;
   tStr.sprintf("/%1d-%1d-%1d", zm, tXY.x(), tXY.y());

   return tStr;
}

QPoint conv::TileStr_TileXY(QString tStr)
{
   QStringList list = tStr.split('-');

//   int z = list.at(0).toInt();
   int x = list.at(1).toInt();
   int y = list.at(2).toInt();

   return QPoint(x,y);
}

/*===== EOF ========================================================*/