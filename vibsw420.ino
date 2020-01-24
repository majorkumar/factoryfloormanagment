int led = 4;
int vs = 2;
void setup() {
  pinMode(led, OUTPUT);
  pinMode(vs, INPUT);
  Serial.begin(9600);
}

void loop() {
  long measurement = vibration();
  delay(50);
  Serial.println(measurement);
  if (measurement > 10000)
  {
    digitalWrite(led, LOW);
    }
    else{
      digitalWrite(led, HIGH);
      }
}
long vibration(){
  long measurement = pulseIn (vs, HIGH); //waiting for then pin to get HIGH and returns measurement
  return measurement;
}  
