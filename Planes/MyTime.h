#ifndef __MYTIME_H
#define __MYTIME_H

#include<math.h>
#include <iostream>
#include <fstream>
using namespace std;

class MyTime
{
private:
	int H,M,S;
public:
	MyTime();
	MyTime(int H,int M,int S);
	//MyTime(const MyTime& t);
	int getH()const{return H;}
	int getM()const{return M;}
	int getS()const{return S;}
	int toSec()const; //returns seconds 
	void SetTime(int H,int M,int S){this->H=H;this->M=M;this->S=S;}
	int operator-(const MyTime& t) const ; //returns seconds 
	bool operator>=(const MyTime& t) const ;
	bool operator==(const MyTime& t) const ;
	bool operator!=(const MyTime& t) const ;
	MyTime& operator=(const MyTime& t)  ;
	MyTime operator+=(int MyTime);
	friend ostream& operator<<(ostream& out, const MyTime &t);
	~MyTime(void);
};

#endif

