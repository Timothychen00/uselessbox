#include <Wire.h>

String identity = "sound";
String message = "";



struct Map{
  String name;
  void (*pointer)( void );
};

void sound(){
  digitalWrite(D6,1);
  delay(500);
  digitalWrite(D6,0);
}

void sound_bee(){
  digitalWrite(D6,1);
  delay(1000);
  digitalWrite(D6,0);
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

void receiveEvent(size_t length) {
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

void call(const char *name){
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
  pinMode(D6,OUTPUT);
  pinMode(13,OUTPUT);



  call("sound_bee");
}


void loop() {
  if( message != "identity" && message!=""){
    call(message.c_str());
    message="";
  }
}
