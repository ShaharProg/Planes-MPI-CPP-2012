#pragma once
#include <vector>
#include <algorithm>
#include "ControlPoint.h"
#include "MyPoint.h"
#include "MyTime.h"

class AirPlane
{
	int id;
	vector<ControlPoint> cPoint; 
	int CriticalDegree; 
	vector<int> Hiddenfrom;

public:
	AirPlane(ifstream& in);
	~AirPlane(void);
	
	const vector<ControlPoint>&	getCritacalPoints()const{return this->cPoint;}
	int getCD(){return CriticalDegree;}
	int getCD()const{return CriticalDegree;}
	int	getId()const{return this->id;}
	const vector<int>& GetHiddenfrom()const{return Hiddenfrom;}
	void addToHiddenfrom(int *IDS,int size);
	void PrintHiddenfrom()const;
	void pushControlPoint(ControlPoint &cp){cPoint.push_back(cp);}
	MyPoint* getPointByTime(MyTime &dt)const;
	friend ostream& operator<<(ostream& out, const AirPlane& p);
	friend istream& operator>> (istream& in, AirPlane& p);
};

