#include <Wire.h>
#define MAX_LENGTH 64

struct pair {
  int id;
  String identity;
};

pair slaves[MAX_LENGTH];
// int config[]={
//   [""]
// }
// slaves [id,identity]

bool slaves_status[MAX_LENGTH]={0};
int seek_final(){
  for(int i=0;i<128;i++){
    if(!slaves[i].id)
      return i;
  }
}

void update_slaves(){
  int times=0;
  Serial.println("[update_slaves]");
  for (int addr=0;addr<MAX_LENGTH;addr++){
    Wire.beginTransmission(addr);
    auto result=Wire.endTransmission();
    // Serial.println("test");
    if (result){
      // Serial.println("failed"+String(addr));
    }else{
      slaves_status[addr]=true;
      slaves[times].id=addr;
      times++;
    }
  }
  check_slaves_status();//output slaves boolean
  check_identity();//ask for identity update
  check_slaves();//output slaves array
}

void check_slaves_status(){
  Serial.println("\n[check_slaves_status]");
  for (int addr=0;addr<MAX_LENGTH;addr++){
    if (slaves_status[addr])
      Serial.println(addr+String(":")+slaves_status[addr]);
  }
}

void check_slaves(){
  Serial.println("[check_slaves]");
  for (int addr=0;addr<MAX_LENGTH;addr++){
    if (slaves[addr].id)
      Serial.println(addr+String(":")+slaves[addr].id+String(" identity:")+slaves[addr].identity);
  }
}

String get_data(int id){
  Serial.print("[get_data]");
  Wire.beginTransmission(id);
  Wire.requestFrom(id, 10);    // request 6 bytes from slave device #8
  delay(100);
  String data="";
  auto available=Wire.available();
  Serial.println(String("available:")+available);
  while (available) {
    int temp=Wire.read();
    Serial.print(temp);
    if(temp==255)
      break;
    data+=char(temp);

    available=Wire.available();
  }
  Serial.println(String("\n")+data);

  auto result2=Wire.endTransmission();
  return data;
}

void send_data(int id,char data[]){
  Wire.beginTransmission(id);
  Wire.write(data);
  auto result=Wire.endTransmission();
}

int check_identity(){
  int final_id=seek_final();
  for(int i=0;i<final_id;i++){
    send_data(slaves[i].id,"identity");
    delay(50);
    String result=get_data(slaves[i].id);
    slaves[i].identity=result;//store identity of slaves
  }
}

String command(char identity[],char Function[]){
  Serial.print("[command]");
  int id=-1;
  for (int i=0;i<seek_final();i++){
    if (slaves[i].identity==identity){
      Serial.println(slaves[i].id+String(" ")+slaves[i].identity);
      id=slaves[i].id;
      break;
    }
  }
  Serial.println(String("id:")+id);
  if(id!=-1){
    send_data(id,Function);
    return "success";
  }else{
    update_slaves();
    delay(100);
    return command(identity,Function);
  }
}


void setup() {
  Wire.begin();//master end
  Serial.begin(9600);
  Serial.println("\n\nstart=============");
  delay(5000);
  update_slaves();
  command("sound","sound_open");
//  delay(1000);
//  command("sound","sound_bee");

}

void loop() {
  // update_slaves();

  // delay(5000);
}
