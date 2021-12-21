 /*!
  * @file SmartDHT11.ino
  *
  * @brief Connect the temperature and humidity sensor to release the temperature and humidity information to Aliyun.
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
#include "DFRobot_DS18B20.h"

#define DHT11_PIN  D2

/*Set WIFI name and password*/
const char * WIFI_SSID     = "hitest";
const char * WIFI_PASSWORD = "12345678";

/*Configure device certificate information*/
String ProductKey = "a1EI9lrA8si";
String ClientId = "12345";
String DeviceName = "tinkernode";
String DeviceSecret = "acf94fa519f63df4f07887f9b0fd900e";

/*Configure the domain name and port number*/
String ALIYUN_SERVER = "iot-as-mqtt.cn-shanghai.aliyuncs.com";
uint16_t PORT = 1883;

/*Product identifier that needs to be operated*/
String TempIdentifier = "you_Temp_Identifier";
String HumiIdentifier = "you_Humi_Identifier";

/*TOPIC that need to be published and subscribed*/
const char * subTopic = "you_sub_Topic";//****set
const char * pubTopic = "/a1EI9lrA8si/${deviceName}/user/update";//******post

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
      client.subscribe(subTopic);
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

uint8_t tempTime = 0;
void loop(){
  if(!client.connected()){
    ConnectCloud();
  }
  /*One minute publish twice temperature and humidity information*/
  if(tempTime > 60){
    tempTime = 0;
    client.publish(pubTopic,("{\"id\":"+ClientId+",\"params\":{\""+TempIdentifier+"\":"+1+",\""+HumiIdentifier+"\":"+2+"},\"method\":\"thing.event.property.post\"}").c_str());
  }else{
    tempTime++;
    delay(500);
  }
  client.loop();
}
