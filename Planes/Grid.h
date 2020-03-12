#pragma once
#include <vector>
#include <iostream>
#include <map>
#include "MyPoint.h"
#include "Cell.h"
using namespace std;

class Grid
{
private:
	vector<Cell*> Cells;
	multimap<int, Cell*> Snapshot;
public:
	Grid(int A,int B, int m,int n);
	void clearSnapeshot(){Snapshot.clear();}
	void addPlaneToSnapeshot(int id,MyPoint &cord);
	vector<Cell*> getCells()const{return Cells;}
	vector<Cell*> &getCells(){return Cells;}
	multimap<int, Cell*> getSnapshot()const{return Snapshot;}
	multimap<int, Cell*>& getSnapshot(){return Snapshot;}
	void printCell()const;
	void printSnapshot()const;
	~Grid(void);
};
