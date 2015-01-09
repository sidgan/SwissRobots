#include "MyRobot.hpp"
#include "../supervisor/simSettings.hpp"

using namespace webots;

 
    MyRobot::MyRobot() : DifferentialWheels() {
      ir0 = getDistanceSensor("irRight");
      ir1 = getDistanceSensor("irLeft");
      ir0->enable(TIME_STEP);
      ir1->enable(TIME_STEP);
      
      left_speed = 0;
      right_speed = 0;
      
      right_norm = 0;
      left_norm = 0;
    
      right_sensor = 0;
      left_sensor = 0;
    }
    
    
    MyRobot::~MyRobot() {}


    void MyRobot::run() 
    {
      //int loopFactor = (int)(SIMTIME*1000/TIME_STEP);
      //int i = 0;
    
      // Main control loop
      while (step(TIME_STEP) != -1)
      {
        //if(i++>loopFactor)
        //  break;  
      
        left_sensor = ir0->getValue();
        right_sensor = ir1->getValue();
       
        left_norm = left_sensor/SENSOR_MAX;
        right_norm = right_sensor/SENSOR_MAX;
      
        if (left_norm==0 && right_norm==0)
        {
          left_speed = SPEED;
          right_speed = SPEED;
        }
        else if(left_norm == right_norm)
        {
          left_speed = -SPEED;
          right_speed = -0.2*SPEED;
        }
        else
        {
          //left_speed = FORW_VEL;
          left_speed = (SPEED)*left_norm*W_LL;
          left_speed = left_speed + (SPEED)*right_norm*W_RL;
          
          //right_speed = FORW_VEL;
          right_speed = (SPEED)*right_norm*W_RR;
          right_speed = right_speed + (SPEED)*left_norm*W_LR;
        }
        
        //printf("%f %f ; %f %f\n",left_norm,right_norm,left_speed,right_speed);

        setSpeed(-left_speed, -right_speed);
      }
      
      //while (step(TIME_STEP) != -1){}
    }


