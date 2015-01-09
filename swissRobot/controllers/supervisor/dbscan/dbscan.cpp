#include "dbscan.hpp"
#include "kdtree.h"

using namespace cluster;
using namespace std;

DBScan::DBScan(kdtree* kdTreeInit, vector<Node*> vecNodesInit, double epsInit, int nPtsInit)
{
	kdTree = kdTreeInit;
	vecNodes = vecNodesInit;
	dblEpsilon = epsInit;
	nMinPts = nPtsInit;
}

DBScan::~DBScan(){}

bool DBScan::getNodesInRadius(Node* pt, double dblRadius, int nMinPts, vector<Node*>& rgpNodesFound)
{
	double pos[2] = {pt->getX(), pt->getY()};
	kdres* res = kd_nearest_range(kdTree, pos, dblRadius);
	int number_near = kd_res_size(res);
	if (number_near >= nMinPts)
	{
			while (!kd_res_end(res))
			{
					Node* ptNear = (Node*)kd_res_item(res, pos);
					rgpNodesFound.push_back(ptNear);
					kd_res_next(res);
			}
	}
	kd_res_free(res);
	return number_near >= nMinPts;
}


void DBScan::expandCluster(vector<Node*>& rgp, int nCluster, double dblEpsilon, int nMinPts)
{
    vector<Node*> rgpNeighbourhood;
    for (int i = 0; i < (int)rgp.size(); i++)
	if (!rgp[i]->m_fVisited)
	{
		rgp[i]->m_nCluster = nCluster;
		rgp[i]->m_fVisited = true;
		rgpNeighbourhood.push_back(rgp[i]);
	}

    for (int i = 0; i < (int)rgpNeighbourhood.size(); i++)
    {
		Node* pNodeNear = rgpNeighbourhood[i];
		vector<Node*> rgpNeighbourhood2;
		if (getNodesInRadius(pNodeNear, dblEpsilon, nMinPts, rgpNeighbourhood2))
		{
			// append to rgpNeighbourhood items in rgpNeighbourhood2 that aren't already in rgpNeighbourhood
			for (int j = 0; j < (int)rgpNeighbourhood2.size(); j++)
			{
				Node* pNode = rgpNeighbourhood2[j];
				if (!pNode->m_fVisited)
						pNode->m_fVisited = true;
				if (pNode->m_nCluster < 0)
				{
						pNode->m_nCluster = nCluster;
						rgpNeighbourhood.push_back(pNode);
				}
			}
		}
    }
}


int DBScan::runDBScan()
{
	int nCluster = 0;
	for (vector<Node*>::const_iterator it = vecNodes.begin(); it != vecNodes.end(); it++)
	{
		Node* pNode = *it;
		if (!pNode->m_fVisited)
		{
		  pNode->m_fVisited = true;

		  vector<Node*> rgpNeighbourhood;
		  if (getNodesInRadius(pNode, dblEpsilon, nMinPts, rgpNeighbourhood))
		  {
			  pNode->m_nCluster = nCluster;
			  pNode->m_fVisited = true;
			  expandCluster(rgpNeighbourhood, nCluster, dblEpsilon, nMinPts);
			  nCluster++;
		  }
		}
     }
     return nCluster;
}

vector<Node*> DBScan::getNodes()
{
  return vecNodes;
}
