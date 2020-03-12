#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "Sky.h"
#include "AirPlane.h"


Sky::Sky(ifstream& in) 
{
	in >> *this;
}

//set control poit info from file *relies on a specific text patern
istream& operator>> (istream& in, Sky& sky){
	int temp;
	AirPlane* tempPlane;

	in >> temp;
	sky.setA(temp);
	in >> temp;
	sky.setB(temp);
	in >> temp;
	sky.setM(temp);
	in >> temp;
	sky.setN(temp);
	in >> temp;
	sky.setDT(temp);

	bool fEof = false;

	
	while (!fEof)
	{
		tempPlane = new AirPlane((ifstream&)in);	//promote file
		if(in.eof())
		{ //check if eof
			fEof = true;
			continue;
		}
		sky.pushPlane(*tempPlane);
	}

	//cout << "\n\n counter:" << sky.getPlanes().size() <<endl<<endl;
	
	return in;
}

ostream& operator<<(ostream& out, const Sky& sky){		
	out << "A:" << sky.getA() << " B:" << sky.getB() << " m:" << sky.getM() << " n:" << sky.getN() << " td:" << sky.getDT() << endl;

	vector<AirPlane>::const_iterator it;
	for(it = sky.getPlanes().begin(); it != sky.getPlanes().end(); ++it){
		out << *it << endl;
	}
	return out;
}

Sky::~Sky(void)
{
}


AirPlane* Sky::getPlaneByID(int id)
{
	vector<AirPlane>::iterator it=Planes.begin();
	for(;it!=Planes.end();++it)
	{
		if(it->getId()==id)
			return &(*it);
	}
	return NULL;
}

AirPlane& Sky::getHighestCD()
{
	vector<AirPlane>::iterator it=Planes.begin()+1;
	vector<AirPlane>::iterator max=Planes.begin();
	for(;it!=Planes.end();++it)
	{
		if(it->getCD() > max->getCD())
			max=it;
	}
	return *max;
}
