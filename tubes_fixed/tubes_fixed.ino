#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 INA219_1 (0x40);
Adafruit_INA219 INA219_2 (0x45);
#include <WiFi.h>   
#include <PubSubClient.h> 
#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 19  
#include <Adafruit_INA219.h>
#include <Servo.h>
#define SERVO_MIN 0
#define SERVO_MAX 180
#define SERVO_PIN 13 

DHT dht(DHTPIN, DHTTYPE);
WiFiClient MQTTclient;
PubSubClient client(MQTTclient);

float tegangan_1, arus_1, daya_1;
float tegangan_2, arus_2, daya_2;

const char* mqttServer = "teknik-fisika.or.id";
const int mqttPort = 1883;
const char* ssid = "WifiKu";
const char* password ="sergioaguero";

Adafruit_INA219 sensor219;
Adafruit_INA219 sensor219_2 (0x45);
Servo sg90;

unsigned long currentTime = millis();
const long delayTime = 1000;
unsigned long previousTime = 0;

void callback(char* topic, byte* message, unsigned int length){
  String messageTemp;
  for (int i=0; i<length; i++){
    messageTemp+=(char)message[i];
  }
  Serial.println();
  if (String(topic) == "IoT_Command"){
    Serial.print("Servo Pos :");
    Serial.println(messageTemp);
    sg90.write(messageTemp.toInt());
  }
}

void wifiSetup() {
  Serial.print("Wifi connecting ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); // Connect to WiFi.
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected, IP:");
  Serial.println(WiFi.localIP());  

  client.setServer(mqttServer, mqttPort); // Connect to MQTT
  client.setCallback(callback);
  delay(200);
}

void reconnect(){
  while(!client.connected()){
    Serial.print("Attempting MQTT Connection...");
    if(client.connect("ESP8266Client")){
      Serial.println("connected");
      client.subscribe("IoT_Command");
    } else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}



void setup(void) {
  Serial.begin(9600);
  sg90.attach(SERVO_PIN);
  wifiSetup();
  INA219_1.begin();
  INA219_2.begin();
  dht.begin();
}

void loop(void) {
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  currentTime = millis();
   if(currentTime - previousTime >= delayTime){
    currentTime = millis();
    tegangan_1 = INA219_1.getBusVoltage_V();
    float persentase_baterai = (tegangan_1/3.9)*100; 
    arus_1 = INA219_1.getCurrent_mA(); 
    daya_1 = tegangan_1 * (arus_1 / 1000);
 
    tegangan_2 = INA219_2.getBusVoltage_V();
    arus_2 = INA219_2.getCurrent_mA()*-1; 
    daya_2 = tegangan_2 *arus_2;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    String tegangan_pv = String(tegangan_2);
    String tegangan_baterai = String(persentase_baterai);
    String arus_pv = String(arus_2);
    String daya_pv = String(daya_2);
    String humid = String(h);
    String temperature = String(t);

    int arrLen = tegangan_pv.length()+1;
    char pv_V[arrLen];
    tegangan_pv.toCharArray(pv_V, sizeof(pv_V));

    int arrLen_curr = arus_pv.length()+1;
    char pv_A[arrLen_curr];
    arus_pv.toCharArray(pv_A, sizeof(pv_A));

    int arrLen_daya = daya_pv.length()+1;
    char pv_D[arrLen_curr];
    daya_pv.toCharArray(pv_D, sizeof(pv_D));

    int arrLen_bat = tegangan_baterai.length()+1;
    char baterai_soc[arrLen_bat];
    tegangan_baterai.toCharArray(baterai_soc, sizeof(baterai_soc));

    int arrLen_hum = humid.length()+1;
    char humidity[arrLen_hum];
    humid.toCharArray(humidity, sizeof(humidity));

    int arrLen_temp = temperature.length()+1;
    char temp_[arrLen_hum];
    temperature.toCharArray(temp_, sizeof(temp_));
   
    Serial.print("Persentase : ");Serial.print(persentase_baterai);Serial.print("%");
    Serial.println();
    Serial.print("Tegangan  : ");Serial.print(tegangan_1);Serial.print("V");
    Serial.println();
    Serial.print("Arus 1 : ");Serial.print(arus_1);Serial.print(" mAmp");
    Serial.println();
    Serial.print("Daya 1 : ");Serial.print(daya_1);Serial.print(" Watt");
    Serial.println();
    Serial.println();
    Serial.print("Tegangan 2 : ");Serial.print(tegangan_2);Serial.print(" Volt");
    Serial.println();
    Serial.print("Arus 2 : ");Serial.print(arus_2);Serial.print(" mAmp");
    Serial.println();
    Serial.print("Daya 2 : ");Serial.print(daya_2);Serial.print(" Watt");
    Serial.println();
    Serial.println();
    Serial.print("Humidity : "); Serial.print(h); Serial.print("%");
    Serial.println();
    Serial.print("Temperatur : "); Serial.print(t); Serial.print("C");
    Serial.println();
    Serial.println();
    previousTime = currentTime;
    client.publish("IoT_Sensor/1", pv_V);
    client.publish("IoT_Sensor/2", baterai_soc);
    client.publish("IoT_Sensor/3", pv_A);
    client.publish("IoT_Sensor/4", pv_D);
    client.publish("IoT_Sensor/5", humidity);
    client.publish("IoT_Sensor/6", temp_);
    delay(100);
   }
}
