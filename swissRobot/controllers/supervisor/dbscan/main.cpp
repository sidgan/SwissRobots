#include <iostream>
#include <stdlib.h>
#include <vector>
#include "Node.hpp"
#include "dbscan.hpp"

using namespace std;
using namespace cluster;

int main()
{
	vector<Node*> vecNodes;
	kdtree* kdTree;
	DBScan* dbscan;
	double pts[2][25];

	kdTree = kd_create(2);

	for(int i=0;i<2;i++)
	{
		for(int j=0;j<25;j++)
		{
			pts[i][j] = rand() % 10;
		}
	}


	for(int i=0;i<25;i++){
		//vecNodes.push_back( new Node(pts[0][i],pts[1][i]) );

		Node* pt = new Node(pts[0][i],pts[1][i]);
		vecNodes.push_back(pt);
		double xy[2] = {pts[0][i] , pts[1][i]};
		kd_insert(kdTree, xy, pt);

		cout << pts[0][i] << "   " << pts[1][i] << endl;
	}

	dbscan = new DBScan(kdTree, vecNodes,2,2);
	int nClusters = dbscan->runDBScan();

	cout << nClusters << endl;


}
