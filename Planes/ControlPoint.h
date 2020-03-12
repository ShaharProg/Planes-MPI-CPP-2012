#pragma once

#include "ControlPoint.h"
#include "MyPoint.h"
#include "MyTime.h"

class ControlPoint
{

	//int X,Y,H,M,S;
	MyTime CPTime;
	MyPoint CPoint;

public:
	


	const MyTime &getTime()const{return this->CPTime;}
	MyTime &getTime(){return this->CPTime;}

	const MyPoint &getPoint() const{return this->CPoint;}
	 MyPoint &getPoint() {return this->CPoint;}

	ControlPoint(ifstream& in);
	~ControlPoint(void);

	friend ostream& operator<<(ostream& out, const ControlPoint& cp);
	friend istream& operator>> (istream& in, ControlPoint& cp);
	
};

