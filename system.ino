#include <SparkFun_RFD77402_Arduino_Library.h>

/*
 * Global declarations
 */

// Set up LED PWM drivers
short pins[5] = {3,5,6,10,11};
short vals[5] = {0,0,0,0,0};
short brightPos;
short fullPower = 255;
short halfPower = 127;
short quarterPower = 10;
int numReadings = 5;
int readIndex1 = 0;
int readIndex2 = 0;
int total1 = 0;
int total2 = 0;
int average1 = 0;
int average2 = 0;
int maxVelocity = 4;
unsigned long currTime;
int readings1[5];
int readings2[5];
unsigned int currVal1, lastVal1;
unsigned int currVal2, lastVal2;  // Sensor readings from t-1

RFD77402 Sensor1;

/*
 * Forward declarations
 */
int checkValue(int newValue, int lastValue, int movingAverage, unsigned long time, int sensorNum);
void initSmoothing();
int getAverage1();
int getAverage2();
void clearLEDS(short pins[] , short len);
void updateAverage1(int newValue);
void updateAverage2(int newValue);
void changeLED(int val);


/*
 * Code
 */

// Determine if the new value is unreasonable, based on the last known value and the moving average.
//  e.g. if the newvalue less the moving average indicates the target is moving too fast, and the new value
// is close to the limit of velocity, reject it.  Otherwise, return the new value.
int checkValue(int newValue, int lastValue, int movingAverage, unsigned long time, int sensorNum){
    long dt = millis() - time;
    float velocity = 1.0*abs(newValue - movingAverage)/dt;
    // If we are moving too fast to be a true reading...
    if(velocity > maxVelocity){
      velocity = 1.0*abs(newValue - lastValue)/dt;
      // and the velocity is not skewed by the moving average...
      if(velocity > maxVelocity){
        return lastValue;
      }
    }

    // The new value is acceptable.  Update moving average
    if(sensorNum == 1){
      updateAverage1(newValue);
    }
    else{
      updateAverage2(newValue);  
    }
    return newValue;
}

void initSmoothing()
{
  for (int i = 0; i < numReadings; i++)
  {
    readings1[i] = 0;
    readings2[i] = 0;
  }
}

// Fetch the current moving average
int getAverage1(){
  return total1 / numReadings;
}

// Fetch the current moving average
int getAverage2(){
  return total2 / numReadings;
}

// Incorporate the new value into the moving average, and return the value
void updateAverage1(int newValue)
{
  int newAvg;
  total1 = total1 - readings1[readIndex1];
  readings1[readIndex1] = newValue;
  total1 = total1 + readings1[readIndex1];
  readIndex1 = readIndex1 + 1;

  if (readIndex1 >= numReadings)
  {
    readIndex1 = 0;
  }
}

// Incorporate the new value into the moving average, and return the value
void updateAverage2(int newValue)
{
  int newAvg;
  total2 = total2 - readings2[readIndex2];
  readings2[readIndex2] = newValue;
  total2 = total2 + readings2[readIndex2];
  readIndex2 = readIndex2 + 1;

  if (readIndex2 >= numReadings)
  {
    readIndex2 = 0;
  }
}

void setup() {
  Serial.begin(9600);
  initSmoothing();
  while(!Serial);
  Serial.println("SERIAL UP");
  if(!(Sensor1.begin())){// && Sensor2.begin())){
    Serial.println("Sensor failed to set up, exiting...");
    while(1); // Spin forever
  }
}


// Reset LEDs to off
void clearLEDS(short pins[] , short len){
  for(int i=0; i<len; ++i){
    analogWrite(pins[i], 0);  
  }
}


// Update LEDs over the active zone according to the distance value (in millimeters)
void changeLED(int val)
{
    //clearLEDS(pins, 5);
    if(val<150)
    {
      analogWrite(pins[0],fullPower);
      analogWrite(pins[1],quarterPower);
    }
    else if( val < 210)
    {
      analogWrite(pins[1],fullPower);
      analogWrite(pins[0],quarterPower);
      analogWrite(pins[2],quarterPower);
    }
    else if (val < 270)
    {
     analogWrite(pins[2],fullPower);
     analogWrite(pins[3],quarterPower);
     analogWrite(pins[1],quarterPower);
    }
    else if(val < 330)
    {
      analogWrite(pins[3],fullPower);
      analogWrite(pins[4],quarterPower);
      analogWrite(pins[2],quarterPower);
    }
    else
    {
      analogWrite(pins[4],halfPower);
      analogWrite(pins[3],quarterPower);
    }
}

void loop() {

  Sensor1.takeMeasurement();
  //Sensor2.takeMeasurement();

  // Update lastVal tracking
  lastVal1 = currVal1;
  //lastVal2 = currVal2;  

  // Clear the LEDs
  clearLEDS(pins, 5);
  
  if(currVal1 = Sensor1.getDistance()){
      currVal1 = checkValue(currVal1, lastVal1, getAverage1(), currTime, 1);
      changeLED(currVal1);
      Serial.print("Sensor 1 read : ");
      Serial.println(currVal1, DEC);
    }
  /*
  if(currVal2 = Sensor2.getDistance()){
      currVal2 = checkValue(currVal2, lastVal2, getAverage2(), currTime, 2);
      changeLED(currVal2);
      Serial.print("Sensor 1 read : ");
      Serial.println(currVal1, DEC);
    }
  */
    currTime = millis();
    delay(100);
  }

  
