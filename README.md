-----------------------------------------------------------
This software application converts location coordinates 
between three systems:
-----------------------------------------------------------

1. WGS-84 Coordinates
2. Map Pixel Coordinates (Mercator Projection)
3. Map Tile Coordinates

In Mercator Projection, the min/max of
Latitude is approx  +/-  85.05 degrees
and of Longitude is +/- 180.00 degrees

WGS-84 Coordinates consist of latitude and longitude, 
and can be either in degree decimal format, 
or degree-minute decimal format.

Latitude is either N or S
N = North (above equator) 
S = South (below equator)

Longitude is either E or W
E = East (east of prime meridian, Greenwich, England)
W = West (west of prime meridian)

Examples of WGS-84 Coordinates:
1. Degree decimal format:
     DDD.ddddd
Lat:  22.06463 N
Lon: 057.73658 E

2. Degree Minute decimal format:
      DDMM.mmm
Lat:  2203.878 N
Lon: 05744.195 E
-----------------------------------------------------------
For information regarding mercator map and tile
coordinate systems, visit Bing Maps Tile System:
https://msdn.microsoft.com/en-us/library/bb259689.aspx
-----------------------------------------------------------
How to Use Coordinate Converter Tool:
-----------------------------------------------------------
Enter known coordinates in any one system.

Click the appropriate button at tte bottom to convert
them into the other two coordinate systems.

For the map tile coordinates, the Zoom level (Z) has to
be entered as well.