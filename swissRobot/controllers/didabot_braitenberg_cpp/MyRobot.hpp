#include <webots/DifferentialWheels.hpp>
#include <webots/DistanceSensor.hpp>

static const int W_LL = 1;
static const int W_LR = -1;
static const int W_RL = -1;
static const int W_RR = 1;

static const int SENSOR_MAX = 1024;
static const int SPEED = 20;
static const int TIME_STEP = 32;

using namespace webots;

class MyRobot : public DifferentialWheels {
  private:
    DistanceSensor *ir0;
    DistanceSensor *ir1;
    
    double right_norm;
    double left_norm;
    
    double left_speed, right_speed;
    double right_sensor, left_sensor;
    
	
  public:

    MyRobot();
    virtual ~MyRobot();

    void run();
};
