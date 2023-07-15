#include <Wire.h>

String identity = "sound";
String message = "";



struct Map{
  String name;
  void (*pointer)( void );
};

void sound(){
  digitalWrite(14,1);
  delay(500);
  digitalWrite(14,0);
}

void sound_bee(){
  digitalWrite(14,1);
  delay(1000);
  digitalWrite(14,0);
}




Map gen_map(char name[],void (*pointer)()){
  Map temp;
  temp.name=name;
  temp.pointer=pointer;
  return temp;
}

//fucntion
Map FunctionMap[]={
  gen_map("sound",sound),
  gen_map("sound_bee",sound_bee),
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

void setup() {
  // put your setup code here, to run once:
  Wire.begin(15);
  Serial.begin(9600);
  Serial.println("slave");
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  pinMode(14,OUTPUT);
  pinMode(13,OUTPUT);



  call("sound_bee");
}


void loop() {
  if( message != "identity" && message!=""){
    call(message.c_str());
    message="";
  }
}
