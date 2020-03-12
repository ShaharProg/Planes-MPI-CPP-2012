#ifndef __MYPOINT_H
#define __MYPOINT_H
#include <iostream>
#include <fstream>
using namespace std;
class MyPoint
{
private:
	int x;
	int y;
public:
	MyPoint(int x,int y);
	MyPoint();
	int getX()const{return x;}
	int getY()const{return y;}
	void setPoint(int x,int y){this->x=x; this->y=y;}
	MyPoint operator-(const MyPoint& p) const ;
	bool operator==(const MyPoint& p) const ;
	friend ostream& operator<<(ostream& out, const MyPoint& p);
	float DistanceFromPoint(MyPoint &p);
	~MyPoint(void);
};
#endif