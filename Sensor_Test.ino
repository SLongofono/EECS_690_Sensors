#include <SparkFun_RFD77402_Arduino_Library.h>

// Set up LED PWM drivers
short pins[5] = {3,5,6,10,11};
short vals[5] = {0,0,0,0,0};
short brightPos;
short fullPower = 255;
short halfPower = 127;
short quarterPower = 63;
unsigned int lastVal1, lastVal2;  // Sensor readings from t-1

RFD77402 Sensor1;//, Sensor2;

void setup() {
  Serial.begin(9600);
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

void loop() {

  Sensor1.takeMeasurement();
  //Sensor2.takeMeasurement();
  
  // put your main code here, to run repeatedly:
  for(brightPos =0; brightPos<5; ++brightPos){
    clearLEDS(pins, 5);
    analogWrite(pins[brightPos],halfPower);
    if(brightPos > 0){
      analogWrite(pins[brightPos - 1], quarterPower);
    }
    if(brightPos < 4){
      analogWrite(pins[brightPos + 1], quarterPower);
    }

    if(lastVal1 = Sensor1.getDistance()){
      Serial.print("Sensor 1 read : ");
      Serial.println(lastVal1, DEC);
    }
    /*
    if(lastVal2 = Sensor2.getDistance()){
      Serial.print("Sensor 2 read : ");
      Serial.println(lastVal2, DEC);      
    }
    */
    delay(500);
  }

  
}
