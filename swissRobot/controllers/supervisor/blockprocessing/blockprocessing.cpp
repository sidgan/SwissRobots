#include "blockprocessing.hpp"
#include <string>
#include <sstream>
#include <iostream>

#include "blockPos.hpp"

using namespace std;
using namespace webots;

BlockProcessing::BlockProcessing(Supervisor* sup)
{
  supVisor = sup;
}

BlockProcessing::~BlockProcessing(){}

vector<BlockPos*> BlockProcessing::extractBlocksPositions()
{
  int i;
  //double * position;
  string str;
  stringstream ss;
  Node * tmp;
  //double pts[2][25];
  
  vector<BlockPos*> pts;
  
  for(i=0;i<25;i++)
  {
    stringstream ss; //add number to the stream
    ss << "BOX" << i+1;
    str = ss.str();
    
    //std::cout << str << std::endl;
    
    if(supVisor->getFromDef(str)==NULL)
      std::cout << "Object is NULL" << std::endl;
    else{
      blockVector.push_back( supVisor->getFromDef(str) );
      tmp = blockVector[i];
    
      const double *position = tmp->getPosition();
      //std::cout<< str << " " << *position << " " << *(position+2) << std::endl;
      
      pts.push_back(new BlockPos( *position,*(position+2),i+1 ));
    
    }
    
  }
  
  return pts;
}

void BlockProcessing::processClusters(){}