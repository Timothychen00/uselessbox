#include <Servo.h>   

const int In1 = 4;
const int In2 = 5;  

Servo myservo;

void setup() {
  myservo.attach(9); 
  // put your setup code here, to run once:

  pinMode(2,INPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT); 
  Serial.begin(9600);
  myservo.write(60);
}

int button=0;
void loop() {
  // put your main code here, to run repeatedly:
  button=digitalRead(2);
  if(button){
    Serial.println("open");
    myservo.write(120);
    // mopen();
    // delay(500);
    // mstop();
    while(button)
      button=digitalRead(2);
    while(!button)
      button=digitalRead(2);
    if(button){
      Serial.println("close");
      myservo.write(60);
      // mclose();
      delay(2500);
      // mstop();
    }
  }
  Serial.println(button);
  // mfront();
  // delay(125);
  // mstop();
  // delay(2000);
  // // mstop();

}

void mclose(){
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
}
void mopen(){
  // digitalWrite(In1, LOW);
  // digitalWrite(In2, HIGH);
  analogWrite(In1,0);
  analogWrite(In2, 120);
}

void mstop(){
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
}
   


