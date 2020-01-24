int led = 2;
int buzzer=4;  
int MQ2 = 15;
int sensorthreshold = 4000;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(MQ2, OUTPUT);
  Serial.begin(300);
 }

 void loop() {
  int analogSensor = analogRead(MQ2);
  Serial.print("pin MQ2:  ");
  Serial.println(analogSensor);
  if(analogSensor > sensorthreshold ){
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH);
    }
   else{
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH);
   
    }
  delay(300);
  }
