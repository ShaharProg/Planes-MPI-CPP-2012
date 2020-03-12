
#include "Cell.h"


Cell::Cell(MyPoint &Topleft,MyPoint &ButtomRight) : Topleft(Topleft),ButtomRight(ButtomRight)
{

}

MyPoint Cell::getTopRight()const
{
	int x=ButtomRight.getX();
	int y=Topleft.getY();
	return MyPoint(x,y);
}
MyPoint Cell::getButtomLeft()const
{
	int y=ButtomRight.getY();
	int x=Topleft.getX();
	return MyPoint(x,y);
}

MyPoint Cell::getCenter()const
{
	int x=abs((double)Topleft.getX() + ButtomRight.getX())/2;
	int y=abs((double)Topleft.getY() + ButtomRight.getY())/2;
	return MyPoint(x,y); 
}

bool Cell::inCell(MyPoint &p)const
{
	if (Topleft.getX() <= p.getX() && p.getX() < ButtomRight.getX() && Topleft.getY() <= p.getY() && p.getY() < ButtomRight.getY())
		return true;
	return false;
}

//print out the cell info
ostream& operator<<(ostream& out, const Cell &c)
{	
	out << "[" << c.getTopleft() << " " << c.getButtomRight() << "]";
	return out;
}

bool Cell::LineInCell(MyPoint &center1,MyPoint &center2)const
{
	//cout << "in::LineInCell";
	if (center1==center2) //same cell
		return false;

	float m;
	if (center2.getX()-center1.getX()==0)
		m=0;
	else
		m=(center2.getY()-center1.getY())/(center2.getX()-center1.getX());

	float y1=m*(this->Topleft.getX() - center1.getX()) + center1.getY(); // left edge of square y=m(x-x1)+y1
	if (Topleft.getY() <= y1 && y1 <= getButtomLeft().getY() )
		return true;

	float y2=m*(this->getTopRight().getX() - center1.getX()) + center1.getY(); //right edge of square y=m(x-x1) + y1
	if (Topleft.getY() <= y2 && y2 <= getButtomLeft().getY() )
		return true;

	float x1=(this->Topleft.getY() - center1.getY() + m*center1.getX()) / m; // top edge of square x=(y-y1+mx1) / m
	if (Topleft.getY() <= x1 && x1 <= ButtomRight.getY() )
		return true;

	float x2=(this->ButtomRight.getY() - center1.getY() + m*center1.getX()) / m; // buttom edge of square x=(y-y1+mx1) / m
	if (Topleft.getY() <= x2 && x2 <= ButtomRight.getY() )
		return true;

	return false;
}