
#include "Grid.h"


Grid::Grid(int A,int B, int m,int n)
{
	//A width
	//B height
	//m rows
	//n cols
	//cout << "Grid constrector Enters\n";

	int cellwidth=A/n;
	int cellheight=B/m;
	
	for(int i=0; i<n;i++)
		for(int j=0;j<m;j++)
			Cells.push_back(new Cell(MyPoint((j)*cellwidth,(i)*cellheight),MyPoint((j+1)*cellwidth,(i+1)*cellheight)));

	//cout << "Grid constrector Exits\n";
}

void Grid::addPlaneToSnapeshot(int id,MyPoint &cord)
{
	vector<Cell*>::const_iterator it=Cells.begin();

	for(;it!=Cells.end();++it) //finds current cell
	{
		if((*it)->inCell(cord))
		{
			#pragma omp critical
			Snapshot.insert(pair<int, Cell*>(id,*it));
			return;
		}
	}
}

void Grid::printCell()const
{
	vector<Cell*>::const_iterator CellPrint = Cells.begin();
	int count=0;
	for(;CellPrint!=Cells.end();++CellPrint,++count)
	{
			cout << "Cell: " << count << endl;
			cout << "Top:" << (*CellPrint)->getTopleft() << " " << (*CellPrint)->getTopRight() << endl;
			cout << "Buttom:" << (*CellPrint)->getButtomLeft() << " " << (*CellPrint)->getButtomRight() << endl;
	}
	cout << "Count cells: " << count << endl;
}

void Grid::printSnapshot()const
{
	//cout << "Printsnapeshot enters" << endl;

	multimap<int, Cell*>::const_iterator SnapshotPrint = Snapshot.begin();

	for(;SnapshotPrint!=Snapshot.end();++SnapshotPrint)
	{
		cout << "Plane ID: " << SnapshotPrint->first << endl;
		cout << "Current Cell: " << *(SnapshotPrint->second) << endl << endl;
	}

	//cout << "Printsnapeshot exits" << endl;

}


Grid::~Grid(void)
{
}
