/*!
 * @file SmartBurglar.ino
 *
 * @n Connects to the human body's infrared pyroelectric sensor and speaker, 
 * @n and when it detects someone entering, the speaker plays music and publish information to the cloud platform.
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author      [Wuxiao](xiao.wu@dfrobot.com)
 * @version  V1.0
 * @date  2019-02-20
 * @get from https://www.dfrobot.com
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "DFRobot_Iot.h"
#include "Tone32.h"
#include "music_joy.h"

#define SPEARKER_PIN  A4
#define MOTION_SENSOR_PIN  D2

/*Set WIFI name and password*/
const char * WIFI_SSID     = "WIFI_SSID";
const char * WIFI_PASSWORD = "WIFI_PASSWORD";

/*Configure device certificate information*/
String ProductKey = "you_Product_Key";
String ClientId = "12345";
String DeviceName = "you_Device_Name";
String DeviceSecret = "you_Device_Secret";

/*Configure the domain name and port number*/
String ALIYUN_SERVER = "iot-as-mqtt.cn-shanghai.aliyuncs.com";
uint16_t PORT = 1883;

/*Product identifier that needs to be operated*/
String Identifier = "you_Identifier";

/*TOPIC that need to be published*/
const char * pubTopic = "you_pub_Topic";//******post

DFRobot_Iot myIot;
WiFiClient espClient;
PubSubClient client(espClient);

/*play CastleInTheSky*/
static void playCastleInTheSky(){
  for (int i = 0; i < sizeof(CastleInTheSkyData)/sizeof(CastleInTheSkyData[0]); i++) { 
    int noteDuration = CastleInTheSkyDurations[i] *600;
    tone(SPEARKER_PIN, CastleInTheSkyData[i],noteDuration); 
  }
  noTone(SPEARKER_PIN);
}

void connectWiFi(){
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP Adderss: ");
  Serial.println(WiFi.localIP());
}

void callback(char * topic, byte * payload, unsigned int len){
  Serial.print("Recevice [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < len; i++){
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void ConnectCloud(){
  while(!client.connected()){
    Serial.print("Attempting MQTT connection...");
    /*A device connected to the cloud platform based on an automatically calculated username and password*/
    if(client.connect(myIot._clientId,myIot._username,myIot._password)){
      Serial.println("connected");
    }else{
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void setup(){
  Serial.begin(115200);
  pinMode(MOTION_SENSOR_PIN,INPUT);
  
  /*Connect to WIFI*/
  connectWiFi();
  
  /*Initialize the configuration of Aliyun*/
  myIot.init(ALIYUN_SERVER,ProductKey,ClientId,DeviceName,DeviceSecret);
  
  client.setServer(myIot._mqttServer,PORT);
  
  /*Set the callback function to execute the callback function when receiving the subscription information*/
  client.setCallback(callback);
  
  /*Connect to the cloud platform*/
  ConnectCloud();
}

void loop(){
  if(!client.connected()){
    ConnectCloud();
  }
  if(digitalRead(MOTION_SENSOR_PIN) == 1){
    /*Publish information about someone’s intrusion*/
    client.publish(pubTopic,("{\"id\":"+ClientId+",\"params\":{\""+Identifier+"\":1},\"method\":\"thing.event.property.post\"}").c_str());
    /*play CastleInTheSky*/
    playCastleInTheSky();
  }else{
  }
  client.loop();
}