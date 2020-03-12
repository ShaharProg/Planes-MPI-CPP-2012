#pragma once

#include <vector>
#include "AirPlane.h"

class Sky
{
	 int A,B,m,n,dt;
	 vector<AirPlane> Planes; 

public:
	Sky(ifstream& in);

	void setA(const int num){this->A = num ;}
	void setB(const int num){this->B = num ;}
	void setM(const int num){this->m = num ;}
	void setN(const int num){this->n = num ;}
	void setDT(const int num){this->dt = num ;}
	
	void pushPlane(AirPlane &p){Planes.push_back(p);}

	int	getA()const{return this->A;}
	int	getB()const{return this->B;}
	int	getM()const{return this->m;}
	int	getN()const{return this->n;}
	int	getDT()const{return this->dt;}
	AirPlane& getHighestCD();
	AirPlane* getPlaneByID(int id);

	const vector<AirPlane>& getPlanes()const{return this->Planes;}

	~Sky(void);

	friend ostream& operator<<(ostream& out, const Sky& sky);
	friend istream& operator>> (istream& in, Sky& sky);
};

