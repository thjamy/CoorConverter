#ifndef MAPCONV_H
#define MAPCONV_H

/*------------------------------------------------------------------*/
/* Last Updated [2017-11-16]                                        */
/*------------------------------------------------------------------*/
/* conv class contains formulae for conversions between lat/lon     */
/* coordinates (WGS84), pixel coordinates of map and tile indices   */
/*------------------------------------------------------------------*/
/* References for formulae:                                         */
/*------------------------------------------------------------------*/
/* https://en.wikipedia.org/wiki/Haversine_formula                  */
/* . DistFrom()                                                     */
/*                                                                  */
/* http://www.movable-type.co.uk/scripts/latlong.html               */
/* . BrngFrom()                                                     */
/*                                                                  */
/* Bing Maps Tile System:                                           */
/* https://msdn.microsoft.com/en-us/library/bb259689.aspx           */
/* . MapSize(), MapScale(), MapGroundRes(), Wgs84_PixelXY()         */
/* . PixelXY_Wgs84(), PixelXY_TileXY(), TileXY_PixelXY()            */
/*------------------------------------------------------------------*/
/* Deg_DDMM():  Converts coordinates from DD.ddddd to DDDMM.mmm     */
/*------------------------------------------------------------------*/
/* DDMM_Deg():  Converts coordinates from DDDMM.mmm to DD.ddddd     */
/*------------------------------------------------------------------*/
/* Distance():  Calculates distance (metres) between                */
/*              location1 and location2 (DD.ddddd)                  */
/*------------------------------------------------------------------*/
/* Bearing():   Calculates initial bearing (degrees) of             */
/*              destination from source (DD.ddddd)                  */
/*------------------------------------------------------------------*/
/* MapSize():   Calculates map size (pixels) at a given zoom        */
/*              level     (map size = map width = map height)       */
/*------------------------------------------------------------------*/
/* MapGndRes(): Calculates ground resolution (metres/pixel)         */
/*              using given latitude (DD.ddddd) and zoom level      */
/*------------------------------------------------------------------*/
/* MapScale():  Calculates denominator N of the map scale ratio 1:N */
/*              (1 metre on map represents N metres on ground)      */
/*              using given latitude (DD.ddddd) and zoom level      */
/*------------------------------------------------------------------*/
/* Coor_PxXY(): Maps given coordinates location (DD.ddddd) onto     */
/*              the map (pixels) at given zoom level                */
/*              Origin (0,0) of the map is at top left              */
/*------------------------------------------------------------------*/
/* PxXY_Coor(): Maps given location (pixels) back to WGS84          */
/*              coordinates (DD.ddddd) at given zoom level          */
/*------------------------------------------------------------------*/
/* PxXY_TileXY(): Calculates tile coordinates that contains the     */
/*                given pixel location                              */
/*------------------------------------------------------------------*/
/* TileXY_PxXY(): Calculates top-left pixel location of given tile  */
/*                referenced from map origin                        */
/*------------------------------------------------------------------*/
/* TileXY_TileStr(): Returns file name of the tile image using      */
/*                   the given tile coordinates and zoom level      */
/*------------------------------------------------------------------*/
/* TileStr_TileXY(): Returns tile coordinates from the given        */
/*                   file name                                      */
/*------------------------------------------------------------------*/

#include <QtMath>
#include <QPointF>

#include <QString>
#include <QStringList>

#define Deg_Rad(d) ((d) * M_PI /  180)
#define Rad_Deg(r) ((r) *  180 / M_PI)

#define SCRN_DPI   96
#define TSIZE     256      // Tile Width = Height = 256px

#define ZOOM_MIN   11
#define ZOOM_MAX   14
#define ZOOM_DFLT  13

// Spherical Mercator projection
#define MIN_LAT   -85.05112878
#define MAX_LAT    85.05112878
#define MIN_LON  -180
#define MAX_LON   180
#define EARTH_RAD  6378137     // in metres, at equator

class conv
{
public:
   static QPointF Deg_DDMM(QPointF loc_Deg);
   static QPointF DDMM_Deg(QPointF loc_DDMM);

   static qreal   Distance(QPointF loc2, QPointF loc1);
   static qreal   Bearing(QPointF dst, QPointF src);

   static quint32 MapSize(quint8 zm);
   static qreal   MapGndRes(qreal lat, quint8 zm);
   static qreal   MapScale(qreal lat, quint8 zm);

   static QPointF Coor_PxXY(QPointF loc, quint8 zm);
   static QPointF PxXY_Coor(QPointF  px, quint8 zm);

   static QPoint  PxXY_TileXY(QPointF px);
   static QPointF TileXY_PxXY(QPoint  tXY);

   static QString TileXY_TileStr(QPoint tXY, quint8 zm);
   static QPoint  TileStr_TileXY(QString tStr);
};

#endif // MAPCONV_H

/*===== EOF ========================================================*/