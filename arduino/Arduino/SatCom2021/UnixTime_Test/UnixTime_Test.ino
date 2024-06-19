//time_make_and_add
#include "TimeLib.h"

tmElements_t te;  //Time elements structure
time_t unixTime; // a time stamp

int i = 0;

void setup() {
  Serial.begin(115200);
  te.Second = 0;
  te.Hour = 23; //11 pm
  te.Minute = 17;
  te.Day = 7;
  te.Month = 6;
  te.Year = 2017 - 1970; //Y2K, in seconds = 946684800UL
  unixTime =  makeTime(te);
  setTime(unixTime); //set the current time to the above entered
}

void loop() {
  Serial.println(now());}
