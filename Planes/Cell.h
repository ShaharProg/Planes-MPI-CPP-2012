#pragma once
#include "MyPoint.h"
#include <math.h>

class Cell
{
private:
	MyPoint Topleft;
	MyPoint ButtomRight;
public:
	Cell(MyPoint &Topleft,MyPoint &ButtomRight);
	MyPoint getTopleft()const{return Topleft;}
	MyPoint getButtomRight()const{return ButtomRight;}
	MyPoint getTopRight()const;
	MyPoint getButtomLeft()const;
	MyPoint getCenter()const;
	bool inCell(MyPoint &p)const;
	bool LineInCell(MyPoint &center1,MyPoint &center2)const;
	friend ostream& operator<<(ostream& out, const Cell &c);
	~Cell(void);
};
