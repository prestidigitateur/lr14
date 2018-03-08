#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define RightMotorSpeed 4
#define RightMotorDir   0  
#define LeftMotorSpeed  5  
#define LeftMotorDir    2


WidgetTerminal terminal(V0);

char auth[] = "6c1116f158624b759436e2c6676f3ef9";
char ssid[] = "Keenetic-5849";
char pass[] = "uwHE2Ve3";

 
void moveControl(int x, int y)
{
  x=x-512;
  y=y-512;
    //rotate XY to -45' (= 315')
  // x = right; y = left
  int xN = x*cos(315)+y*sin(315);
  int yN = y*cos(315)-x*sin(315);

  bool xDir = xN>0;
  bool yDir = yN>0;

  int xSpd = map(abs(xN), 0, 512, 400, 1024);
  int ySpd = map(abs(yN), 0, 512, 400, 1024);

  if(y>0)
  {
    int temp = xSpd;
    xSpd = ySpd;
    ySpd = temp;

    bool temp2 = xDir;
    xDir = yDir;
    yDir = temp2;
  }
  
  
  String st = "X="+String(xN)+"; Y="+String(yN)+"\r\n";
  st += "rDir="+String(xDir)+"; lDir="+String(yDir)+"\r\n";
  st += "rSpd="+String(xSpd)+"; lSpd="+String(ySpd)+"\r\n";
  terminal.println(st);


    Blynk.virtualWrite(V4, yDir);
    Blynk.virtualWrite(V5, xDir);
    digitalWrite(RightMotorDir,xDir);  
    digitalWrite(LeftMotorDir,yDir);
    Blynk.virtualWrite(V2, ySpd);
    Blynk.virtualWrite(V3, xSpd);
    analogWrite(RightMotorSpeed,xSpd); 
    analogWrite(LeftMotorSpeed,ySpd);
}
 
void setup()
{
  // initial settings for motors off and direction forward
  pinMode(RightMotorSpeed, OUTPUT);
  pinMode(LeftMotorSpeed, OUTPUT);
  pinMode(RightMotorDir, OUTPUT);
  pinMode(LeftMotorDir, OUTPUT);
  digitalWrite(RightMotorSpeed, LOW);
  digitalWrite(LeftMotorSpeed, LOW);
  digitalWrite(RightMotorDir, HIGH);
  digitalWrite(LeftMotorDir,HIGH);


 
  Blynk.begin(auth, ssid, pass);
 }
 
 
void loop()
{
  Blynk.run();
}
 
 
BLYNK_WRITE(V1)
{
  int x = param[0].asInt();
  int y = param[1].asInt();
  moveControl(x,y); 
}
