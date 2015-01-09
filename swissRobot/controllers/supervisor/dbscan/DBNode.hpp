/*
 * Node.h
 *
 *  Created on: Oct 7, 2011
 *      Author: xis
 */

#include <utility>

#ifndef NODE_HPP_
#define NODE_HPP_

namespace cluster
{

	class Node : public std::pair<double, double>
	{
		public:
			Node(double, double, int);
			~Node();


			double getX() const;
			double getY() const;
			int getId() const;

			bool m_fVisited;
			int m_nCluster;
			int id;
	};

}

#endif /* NODE_H_ */
