#include "MyPoint.h"
#include<math.h>

MyPoint::MyPoint(int x,int y)
{
	this->x=x;
	this->y=y;
}

MyPoint::MyPoint()
{
	this->x=0;
	this->y=0;
}


MyPoint MyPoint::operator-(const MyPoint& p) const 
{
	return MyPoint(abs(this->x - p.getX()),abs(this->y - p.getY()));
}

bool MyPoint::operator==(const MyPoint& p) const 
{
	if(this->x==p.getX() && this->y==p.getY())
		return true;
	return false;
}

//print out the Point info
ostream& operator<<(ostream& out, const MyPoint& p)
{	
	out << "(" << p.getX() << "," << p.getY() << ")";
	return out;
}

float MyPoint::DistanceFromPoint(MyPoint &p)
{
	return sqrt( pow(p.getX() - this->x,2.0) + pow(p.getY() - this->y,2.0) );
}


MyPoint::~MyPoint(void)
{
}
