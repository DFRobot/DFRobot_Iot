/*!
 * @file SmartButton.ino
 * @brief Simulate the esp32 plus button module as a remote switch, 
 * @brief and send the status value to the SwitchStatus Topic to remotely control the device that subscribed to the Topic.
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

#define BUTTON_PIN  D2
bool SwitchStatus ;
/*Set WIFI name and password*/
const char * WIFI_SSID     = "WIFI_SSID";
const char * WIFI_PASSWORD = "WIFI_PASSWORD";

/*Configure device certificate information*/
String ProductID = "you_ProductID";
String DeviceId  = "you_DeviceId";
String ApiKey    = "you_ApiKey";

/*Configure the domain name and port number*/
String ONENET_SERVER = "mqtt.heclouds.com";
uint16_t PORT = 6002;

/*Set the Topic you need to publish to*/
const char *pubTopic = "you_pub_Topic";

DFRobot_Iot myIot;
WiFiClient espClient;
PubSubClient client(espClient);

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
  pinMode(BUTTON_PIN,INPUT);
  
   /*Connect to WIFI*/
  connectWiFi();
  
  /*Initialize the configuration of OneNet*/
  myIot.init(ONENET_SERVER, ProductID, DeviceId, ApiKey);
  
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
  if(digitalRead(BUTTON_PIN) == 1){
    if(SwitchStatus){
      SwitchStatus = false;
      client.publish(pubTopic,"ON");
    }else{
      SwitchStatus = true;
      client.publish(pubTopic,"OFF");
    }
    delay(1000);
  }
  client.loop();
}