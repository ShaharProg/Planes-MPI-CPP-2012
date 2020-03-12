#include <iostream>
#include <fstream>

using namespace std;

#include "AirPlane.h"
#include "ControlPoint.h"

AirPlane::AirPlane(ifstream& in) 
{
	CriticalDegree=0;
	in >> *this;
}

//print out the Plane info
ostream& operator<<(ostream& out, const AirPlane& p){	
	out << "Plane ID:" << p.getId() << endl;
	out << "Critical Degree:" << p.getCD() << endl << endl;
	out << "Hidden: " << endl;


	vector<int>::const_iterator it2;
	for(it2 = p.GetHiddenfrom().begin(); it2 != p.GetHiddenfrom().end(); ++it2)
		out << *it2 << " ";

	cout << endl;
	out << "\nControl Points: " << endl;

	vector<ControlPoint>::const_iterator it;
	for(it = p.getCritacalPoints().begin(); it != p.getCritacalPoints().end(); ++it)
		out << *it << endl;


	return out;
}

//set Plane info from file *relies on a specific text patern
istream& operator>> (istream& in, AirPlane& p){
	if(typeid(in) == typeid(ifstream)){
		int numOfPoint;
		
		in >> p.id;
		in >> numOfPoint;
		for(int i=0; i<numOfPoint ; i++){
			p.pushControlPoint(ControlPoint((ifstream&)in));
		}
	}
	return in;
}

AirPlane::~AirPlane(void)
{

}


MyPoint* AirPlane::getPointByTime(MyTime &dt)const
{
	vector<ControlPoint>::const_iterator  it = cPoint.begin(); 
	vector<ControlPoint>::const_iterator  it2 = cPoint.begin();
	++it2;

	while(it2 != cPoint.end())
	 {
		 if( ( (dt >= it->getTime() ) && it2->getTime() >= dt))		 //check if the DT is between the interval
		 {
			 //cout << *it << " " << *it2 << endl;
			int t= dt - it->getTime(); //t from begining
			int d_t=(it2->getTime() - it->getTime()); //dt

			int x0 = it->getPoint().getX(); //x0
			int ds_x=(it2->getPoint().getX() - it->getPoint().getX()); //dx
			int x = x0 + ((double)ds_x*t/d_t); 

			int y0 = it->getPoint().getY();
			int ds_y=(it2->getPoint().getY() - it->getPoint().getY());
			int y = y0 + ((double)ds_y/d_t)*t; 

			return new MyPoint(x, y);
		 }
		++it;
		++it2;
	}
	return NULL;
}

void AirPlane::addToHiddenfrom(int *IDS,int size)
{
	CriticalDegree+=size;
	for(int i=0;i<size;i++) //skip first - id of the plane
	{
		if(find(Hiddenfrom.begin(),Hiddenfrom.end(),IDS[i])==Hiddenfrom.end())
			Hiddenfrom.push_back(IDS[i]);
	}
}

void AirPlane::PrintHiddenfrom()const
{
	vector<int>::const_iterator it = Hiddenfrom.begin();

	for(;it!=Hiddenfrom.end();++it)
		cout << *it << " ";
}