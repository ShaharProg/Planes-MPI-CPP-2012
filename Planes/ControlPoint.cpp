#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "ControlPoint.h"


ControlPoint::ControlPoint(ifstream& in)
{
	in >> *this;
}

//print out control points info
ostream& operator<<(ostream& out,const ControlPoint& cp){
	out << cp.getPoint() << " " << cp.getTime();
	return out;
}

//set control poit info from file *relies on a specific text patern
istream& operator>> (istream& in, ControlPoint& cp){
	if(typeid(in) == typeid(ifstream)){
		int temp1,temp2;
		string tempTime;

		//(X,Y)
		in >> temp1;		//get the x value from the data file
		in >> temp2;		//get the y value from the data file
		cp.getPoint().setPoint(temp1,temp2);	//set the point 

		//HH:MM:SS
		in >> tempTime;	//get the HH:MM:SS values from the data file
		cp.getTime().SetTime(
			atoi(tempTime.substr(0,2).c_str()),
			atoi(tempTime.substr(3,2).c_str()),	
			atoi(tempTime.substr(5,2).c_str())
			);	//set the second of the point
	}
	return in;
}


ControlPoint::~ControlPoint(void)
{
}
