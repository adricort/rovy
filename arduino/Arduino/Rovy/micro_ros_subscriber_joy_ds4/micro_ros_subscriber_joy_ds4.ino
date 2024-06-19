#include <micro_ros_arduino.h>
#include <ESP32Servo.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <sensor_msgs/msg/joy.h>
//#include <std_msgs/msg/float32_multi_array.h>

rcl_subscription_t subscriber;
sensor_msgs__msg__Joy msg;
rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

#define LED_PIN 13

#define FL_EN_PIN 12
#define BL_EN_PIN 14
#define FL_PWM_PIN 27
#define BL_PWM_PIN 26

#define FR_EN_PIN 25
#define BR_EN_PIN 33
#define FR_PWM_PIN 32
#define BR_PWM_PIN 23

#define panPin 22
#define tiltPin 21

#define PWM_freq 5000

// Channel 0 and 1 are automatically dedicated for the servos ???
#define FL_PWM_Ch 2
#define BL_PWM_Ch 3
#define FR_PWM_Ch 4
#define BR_PWM_Ch 5
#define resolution 8

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

// create four servo objects 
Servo pan;
Servo tilt;
// Published values for SG90 servos; adjust if needed
int pan_deg = 81;
int tilt_deg = 150;

// This two variables independently so that the data transmission for the camera servos is more reliable and softer (data workload distribution)
float left_joystick_x = 0;
float left_joystick_y = 0;

void error_loop(){
  while(1){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(200);
  }
}

void subscription_callback(const void *msgin)
{
  const sensor_msgs__msg__Joy * msg = (const sensor_msgs__msg__Joy *)msgin;
  float pwm_fl = 0;
  float pwm_fr = 0;
  float pwm_bl = 0;
  float pwm_br = 0;
  float left_joystick_x = msg->axes.data[0];
  float left_joystick_y = msg->axes.data[1];
  bool left_arrow = msg->buttons.data[14];
  bool up_arrow = msg->buttons.data[15];
  bool right_arrow = msg->buttons.data[16];
  bool down_arrow = msg->buttons.data[17];
  float right_joystick_x = msg->axes.data[2];
  float right_joystick_y = msg->axes.data[3];
  float left_trigger = msg->axes.data[4];
  float right_trigger = msg->axes.data[5];
  if(right_joystick_y >= 0)
  {
    pwm_fl = (right_joystick_y*150-right_joystick_x*50)*right_trigger;
    pwm_fr = (right_joystick_y*150+right_joystick_x*50)*right_trigger;
    pwm_bl = 0;
    pwm_br = 0;
  }
  else
  {
    pwm_fl = 0;
    pwm_fr = 0;
    pwm_bl = (abs(right_joystick_y)*150-right_joystick_x*50)*right_trigger;
    pwm_br = (abs(right_joystick_y)*150+right_joystick_x*50)*right_trigger;
  }
  ledcWrite(FL_PWM_Ch,int(pwm_fl));
  ledcWrite(FR_PWM_Ch,int(pwm_fr));
  ledcWrite(BL_PWM_Ch,int(pwm_bl));
  ledcWrite(BR_PWM_Ch,int(pwm_br));
  if(left_joystick_x == 0)
    pan_deg += (left_arrow-right_arrow)*10;
  else
    pan_deg = int(left_joystick_x*90)+81;
  if(left_joystick_y == 0)
    tilt_deg += (up_arrow-down_arrow)*10;
  else
    tilt_deg = int(left_joystick_y*90)+150;
  
  if(pan_deg > 170)
    pan_deg = 170;
  else if(pan_deg < 10)
    pan_deg = 10;
  if(tilt_deg > 170)
    tilt_deg = 170;
  else if(tilt_deg < 10)
    tilt_deg = 10;
  digitalWrite(LED_PIN, (right_trigger == 0) ? LOW : HIGH);
  pan.write(pan_deg);
  tilt.write(tilt_deg);
}


void setup() {
  set_microros_transports();
  pinMode(FL_EN_PIN, OUTPUT);
  pinMode(BL_EN_PIN, OUTPUT);
  pinMode(FR_EN_PIN, OUTPUT);
  pinMode(BR_EN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  ledcSetup(FL_PWM_Ch, PWM_freq, resolution);
  ledcSetup(BL_PWM_Ch, PWM_freq, resolution);
  ledcSetup(FR_PWM_Ch, PWM_freq, resolution);
  ledcSetup(BR_PWM_Ch, PWM_freq, resolution);
  ledcAttachPin(FL_PWM_PIN,FL_PWM_Ch);
  ledcAttachPin(BL_PWM_PIN,BL_PWM_Ch);
  ledcAttachPin(FR_PWM_PIN,FR_PWM_Ch);
  ledcAttachPin(BR_PWM_PIN,BR_PWM_Ch);

  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	pan.setPeriodHertz(50);      // Standard 50hz servo
	tilt.setPeriodHertz(50);      // Standard 50hz servo
  pan.attach(panPin, 500, 3000);
	tilt.attach(tiltPin, 600, 3000);

  digitalWrite(LED_PIN, HIGH);

  digitalWrite(FL_EN_PIN, HIGH);
  digitalWrite(BL_EN_PIN, HIGH);
  ledcWrite(FL_PWM_Ch,0);
  ledcWrite(BL_PWM_Ch,0);
  digitalWrite(FR_EN_PIN, HIGH);
  digitalWrite(BR_EN_PIN, HIGH);
  ledcWrite(FR_PWM_Ch,0);
  ledcWrite(FR_PWM_Ch,0);

  //servo1.write(int(servo_pos1));
  //servo2.write(int(servo_pos2)); 
  
  delay(2000);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_arduino_node", "", &support));

  // create subscriber
  RCCHECK(rclc_subscription_init_default(
    &subscriber,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Joy),
    "joy"));

  // Assigning dynamic memory to the frame_id char sequence
  /*msg.header.frame_id.capacity = 100;
  msg.header.frame_id.data = (char*) malloc(msg.axes.capacity * sizeof(char));
  msg.header.frame_id.size = 0;

  // Assigning value to the frame_id char sequence
  strcpy(msg.header.frame_id.data, "base_link");
  msg.header.frame_id.size = strlen(msg.header.frame_id.data);

  // Assigning value to other members
  msg.header.stamp.sec = 10;
  msg.header.stamp.nanosec = 20;*/

  // Init the memory of your array in order to provide it to the executor.
  // If a message from ROS comes and it is bigger than this, it will be ignored, so ensure that capacities here are big enought.
  msg.axes.capacity = 6; 
  msg.axes.data = (float_t*) malloc(msg.axes.capacity * sizeof(float_t));
  msg.axes.size = 0;

  msg.buttons.capacity = 18; 
  msg.buttons.data = (int32_t*) malloc(msg.buttons.capacity * sizeof(int32_t));
  msg.buttons.size = 0;

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA));
}

void loop() {
  delay(1);
  RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(1)));
}