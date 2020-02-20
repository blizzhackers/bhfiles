#ifndef _COMMONMATH_H
#define _COMMONMATH_H

#define PI 3.1415926535897932384626433832795

BYTE CalcPercent(DWORD dwVal, DWORD dwMaxVal, BYTE iMin = 0);
long CalculateDistance(const POINT& pt1, const POINT& pt2);
long CalculateAngle(const POINT& pt1, const POINT& pt2);
long CalculateDistance(long x1, long y1, long x2, long y2);
void NormalizeAngle(int& rAngle);
void NormalizeRect(RECT& rRect);
void NormalizeRect(LPRECT lpRect);
long CalculateAngle(long x1, long y1, long x2, long y2);
POINT CalculatePointOnTrack(const POINT& ptOrigin, int nRadius, int nAngle);
POINT CalculateRandomPosition(const POINT& ptOrigin, int nRadiusMin, int nRadiusMax, int nAngleMin = 0 , int nAngleMax = 360);
BOOL PtInCircle(const POINT& pt, const POINT& ptOrigin, int nRadius);

#endif