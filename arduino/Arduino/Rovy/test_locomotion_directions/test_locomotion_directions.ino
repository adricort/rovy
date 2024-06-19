#include <micro_ros_arduino.h>
#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <sensor_msgs/msg/joy.h>

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
#define PWM_freq 5000

// Channel 0 and 1 are automatically dedicated for the servos ???
#define FL_PWM_Ch 0
#define BL_PWM_Ch 1
#define FR_PWM_Ch 2
#define BR_PWM_Ch 3
#define resolution 8

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

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
  bool L1_button = msg->buttons.data[4];
  bool L2_button = msg->buttons.data[5];
  bool R1_button = msg->buttons.data[6];
  bool R2_button = msg->buttons.data[7];

  ledcWrite(FL_PWM_Ch,100*L2_button);
  ledcWrite(FR_PWM_Ch,100*R2_button);
  ledcWrite(BL_PWM_Ch,100*L1_button);
  ledcWrite(BR_PWM_Ch,100*R1_button);
  digitalWrite(LED_PIN, (R2_button == 0) ? LOW : HIGH);
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
    ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Joy),
    "joy"));

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