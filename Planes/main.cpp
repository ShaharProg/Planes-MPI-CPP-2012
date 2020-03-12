#include <mpi.h>
#include <map>
#include <vector>
#include <iostream>
#include<fstream>
#include <algorithm>
#include <omp.h>
/*****************************************************************************

						Shahar Zigman 303065858

							Project: Planes

*****************************************************************************/


#define FILE "C:\\data.txt"
#define MIN_PROC 2
#define ROOT 0
#define TERMINATE -1

using namespace std;
#include "Sky.h"
#include "Grid.h"

int **CreateMatrix(int row,int col);
int *CreateArray(int row);
void printIntArr(int *arr,int size);

int main(int argc,char *argv[])
{
    int  namelen, numprocs, myid;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
	int *X;
	int *Y;
	int *ID;
	Sky *world=NULL;

    MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);	
	MPI_Get_processor_name(processor_name,&namelen);
	MPI_Status status;

	if (numprocs<MIN_PROC)
	{
		if(myid==ROOT)
			cout << "This program can't work with less than " << MIN_PROC <<" process";
		
	}
	else
	{

		if (myid == ROOT)	//-------------------------------------MASTER-START-------------------------------------
		{
			
			int TermCounter=0; //count terminate commands;
			
			// create world from file   
			ifstream myReadFile;
			myReadFile.open(FILE);
			if (!myReadFile.is_open())
			{
				cout << "Couldn't open file, please check that " << FILE << " exists and try again!";
				exit(0);
			}
			world =new Sky(myReadFile);
			myReadFile.close();

			
			int Matrix_Data[4]={world->getA(),world->getB(),world->getM(),world->getN()};
			MPI_Bcast(Matrix_Data,4,MPI_INT,ROOT,MPI_COMM_WORLD); //broadcasting matrix information from ROOT
			
			//  find earlier & late flight hours  
			vector<ControlPoint>::const_iterator earlyFlight = world->getPlanes().begin()->getCritacalPoints().begin(); //first plane depart time
			vector<ControlPoint>::const_iterator lateFlight = world->getPlanes().begin()->getCritacalPoints().end(); //first plane boarding time
			vector<AirPlane>::const_iterator it;
			lateFlight--;

			#pragma omp parallel for private(it)
			for(int j=1;j<world->getPlanes().size();++j)
			{
				it = world->getPlanes().begin();
				advance(it,j);

				//check early flight (departing hours only)
				if((it->getCritacalPoints().begin())->getTime().toSec() < (earlyFlight->getTime().toSec()))
					earlyFlight=it->getCritacalPoints().begin();

				//check late flight (boarding hours only)
				if((--it->getCritacalPoints().end())->getTime().toSec() > (lateFlight->getTime().toSec()))
				{
					lateFlight=it->getCritacalPoints().end();
					lateFlight--;
				}

			}

			MyTime Begin=earlyFlight->getTime();
			MyTime End=lateFlight->getTime();


			//allocate max Planes information matrix (flight id & x,y cords according to dt)
			int Totalplanes=world->getPlanes().size(); //total number of planes
			X=CreateArray(Totalplanes);
			Y=CreateArray(Totalplanes);
			ID=CreateArray(Totalplanes);


			// Init Slaves 
			int count=0;
			for(int i=1;i<numprocs;++i,count=0)
			{
				if(Begin.toSec() <= End.toSec())
				{
					//snapshot of the sky at current time
					MyPoint *Pexist;
					#pragma omp parallel for private(it,Pexist)
					for(int j=0;j<world->getPlanes().size();++j)
					{
						it = world->getPlanes().begin();
						advance(it,j);

						Pexist=it->getPointByTime(Begin);
						if(Pexist)
						{
							#pragma omp critical
							{
								ID[count]=it->getId();
								X[count]=Pexist->getX();
								Y[count]=Pexist->getY();
								count++;
							}
						}
						delete Pexist;
					}

					MPI_Send(&count,1,MPI_INT,i,1,MPI_COMM_WORLD); //send array size
					MPI_Send(ID,count,MPI_INT,i,2,MPI_COMM_WORLD); //send ID array 
					MPI_Send(X,count,MPI_INT,i,3,MPI_COMM_WORLD); //send X array 
					MPI_Send(Y,count,MPI_INT,i,4,MPI_COMM_WORLD); //send Y array
					Begin+=world->getDT();
				}
				else
				{
					//Terminate
					count=TERMINATE;
					MPI_Send(&count,1,MPI_INT,i,1,MPI_COMM_WORLD); 
					TermCounter++;
				}
			}
			//End of Init Slaves

		
			//LOOP STARTS
			for(;TermCounter!=numprocs-1;)
			{
				int Loop_size;
				int Data_size;
				int *data_from_slave;
				MPI_Recv(&Loop_size,1,MPI_INT,MPI_ANY_SOURCE,5,MPI_COMM_WORLD,&status); // recieve loop size from any source
		
				for(int i=0;i<Loop_size;++i)
				{
					MPI_Recv(&Data_size,1,MPI_INT,status.MPI_SOURCE,6,MPI_COMM_WORLD,&status); //rec size of data
					data_from_slave=CreateArray(Data_size);
					MPI_Recv(data_from_slave,Data_size,MPI_INT,status.MPI_SOURCE,7,MPI_COMM_WORLD,&status); //rec data from slaves (id + hidden planes id)

					AirPlane* currentPlane=world->getPlaneByID(data_from_slave[0]);

					if(currentPlane)
						currentPlane->addToHiddenfrom(data_from_slave+1,Data_size-1);

					delete data_from_slave;
				}
				
				//New Tasks for slaves
				if(Begin.toSec()<=End.toSec())
				{
					int count=0;		
					//snapshot of the sky at current time
					MyPoint *Pexist;
					#pragma omp parallel for private(it,Pexist)
					for(int j=0;j<world->getPlanes().size();++j)
					{
						it = world->getPlanes().begin();
						advance(it,j);

						Pexist=it->getPointByTime(Begin);
						if(Pexist)
						{
							#pragma omp critical
							{
								ID[count]=it->getId();
								X[count]=Pexist->getX();
								Y[count]=Pexist->getY();
								count++;
							}
						}
						delete Pexist;
					}

					MPI_Send(&count,1,MPI_INT,status.MPI_SOURCE,1,MPI_COMM_WORLD); //send array size
					MPI_Send(ID,count,MPI_INT,status.MPI_SOURCE,2,MPI_COMM_WORLD); //send ID array 
					MPI_Send(X,count,MPI_INT,status.MPI_SOURCE,3,MPI_COMM_WORLD); //send X array 
					MPI_Send(Y,count,MPI_INT,status.MPI_SOURCE,4,MPI_COMM_WORLD); //send Y array 
					
					Begin+=world->getDT();
				}
				else //Terminate
				{
					int Terminate=TERMINATE;
					MPI_Send(&Terminate,1,MPI_INT,status.MPI_SOURCE,1,MPI_COMM_WORLD);
					TermCounter++;
				}


			}//LOOP ENDS
			delete ID;
			delete X;
			delete Y;

			cout << world->getHighestCD();
			
		}		//-------------------------------------MASTER-END-------------------------------------
		else	//-------------------------------------SLAVE-START-------------------------------------
		{
			
			int currentPlanes;
			int Matrix_Data[4];
			map<int, vector<int>> ID_Situation;
			vector<int> ID_List; //for ID_Situation
			
			MPI_Bcast(Matrix_Data,4,MPI_INT,ROOT,MPI_COMM_WORLD); //getting airspace information as matrix
			Grid grid(Matrix_Data[0],Matrix_Data[1],Matrix_Data[2],Matrix_Data[3]);
			
			
			//LOOP
			for(;;)
			{
				MPI_Recv(&currentPlanes,1,MPI_INT,ROOT,1,MPI_COMM_WORLD,&status); //getting current plane count

				if (currentPlanes==TERMINATE)
					break;

				//getting snapeshot of the sky
				X=CreateArray(currentPlanes);
				Y=CreateArray(currentPlanes);
				ID=CreateArray(currentPlanes);

				MPI_Recv(ID,currentPlanes,MPI_INT,ROOT,2,MPI_COMM_WORLD,&status); //getting planes IDs
				MPI_Recv(X,currentPlanes,MPI_INT,ROOT,3,MPI_COMM_WORLD,&status); //getting planes x cords
				MPI_Recv(Y,currentPlanes,MPI_INT,ROOT,4,MPI_COMM_WORLD,&status); //getting planes y cords

				int i;
				
				#pragma omp parallel for //num_threads(1)
				for(i=0;i<currentPlanes;i++)  //creating current snapeshot of the sky
				{
					grid.addPlaneToSnapeshot(ID[i],MyPoint(X[i],Y[i]));
				}
				

				//SNAPESHOT OF THE SKY
				multimap<int, Cell*>::const_iterator Anchor = grid.getSnapshot().begin();
				multimap<int, Cell*>::const_iterator RefPoint;
				multimap<int, Cell*>::const_iterator TarPoint;

				for(;Anchor!=grid.getSnapshot().end();++Anchor)
				{
					ID_List.clear(); //new iteration

					for(RefPoint=grid.getSnapshot().begin();RefPoint!=grid.getSnapshot().end();++RefPoint)
					{
						if(RefPoint==Anchor) //skip
							continue;
						
						#pragma omp parallel for private(TarPoint) //ANCHOR + RefPoint
						for(int j=0;j<grid.getSnapshot().size();++j)
						{
							TarPoint=grid.getSnapshot().begin();
							advance(TarPoint,j);
			

							if(TarPoint==Anchor || TarPoint==RefPoint)
								continue;
							
							
							if(TarPoint->second->LineInCell(Anchor->second->getCenter(),RefPoint->second->getCenter())) 
							{

								//Refernce must be between refernce and target
								int max_x=max(TarPoint->second->getCenter().getX(),Anchor->second->getCenter().getX());
								int min_x=min(TarPoint->second->getCenter().getX(),Anchor->second->getCenter().getX());

								int max_y=max(TarPoint->second->getCenter().getY(),Anchor->second->getCenter().getY());
								int min_y=min(TarPoint->second->getCenter().getY(),Anchor->second->getCenter().getY());

								int checkx=RefPoint->second->getCenter().getX();
								int checky=RefPoint->second->getCenter().getY();

								if((max_x >= checkx && checkx >= min_x) && (max_y >= checky && checky >= min_y))
								{
		
									if(find(ID_List.begin(),ID_List.end(),TarPoint->first)==ID_List.end()) //if we didn't check this plane already
									{
										#pragma omp critical
										ID_List.push_back(TarPoint->first); //insert target id
									}

								}
							}
						}
					}


					ID_Situation.insert(pair<int,vector<int>>(Anchor->first,vector<int>(ID_List))); //assign ID_List using copy cons
				}



				int loopIter=ID_Situation.size();
				MPI_Send(&loopIter,1,MPI_INT,ROOT,5,MPI_COMM_WORLD); //send loop size
				map<int, vector<int>>::const_iterator Situation;

				for(Situation=ID_Situation.begin();Situation!=ID_Situation.end();++Situation)
				{
					int Data_size=Situation->second.size() + 1;
			
				
					MPI_Send(&Data_size,1,MPI_INT,ROOT,6,MPI_COMM_WORLD); //send array size

					int *data_array=CreateArray(Data_size);

					//create data array (first planeid and the following are the ids of the hidden planes)
					data_array[0]=Situation->first;
					for(int i=1;i<Data_size;++i)
						data_array[i]=Situation->second[i-1];

					MPI_Send(data_array,Data_size,MPI_INT,ROOT,7,MPI_COMM_WORLD); //send array 

					delete data_array;

				}

				delete ID;
				delete X;
				delete Y;
				grid.clearSnapeshot();
				ID_Situation.clear();

			}
			//END LOOP
		}		//-------------------------------------SLAVE-START-------------------------------------
		
	}
	delete world;
	MPI_Finalize();
	return 0;

}

int **CreateMatrix(int row,int col)
{
	int **Matrix= new int*[row];
		for(int i=0; i<col; i++)
			Matrix[i] = new int[col];
		return Matrix;
}

int *CreateArray(int row)
{
	int *Matrix= new int[row];
	return Matrix;
}

void printIntArr(int *arr,int size)
{
	for(int i=0;i<size;++i)
		cout << arr[i] << " ";
}
