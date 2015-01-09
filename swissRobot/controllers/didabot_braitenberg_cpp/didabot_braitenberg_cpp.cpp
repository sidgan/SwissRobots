#include "MyRobot.hpp"

int main(int argc, char **argv) {
  
  MyRobot *robot = new MyRobot();
  robot->run();
  
  delete robot;
  
  return 0;
}