#include "MyTime.h"

MyTime::MyTime(int H,int M,int S)
{
	this->H=H;
	this->M=M;
	this->S=S;
}

MyTime::MyTime()
{
	this->H=0;
	this->M=0;
	this->S=0;
}


int MyTime::operator-(const MyTime& t)const  //returns seconds 
{
	return abs(this->toSec() - t.toSec());
}


int MyTime::toSec()const
{
	return this->H*3600 + this->M*60 + this->S;
}

MyTime::~MyTime(void)
{
}

bool MyTime::operator>=(const MyTime& t) const 
{
	int sum=0;
	sum+=(this->H - t.getH())*3600;
	sum+=(this->M - t.getM())*60;
	sum+=this->S - t.getS();
	if(sum>=0) return true;
	return false;
}

MyTime MyTime::operator+=(int time)
{
	H+=time/3600;

	M+=(time%3600)/60;

	S+=((time%3600)%60);
	if(S>59)
	{
		M++;
		S=0;
	}

	if(M>59)
	{
		H++;
		M=(M%60);
	}

	return MyTime(H,M,S);
}

ostream& operator<<(ostream& out, const MyTime &t)
{		
	out << t.getH() << ":" << t.getM() << ":" << t.getS();
	return out;
}

bool MyTime::operator==(const MyTime& t) const 
{
	if(this->H == t.getH() && this->M==t.getM() && this->S==t.getS())
		return true;
	return false;
}

bool MyTime::operator!=(const MyTime& t) const 
{
	return !(*this==t);
}

MyTime& MyTime::operator=(const MyTime& t)  
{

	this->H=t.getH();
	this->M=t.getM();
	this->S=t.getS();

	return *this;
}

/*
MyTime::MyTime(const MyTime& t)
{
	*this=t;
}
*/