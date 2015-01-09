// File:          supervisor.cpp
// Date:          10/10/2011
// Description:   Stop the simulation after simtime seconds, 
//                extract the position of the blocks, compute the 
//                clusters and the noise-boxes, and stores these 
//                results in a file.
// Authors:       Cristiano Alessandro (alessandro@ifi.uzh.ch)
//                Naveen Kuppuswamy (naveenoid@ifi.uzh.ch)

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sys/time.h>

#include <webots/Robot.hpp>

#include "./blockprocessing/blockprocessing.hpp"
#include "./blockprocessing/blockPos.hpp"

#include "./dbscan/DBNode.hpp"
#include "./dbscan/dbscan.hpp"

static const int TIMESTEP = 32; // Time Step of the simulation, Do not change this 
static const int SIMTIME = 120; // Simulation time in Seconds, change according to your needs (must be an integer)

using namespace webots;
using namespace std;

class supervisor : public Supervisor {
  
  private:
    Node* robotto;
    
    void stopRobot(string name)
    {
      robotto = getFromDef(name.c_str());         
      Field* ctr = robotto->getField("maxSpeed");
      ctr->setSFFloat(0);
    }
  
  public:
    
    supervisor(): Supervisor(){}
    virtual ~supervisor() {}
    
    void run(){
    
      std::cout << "supervisor starts" << std::endl;
      
      //Leave the loop after SIMTIME seconds
/*      timeval start,final; 
      gettimeofday(&start,NULL);
      do {
        gettimeofday(&final,NULL);
        //std::cout<<final.tv_sec-start.tv_sec<<std::endl;
        if( (final.tv_sec-start.tv_sec)>=SIMTIME )
          break;
      } while (step(TIMESTEP) != -1);*/
      
      double start, final;
      start = getTime();
      do {
        final = getTime();
        //std::cout<<final-start<<std::endl;
        if( (final-start)>=SIMTIME )
          break;
      } while (step(TIMESTEP) != -1);
      
      /*
      // Get the robot node and set its speed to zero
      robotto = getFromDef("ROBOT1"); 
      Field* ctr = robotto->getField("maxSpeed");
      ctr->setSFFloat(0);
    */
      stopRobot("ROBOT1");
      stopRobot("ROBOT2");
      stopRobot("ROBOT3");
      
      std::cout<< "supervisor terminated" << std::endl;
      
    }
};


//Counts the number of noise-boxes and add it to the number of clusters
int numClustersAndNoise(int nClr, vector<cluster::Node*> nds)
{
  int count = 0;
  for(int i=0;i<25;i++){
    if (nds[i]->m_nCluster == -1)
      count ++;
  }
  return count + nClr;
}


//Sets box colour
void setBoxColour(supervisor* supCtr, string boxName, const double col[3])
{
  Node* box = supCtr->getFromDef(boxName);
  Field* chld = box->getField("children");
  box = chld->getMFNode(0);
  chld = box->getField("appearance");
  box = chld->getSFNode();
  chld = box->getField("material");
  box = chld->getSFNode();
  //cout<<box->getTypeName()<<endl;

  chld = box->getField("diffuseColor");  
  chld->setSFColor(col);
}


//Sets box colour based on the cluster number
void colourBox(supervisor* supCtr, int boxId, int nClst)
{
  stringstream ss;
  ss << boxId;
  string boxName = "BOX" + ss.str();
  
  double col[3];
  
  switch (nClst)
  {
    case 0:
      col[0] = 0.6; col[1] = 0.8; col[2] = 0.2;
      break;
    case 1:
      col[0] = 0.2; col[1] = 0.2; col[2] = 0.2;
      break;
    case 2:
      col[0] = 1; col[1] = 0; col[2] = 0;
      break;
    case 3:
      col[0] = 0; col[1] = 1; col[2] = 0;
      break;
    case 4:
      col[0] = 0.6; col[1] = 0.1; col[2] = 0.9;
      break;
    case 5:
      col[0] = 0; col[1] = 0; col[2] = 1;
      break;
    case 6:
      col[0] = 1; col[1] = 1; col[2] = 0;
      break;
    case 7:
      col[0] = 1; col[1] = 0.6; col[2] = 0;
      break;
    case 8:
      col[0] = 0; col[1] = 1; col[2] = 1;
      break;
    case 9:
      col[0] = 0.6; col[1] = 0.1; col[2] = 0.1;
      break;
    case 10:
      col[0] = 0.5; col[1] = 0.4; col[2] = 0;
      break;
    case 11:
      col[0] = 1; col[1] = 0.7; col[2] = 0.8;
      break;
    case 12:
      col[0] = 0.9; col[1] = 0.4; col[2] = 0.9;
      break;
    case -1:
      col[0] = 0.8; col[1] = 0.8; col[2] = 0.8;
      break;
  }
  setBoxColour(supCtr, boxName, col);
}


int main(int argc, char **argv)
{

  vector<BlockPos*> pts;            //Position of the blocks
  vector<cluster::Node*> vecNodes;  //Vector of nodes storing the positions
	cluster::DBScan* dbscan;          //Clustering algorithm object
  
  //Data structure used by the DBScan algorithm
  kdtree* kdTree;            
  kdTree = kd_create(2);
  
  //Run the supervisor to stop the robot controller after SIMTIME seconds
  supervisor* supCtr = new supervisor();
  supCtr->run();
  
  //Extract block positions from the world
  BlockProcessing *bp = new BlockProcessing(supCtr);
  pts = bp->extractBlocksPositions();
  
  ofstream resFile;   //Handler to the file where results will be stored
  int nClusters = 0;  //Variable storing the number of clusters found
  BlockPos * tmp;     
  double xy[2];
  
  //Insert each block position in the kdTree data structure
  for(int i=0;i<25;i++){
    tmp = pts[i];
		cluster::Node* pt = new cluster::Node(tmp->getX(),tmp->getY(), tmp->getId());
		vecNodes.push_back(pt);   
    xy[0] = tmp->getX();
    xy[1] = tmp->getY();
		kd_insert(kdTree, xy, pt);
	}
  
  //Retrive size of the objects
  Node* box = supCtr->getFromDef("BOX1");
  Field* chld = box->getField("children");
  box = chld->getMFNode(0);
  chld = box->getField("geometry");
  box = chld->getSFNode();
  chld = box->getField("size");
  const double* szVect = chld->getSFVec3f();
  //Compute the appropriate value for eps
  double eps = szVect[0]*2 + szVect[0]*2/5.5;

  //Retrive the clustering algorithm object
	dbscan = new cluster::DBScan(kdTree, vecNodes,eps,2);
  //Run the DBScan algorithm and get the number of clusters found
	nClusters = dbscan->runDBScan();

  /*
  Print the id, the associated cluster and the position of each box.
  If the associated cluster is -1, the box does not belong to any cluster 
  and it is considered as noise.
  These data are also stored in the file result.txt in the following format
  <box-id,cluster-number,x-position,y-position>
  (a line for each box, for a total of 25 lines).
  The last two rows of the file contain the number of clusters and 
  the number of clusters+noise-boxes respectively.
  */
  int id, nCluster;
  resFile.open ("../../data/results.txt");
  for(int i=0;i<25;i++)
  {
    
    id = vecNodes[i]->getId();
    nCluster = vecNodes[i]->m_nCluster;
    
    //Coulouring
    colourBox(supCtr, id, nCluster);
    
    //Printing
    cout<< id << "\t";
    cout<< nCluster << "\t";
    cout<< "x: " << vecNodes[i]->getX() << "\t";
    cout<< "y: " << vecNodes[i]->getY() << endl;
    
    //Storing in the file
    resFile << vecNodes[i]->getId() << ",";
    resFile << vecNodes[i]->m_nCluster << ",";
    resFile << vecNodes[i]->getX() << ",";
    resFile << vecNodes[i]->getY() << endl;
  }
  
  
  //Print how many clusters DBScan has found (i.e. different than -1)
	cout << "Number of clusters: " << nClusters << endl;
  //Print number of clusters + noise-boxes
  cout << "Number of clusters and noise-boxes: " << numClustersAndNoise(nClusters, vecNodes) << endl;

  //Store how many clusters DBScan has found (i.e. different than -1)
	resFile << nClusters << endl;
  //Store number of clusters + noise-boxes
  resFile << numClustersAndNoise(nClusters, vecNodes) << endl;

  resFile.close();
  delete supCtr;
  return 0;
}
