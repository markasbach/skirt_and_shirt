// Skirt and Shirt project for Miriam and Mark
// 
// 2021-06-12, mark.asbach@web.de
// Based on 
// - example code coming with the Adafruit LSM 303 Accel library for the Arduino IDE
// - https://dumblebots.com/2019/06/19/arduino-tutorial-touch-and-capacitive-sensing/



// We include the "header" files that extend the langague with additional commands and data types for the accelerometer sensor.
// These come with the "Library" module that we install in the Arduino integrated development environment (IDE)
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>



// Declare a variable that represents the accelerometer 
// Initialise it and assign a uniqe id (54321) - so 
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);



// Define some names for the pins so we can use them in our code instead of the numbers and everything gets more legible
#define CAPACITY_INPUT_PIN   0
#define CAPACITY_OUTPUT_PIN  1
#define ONBOARD_LED_PIN      7



void setup(void) {

  // Initialise the "serial" interface to the Arduino "serial monitor" so we can print log messages for development
  Serial.begin(9600);
  Serial.println("Accelerometer Test");
  Serial.println("");
  
  // Initialise the accelerometer
  if (!accel.begin()) {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while (true) {
      /* loop endlessly and do nothing */
    }
  }
  
  // Show some basic information on the accelerometer sensor
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" m/s^2");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" m/s^2");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);

  // Try to set the range of the accelerometer to +-4G
  accel.setRange(LSM303_RANGE_4G);

  // Show how the accelerometer range it actually set
  lsm303_accel_range_t new_range = accel.getRange();
  Serial.print("Range set to: ");
  switch (new_range) {
    case LSM303_RANGE_2G:
      Serial.println("+- 2G");
      break;
    case LSM303_RANGE_4G:
      Serial.println("+- 4G");
      break;
    case LSM303_RANGE_8G:
      Serial.println("+- 8G");
      break;
    case LSM303_RANGE_16G:
      Serial.println("+- 16G");
      break;
  }

  // Try to set the mode of the accelerometer to "normal" operation mode
  accel.setMode(LSM303_MODE_NORMAL);
  
  // Show how the accelerometer mode is actually set
  lsm303_accel_mode_t new_mode = accel.getMode();
  Serial.print("Mode set to: ");
  switch (new_mode) {
    case LSM303_MODE_NORMAL:
      Serial.println("Normal");
      break;
    case LSM303_MODE_LOW_POWER:
      Serial.println("Low Power");
      break;
    case LSM303_MODE_HIGH_RESOLUTION:
      Serial.println("High Resolution");
      break;
  }
  
  // Set the correct modes on all pins we want to use directly
  pinMode(CAPACITY_INPUT_PIN, INPUT);
  pinMode(CAPACITY_OUT_PIN, OUTPUT);
  pinMode(ONBOARD_LED_PIN, OUTPUT);
}



void loop(void) {
  
  // Get a new accelerometer sensor event, i.e. ask for new data
  sensors_event_t event;
  accel.getEvent(&event);

  // Display the acceleration (measured in m/s^2)
  Serial.print("X: ");
  Serial.print(event.acceleration.x);
  Serial.print("  ");
  Serial.print("Y: ");
  Serial.print(event.acceleration.y);
  Serial.print("  ");
  Serial.print("Z: ");
  Serial.print(event.acceleration.z);
  Serial.print("  ");
  Serial.println("m/s^2");


  // Drain "all electrons" out of our capacity sensor by setting it to "low" for 10 milliseconds
  digitalWrite(CAPACITY_OUTPUT_PIN, LOW);
  delay(10);

  // Then set it to "high" and start measuring the time
  long startTime = micros();
  digitalWrite(CAPACITY_OUTPUT_PIN, HIGH);

  // Read the input pin again and again as long as it is "low",
  // i.e. wait until the output pin has "filled" the capacity sensor with electrons (so the input pin sees "high")
  while (digitalRead(INCAPACITY_INPUT_PIN) == LOW) {
    // do nothing
  }

  // Take the time it took to "fill" the capacity sensor with electrons
  long sensVal = micros()-startTime;
  Serial.println(sensVal);

  // The longer the time to fill the capacity (over a resistor), the larger the capacity must be.
  // For our experiment, we have observed times like 24 or 16 for the wire alone,
  // and times from 40 to 120 for a finger touching the wire. So we chose 30 as decision boundary
  // between "untouched" and "touched"
  if (sensVal > 30) {
    // If a touch is detected, set the LED to "on"
    digitalWrite(ONBOARD_LED_PIN, HIGH);
  } else {
    // If no touch is detected, set the LED to "off"
    digitalWrite(ONBOARD_LED_PIN, LOW);
  }


  // Delay before the next loop iteration
  delay(500);
}
