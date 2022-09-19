#include "SoftwareSerial.h"
SoftwareSerial serial_connection(10, 11);//Create a serial connection with TX and RX on these pins
#define BUFFER_SIZE 64//This will prevent buffer overruns.
char inData[BUFFER_SIZE];//This is a character buffer where the data sent by the python script will go.
char inChar=-1;//Initialie the first character as nothing
int count=0;//This is the number of lines sent in from the python script
int i=0;//Arduinos are not the most capable chips in the world so I just create the looping variable once
int pwm1  = 6; 
int pwm2 = 5 ; 

int in1 = A5 ;
int in2 = A4 ;
int in3 = A3; 
int in4 = A2 ;

int outSignalLeft = 0 ;
int outSignalRight= 0;
int baseSpeed = 60;

void setup()
{
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(9600);//Initialize communications to the serial monitor in the Arduino IDE
  serial_connection.begin(9600);//Initialize communications with the bluetooth module
  //serial_connection.println("Ready!!!");//Send something to just start comms. This will never be seen.
  //Serial.println("Started");//Tell the serial monitor that the sketch has started.
  
}
void loop()
{
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);
  String command ="";
  //int intensity = 0 ;
  //This will prevent bufferoverrun errors
  byte byte_count=serial_connection.available();//This gets the number of bytes that were sent by the python script
  if(byte_count)//If there are any bytes then deal with them
  {
    //Serial.println("Incoming Data");//Signal to the monitor that something is happening
    int first_bytes=byte_count;//initialize the number of bytes that we might handle. 
    int remaining_bytes=0;//Initialize the bytes that we may have to burn off to prevent a buffer overrun
    if(first_bytes>=BUFFER_SIZE-1)//If the incoming byte count is more than our buffer...
    {
      remaining_bytes=byte_count-(BUFFER_SIZE-1);//Reduce the bytes that we plan on handleing to below the buffer size
    }
    for(i=0;i<first_bytes;i++)//Handle the number of incoming bytes
    {
      inChar=serial_connection.read();//Read one byte
      inData[i]=inChar;//Put it into a character string(array)
    }
    inData[i]='\0';//This ends the character array with a null character. This signals the end of a string
    //Serial.println(String(inData));
    command = getValue(String(inData),'&',0);
    //intensity = getValue(String(inData),'&',1).toInt();
    //Serial.println(intensity);
    
    
    for(i=0;i<remaining_bytes;i++)//This burns off any remaining bytes that the buffer can't handle.
    {
      inChar=serial_connection.read();
    }
    
   //Serial.println(command);//Print to the monitor what was detected
   // Serial.println(intensity);//Print to the monitor what was detected
    // if no command is detected make the current input decay linearly
    if(command == "straight")
    {
     outSignalLeft=baseSpeed;
     outSignalRight=baseSpeed;
    }
    else if(command  == "right")
    {

     outSignalLeft=0;
     outSignalRight=baseSpeed;
    }
    else if(command  == "left")
    {
      outSignalLeft=baseSpeed;
      outSignalRight=0;
    }

      }
  
  analogWrite(5, outSignalLeft);
  analogWrite(6, outSignalRight);
  //Serial.println(outSignal);
  delay(50);//Pause for a moment 
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
