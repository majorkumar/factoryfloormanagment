#include <PubSubClient.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <MQUnifiedsensor.h>

//defining the pin for sensor input and reading.
#define pin 15 //for reading analog output of MQ2 sensor,
#define type 2 //type of 

#define DHTPIN 4
#define DHTTYPE DHT11


int mq_pin = 15;
int mqalarm_pin = 19;       //for alarming MQ2 .
int vib_pin = 2;
int vibalarm_pin = 18;    //for alarming vibration.
int mqthreshold = 250;

DHT dht(DHTPIN, DHTTYPE);

unsigned long lost1 =0,lost2=0, lost3 =0;

const char* ssid = "Optimus";                   // wifi ssid
const char* password =  "Optimus890*";         // wifi password
const char* mqttServer = "10.42.0.193";       // IP adress Raspberry Pi
const int mqttPort = 1883;                   // port at which mqtt is communicating
const char* mqttUser = "amit";             // if you don  v't have MQTT Username, no need input
const char* mqttPassword = "amitsingh";   // if you don't have MQTT Password, no need input

WiFiClient espClient;
PubSubClient client(espClient);
void setup() {
  pinMode(vib_pin, INPUT);
  pinMode(vibalarm_pin, OUTPUT);
  Serial.println(F("DHTxx test!"));

  dht.begin();

  Serial.begin(300);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {

      Serial.println("connected");

    } else {

      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  Serial.println();
  Serial.println("-----------------------");

}


void loop() {
  char _payload[50];
  long vibreading = pulseIn(vib_pin,INPUT);
 
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float DHTreading = (h,t);

  int mqreading = analogRead(mq_pin);
  
  if(millis() -lost1 > 1000){
    lost1 = millis();
     if(vibreading > 1000){
      digitalWrite(vibalarm_pin, HIGH);
      }
      else{
        digitalWrite(vibalarm_pin, LOW);
        }
  String data = String(vibreading) + "/" + String(mqreading) + "/" + String(DHTreading);
  data.toCharArray(_payload,50);
  client.publish("/iot/live", _payload);
    }
  Serial.println(vibreading);
 // Serial.println(***************);

  if(millis() -lost3 > 2000){
    
   if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
    }
   String data = String(vibreading) + "/" + String(mqreading) + "/" + String(DHTreading);
   data.toCharArray(_payload,50);
   client.publish("/iot/liv", _payload);
    
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C \n"));
  delay(1000); 
   }
 
   
   if(millis() -lost2 > 3000){
    lost2 = millis();
      if(mqreading > mqthreshold ){
      digitalWrite(mqalarm_pin, HIGH);
      }
      else{
      digitalWrite(mqalarm_pin, LOW);
      }
  String data = String(vibreading) + "/" + String(mqreading) + "/" + String(DHTreading);
  data.toCharArray(_payload,50);
  client.publish("/iot/log", _payload);
  Serial.println("MQread");
  Serial.println(mqreading);
  delay(3000);
  }
  
   
   if(millis() -lost3 > 30000){
    
     if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
      }
   String data = String(vibreading) + "/" + String(mqreading) + "/" + String(DHTreading);
   data.toCharArray(_payload,50);
   client.publish("/iot/liv", _payload);
    
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C \n"));
  delay(5000);
   }
}
