#include <SparkFun_RFD77402_Arduino_Library.h>

// Set up LED PWM drivers
short pins[5] = {3,5,6,10,11};
short vals[5] = {0,0,0,0,0};
short brightPos;
short fullPower = 255;
short halfPower = 127;
short quarterPower = 10;
int numReadings = 10;
int readIndex = 0;
int total = 0;
int average = 0;
int readings[200];
unsigned int lastVal1, lastVal2;  // Sensor readings from t-1

RFD77402 Sensor1;//, Sensor2;

void initSmoothing()
{
  for (int i = 0; i < numReadings; i++)
  {
    readings[i] = 0;
  }
}

int smooth(int newValue)
{
  int newAvg;
  total = total - readings[readIndex];
  readings[readIndex] = newValue;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings)
  {
    readIndex = 0;
  }

  newAvg = total / numReadings;
  return newAvg;
}

void setup() {
  Serial.begin(9600);
  initSmoothing();
  while(!Serial);
  Serial.println("SERIAL UP");
  if(!(Sensor1.begin())) { //&& Sensor2.begin())){
    Serial.println("Sensor failed to set up, exiting...");
    while(1); // Spin forever
  }
}

void clearLEDS(short pins[] , short len){
  for(int i=0; i<len; ++i){
    analogWrite(pins[i], 0);  
  }
}

void changeLED(int val)
{
    clearLEDS(pins, 5);
    if(val<150)
    {
      analogWrite(pins[0],fullPower);
      analogWrite(pins[4],quarterPower);
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
      analogWrite(pins[0],quarterPower);
    }
}

void loop() {

  Sensor1.takeMeasurement();
  //Sensor2.takeMeasurement();
  
  // put your main code here, to run repeatedly:


    if(lastVal1 = Sensor1.getDistance()){
      int newVal = smooth(lastVal1);
      changeLED(newVal);
      Serial.print("Sensor 1 read : ");
      Serial.println(newVal, DEC);
    }
    /*
    if(lastVal2 = Sensor2.getDistance()){
      Serial.print("Sensor 2 read : ");
      Serial.println(lastVal2, DEC);      
    }
    */
    //delay(100);
  }

  
