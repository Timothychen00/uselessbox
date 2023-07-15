#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <Wire.h>

String identity = "sound";
String message = "";

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

struct Map{
  String name;
  void (*pointer)( void );
};


void sound_open(){ 
    myDFPlayer.play(1);  //Play the first mp3
}

void sound_close(){
    myDFPlayer.play(2);  //Play the first mp3
}

Map gen_map(char name[],void (*pointer)()){
  Map temp;
  temp.name=name;
  temp.pointer=pointer;
  return temp;
}

//fucntion
Map FunctionMap[]={
  gen_map("sound_open",sound_open),
  gen_map("sound_close",sound_close),
};

void receiveEvent() {
  String data = "";
  while (Wire.available()) {
    data += (char)Wire.read();
  }  // print the integer
  message = data;
  Serial.println(data);
}


void requestEvent() {
  Serial.println("[slave]send_identity");
  Serial.println(message);
  if (message == "identity") {
    Wire.write(identity.c_str());
    message="";//clear message
  }
}

void call(char name[]){
  for(int i=0;i<sizeof(FunctionMap)/sizeof(FunctionMap[0]);i++ ){
    if (FunctionMap[i].name==name){
      FunctionMap[i].pointer();
    }
  }
}



void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
    
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(3);  //Play the first mp3

  Wire.begin(15);

  Serial.println("slave");
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  pinMode(14,OUTPUT);
  pinMode(13,OUTPUT);
}

void loop()
{
//  static unsigned long timer = millis();
//  
//  if (millis() - timer > 3000) {
//    timer = millis();
//    myDFPlayer.next();  //Play next mp3 every 1 second.
//  }
  //check player detail
  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
  if( message != "identity" && message!=""){
    call(message.c_str());
    message="";
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}
