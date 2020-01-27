//#include <MQUnifiedsensor.h>
#include <DHT.h>
#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//pin for sensor input vcc
#define VIBPIN 12
#define MQ2PIN 13
#define CURRENTPIN 34

//pin for alarming sensor
#define VIBALARM 2
#define MQ2ALARM 4
#define CURALARM 5
#define TEMPALARM 18
#define HUMDALARM 19

#include <WiFi.h>
#define WIFI_SSID "OpTi^^uS"
#define WIFI_PASSWORD "prime100"

#include <PubSubClient.h>
const char* mqttServer = "192.168.137.194";
const int mqttPort = 1883;
WiFiClient espClient;
PubSubClient client(espClient);

// for reading analog value
float t, h;
long int vibreading;
long int mqreading;
float curreading;
char _payload[200];

//for millis function
unsigned long lastMillis1 = 0;
unsigned long lastMillis2 = 0;

void setup()
{
  Serial.begin(300);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  dht.begin();

  client.setServer(mqttServer, mqttPort);
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    }

    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");
      delay(3000);
    }
  }
}


void loop()
{

  t = dht.readTemperature();
  h = dht.readHumidity();
  vibreading = pulseIn(VIBPIN, HIGH);
  mqreading = analogRead(MQ2PIN);
  curreading = analogRead(CURRENTPIN);

  reconnect();

  if (millis() - lastMillis1 > 3000)
  {
    lastMillis1 = millis();
    if(vibreading > 5000){           //starting vibration sensor reading.
     digitalWrite(VIBALARM, HIGH);   //checking for value if exceed than alarm,
     }
     else{
     digitalWrite(VIBALARM, LOW);
     }
    String data = String(vibreading) + "/" + String(mqreading) + "/" + String(curreading) + "/" + String(t) + "/" + String(h) ;
    data.toCharArray(_payload, 200);
    client.publish("/iot/liv", _payload);
  }
  Serial.println(vibreading);

  
  if (millis() - lastMillis2 > 60000)
  {
    lastMillis2 = millis();
    if(mqreading > 2000){             //starting vibration sensor reading.
     digitalWrite(MQ2ALARM, HIGH);   //checking for value if exceed than alarm,
     }
     else{
    digitalWrite(MQ2ALARM, LOW);
     }
    String data = String(vibreading) + "/" + String(mqreading) + "/" + String(curreading) + "/" + String(t) + "/" + String(h) ;
    data.toCharArray(_payload, 200);
    client.publish("/iot/log", _payload);
  }
  Serial.println(mqreading);


  if (millis() - lastMillis2 > 60000)
  {
    lastMillis2 = millis();
    if(curreading > 10){            //starting current sensor reading
   digitalWrite(CURALARM, HIGH);  //checking curent value if exceed than alarm,
    }
   else{
    digitalWrite(CURALARM, LOW);
    }
   String data = String(vibreading) + "/" + String(mqreading) + "/" + String(curreading) + "/" + String(t) + "/" + String(h) ;
    data.toCharArray(_payload, 200);
    client.publish("/iot/log", _payload);
  }
  Serial.println(curreading);
  

  if (millis() - lastMillis1 > 3000)
  {
    lastMillis1 = millis();
    if(t > 50){                        //comparing temprature sensor reading.
     digitalWrite(TEMPALARM, HIGH);   //checking for value if exceed than alarm,
     }
     else{
    digitalWrite(TEMPALARM, LOW);
     }
   String data = String(vibreading) + "/" + String(mqreading) + "/" + String(curreading) + "/" + String(t) + "/" + String(h);
    data.toCharArray(_payload, 200);
    client.publish("/iot/liv", _payload);
  }
   Serial.println(t);

   
  if (millis() - lastMillis1 > 3000)
  {
    lastMillis1 = millis();
    if(h > 70){                       //comparing humidity of sensor reading.
     digitalWrite(HUMDALARM, HIGH);   //checking for value if exceed than alarm,
     }
     else{
    digitalWrite(HUMDALARM, LOW);
     }
    String data = String(vibreading) + "/" + String(mqreading) + "/" + String(curreading) + "/" + String(t) + "/" + String(h) ;
    data.toCharArray(_payload, 200);
    client.publish("/iot/liv", _payload);
  }
   Serial.println(h);
   
}
