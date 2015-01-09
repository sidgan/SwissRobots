#include <utility>

using namespace std;

#ifndef   BLOCKPOS_HPP_
#define BLOCKPOS_HPP_

class BlockPos
{
  private:
    pair<double,double> pos;
    int id;
    
    public:
      BlockPos(double,double,int);
      ~BlockPos();
      
      pair<double,double> operator()();
      double getX();
      double getY();
      int getId();
      void setId(int);
      
      
      //setPair(double,double);
};

#endif /* BLOCKPOS_HPP_*/