#include <vector>
#include <webots/Supervisor.hpp>
#include <webots/Node.hpp>
#include "blockPos.hpp"

using namespace std;
using namespace webots;

class BlockProcessing{

  private:
    
    vector<Node*> blockVector;
    Supervisor* supVisor;    
    
    
  public:
    BlockProcessing(Supervisor*);
    ~BlockProcessing();
    
    vector<BlockPos*> extractBlocksPositions();
    void processClusters();

};