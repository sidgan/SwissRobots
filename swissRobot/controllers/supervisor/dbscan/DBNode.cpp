/*
 * Node.cpp
 *
 *  Created on: Oct 7, 2011
 *      Author: xis
 */

#include "DBNode.hpp"

namespace cluster
{

	Node::Node(double x, double y, int idInit) : std::pair<double, double>(y,x)
	{
		m_fVisited = false;
		m_nCluster = -1;
		id = idInit;
	}


	Node::~Node() {
		// TODO Auto-generated destructor stub
	}

	double Node::getX() const{
		return second;
	}

	double Node::getY() const{
		return first;
	}

	int Node::getId() const{
		return id;
	}

}
