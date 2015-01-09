#include "blockPos.hpp"

BlockPos::BlockPos(double x,double y, int idInit):pos(x,y)
{
  id = idInit;
}

BlockPos::~BlockPos(){}

pair<double,double> BlockPos::operator()()
{
  return pos;
}

double BlockPos::getX()
{
  return pos.first;
}

double BlockPos::getY()
{
  return pos.second;
}

void BlockPos::setId(int idInit)
{
  id = idInit;
}

int BlockPos::getId()
{
  return id;
}