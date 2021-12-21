/*!
 * @file SmartButton.ino
 * @n Simulate the esp32 connects the button module as a remote switch,, 
 * @n and send the status value to the SwitchStatus Topic to remotely control the device that subscribed to the Topic.
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author      [Wuxiao](xiao.wu@dfrobot.com)
 * @version  V1.0
 * @date  2019-10-29
 * @get from https://www.dfrobot.com
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "DFRobot_Iot.h"
#include "Tone32.h"

#define BUTTON_PIN  D2

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

/*TOPIC that need to be published and subscribed*/
const char * pubTopic = "you_pub_Topic";//******post

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
  if(digitalRead(BUTTON_PIN) == 1){
    /*Publish button status*/
    client.publish(pubTopic,("{\"id\":"+ClientId+",\"params\":{\""+Identifier+"\":1},\"method\":\"thing.event.property.post\"}").c_str());
    delay(1000);
  }else{
  }
  client.loop();
}