#include "DBNode.hpp"
#include <vector>
#include "kdtree.h"

using namespace std;

namespace cluster
{
	class DBScan
	{
		public:
			DBScan(kdtree*, vector<Node*>, double, int);
			~DBScan();
			int runDBScan();
			vector<Node*> getNodes();

		private:
			kdtree* kdTree;
			vector<Node*> vecNodes;
			double dblEpsilon;
			int nMinPts;

			bool getNodesInRadius(Node* pt, double dblRadius, int nMinPts, vector<Node*>& rgpNodesFound);
			void expandCluster(vector<Node*>& rgp, int nCluster, double dblEpsilon, int nMinPts);



	};
}
