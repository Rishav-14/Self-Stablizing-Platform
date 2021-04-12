#include<Wire.h>
#include<Servo.h>

int servoPin = 10;
Servo myservo;
 
const int MPU_addr=0x68;

int minVal=265;
int maxVal=402;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
double x;
double y;
double z;

//initialization
void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(9600);
  myservo.attach(10);
  myservo.write(90);
}

//function to get acceleration data and calculates rotation about various axes
void get_acceleration_values(){
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);
  
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  
  int xAng = map(AcX, minVal, maxVal,-90, 90);
  int yAng = map(AcY, minVal, maxVal,-90, 90);
  int zAng = map(AcZ, minVal, maxVal,-90, 90);
  
  x = RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

  Serial.print("Angle X = ");
  Serial.println(x);
 
  Serial.print("Angle Y = ");
  Serial.println(y);
 
  Serial.print("Angle Z = ");
  Serial.println(z);
  Serial.println("");
}

//function used to readjust the servo motor's orientation
void stabilize(){
  if(y > 180){
    y = 360 - y;
    myservo.write(90 - y);
    return;
  }
  myservo.write(90 + y);
}

void loop(){
  get_acceleration_values();
  stabilize();
}
