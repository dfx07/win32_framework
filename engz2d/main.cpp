
#include "xgeo2dlib.h"

int main()
{
	Vec2D p1 = { 1,  1};

	Vec2D p2 = { 2, 2.1f };


	Vec2D p3 = p1/2;


	Rect2D rect = { 10 , 10, 10 , 10 };


	Point2D p6 = rect.BottomRight();


	float a =  geo::v2::cross(p1, p2);

	return 1;
}