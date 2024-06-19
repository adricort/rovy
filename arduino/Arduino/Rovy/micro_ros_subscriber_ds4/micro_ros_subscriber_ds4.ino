#include <micro_ros_arduino.h>
#include <ESP32Servo.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <geometry_msgs/msg/twist.h>

rcl_subscription_t subscriber;
geometry_msgs__msg__Twist msg;
rclc_executor_t executor;
rcl_allocator_t allocator;
rclc_support_t support;
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

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}

#define PWM_freq 5000
#define FL_PWM_Ch 0
#define BL_PWM_Ch 1
#define FR_PWM_Ch 2
#define BR_PWM_Ch 3
#define resolution 8

// create four servo objects 
Servo servo1;
Servo servo2;
// Published values for SG90 servos; adjust if needed
int minUs = 1000;
int maxUs = 2000;
int servo1Pin = 6;
int servo2Pin = 7;
int pos = 0;      // position in degrees
ESP32PWM pwm;

void error_loop(){
  while(1){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(100);
  }
}

//twist message cb
void subscription_callback(const void *msgin) {
  const geometry_msgs__msg__Twist * msg = (const geometry_msgs__msg__Twist *)msgin;
  int pwm_fl = 0;
  int pwm_fr = 0;
  int pwm_bl = 0;
  int pwm_br = 0;
  if(msg->angular.y >= 0)
  {
    pwm_fl = ((msg->angular.y)*800+(msg->angular.x)*400)*(msg->linear.z);
    pwm_fr = ((msg->angular.y)*800-(msg->angular.x)*400)*(msg->linear.z);
    pwm_bl = 0;
    pwm_br = 0;
  }
  else
  {
    pwm_fl = 0;
    pwm_fr = 0;
    pwm_bl = (abs((msg->angular.y)*800)+(msg->angular.x)*400)*(msg->linear.z);
    pwm_br = (abs((msg->angular.y)*800)-(msg->angular.x)*400)*(msg->linear.z);
  }
  ledcWrite(FL_PWM_Ch,pwm_fl);
  ledcWrite(FR_PWM_Ch,pwm_fr);
  ledcWrite(BL_PWM_Ch,pwm_bl);
  ledcWrite(BR_PWM_Ch,pwm_br);
  servo1.write(pos);
  servo2.write(pos);
  digitalWrite(LED_PIN, (msg->linear.y == 0) ? LOW : HIGH);
}

//ros2 topic pub -r 10 twist_subscriber geometry_msgs/msg/Twist "{linear: {x:   1.0, y: 0.0, z: 0.0}, angular: {x: 0.0, y: 0.0, z: 0.0}}"
void setup() {
  set_microros_transports();
  pinMode(FL_EN_PIN, OUTPUT);
  pinMode(BL_EN_PIN, OUTPUT);
  //pinMode(FL_PWM_PIN, OUTPUT);
  //pinMode(BL_PWM_PIN, OUTPUT);
  pinMode(FR_EN_PIN, OUTPUT);
  pinMode(BR_EN_PIN, OUTPUT);
  //pinMode(FR_PWM_PIN, OUTPUT);
  //pinMode(BR_PWM_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

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
	Serial.begin(115200);
	servo1.setPeriodHertz(50);      // Standard 50hz servo
	servo2.setPeriodHertz(50);      // Standard 50hz servo
  servo1.attach(servo1Pin, minUs, maxUs);
	servo2.attach(servo2Pin, minUs, maxUs);

  digitalWrite(LED_PIN, HIGH);

  digitalWrite(FL_EN_PIN, HIGH);
  digitalWrite(BL_EN_PIN, HIGH);
  ledcWrite(FL_PWM_Ch,0);
  ledcWrite(BL_PWM_Ch,0);
  digitalWrite(FR_EN_PIN, HIGH);
  digitalWrite(BR_EN_PIN, HIGH);
  ledcWrite(FR_PWM_Ch,0);
  ledcWrite(FR_PWM_Ch,0);
  
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
    ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist),
    "cmd_vel"));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_subscription(&executor, &subscriber, &msg, &subscription_callback, ON_NEW_DATA));

}

void loop() {
  delay(100);
  RCCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(1)));
  //servo1.detach();
	//servo2.detach();
}
