/*!
 * @file Livingroom_Door.ino
 *
 * @brief Use esp32 to connect relays and electronic locks to achieve remote unlocking
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

#define SET_DOOR_PIN D2
#define READ_DOOR_PIN D3

/*Set WIFI name and password*/
const char *WIFI_SSID = "WIFI_SSID";
const char *WIFI_PASSWORD = "WIFI_PASSWORD";

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
const char *subTopic = "you_sub_Topic"; //****set
const char *pubTopic = "you_pub_Topic"; //******post

DFRobot_Iot myIot;
WiFiClient espClient;
PubSubClient client(espClient);

uint8_t readDoorStatus()
{
  return digitalRead(READ_DOOR_PIN);
}
static void openDoor()
{
  digitalWrite(SET_DOOR_PIN, HIGH);
  delay(500);
  digitalWrite(SET_DOOR_PIN, LOW);
}

void connectWiFi()
{
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP Adderss: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int len)
{
  Serial.print("Recevice [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < len; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  StaticJsonDocument<300> jsonBuffer;
  deserializeJson(jsonBuffer, (const char *)payload);
  auto error = deserializeJson(jsonBuffer, (const char *)payload);
  if (error)
  {
    Serial.println("parseObject() failed");
    return;
  }
  const uint16_t DoorStatus = jsonBuffer["params"][Identifier];
  Serial.print("readDoorStatus=");
  Serial.print(DoorStatus);
  if (DoorStatus == 1)
  {
    openDoor();
  }
}

void ConnectCloud()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    /*A device connected to the cloud platform based on an automatically calculated username and password*/
    if (client.connect(myIot._clientId, myIot._username, myIot._password))
    {
      Serial.println("connected");
      client.subscribe(subTopic);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
void setup()
{
  Serial.begin(115200);
  pinMode(SET_DOOR_PIN, OUTPUT);
  pinMode(READ_DOOR_PIN, INPUT_PULLUP);
  digitalWrite(SET_DOOR_PIN, LOW);

  /*Connect to WIFI*/
  connectWiFi();

  /*Initialize the configuration of Aliyun*/
  myIot.init(ALIYUN_SERVER, ProductKey, ClientId, DeviceName, DeviceSecret);

  client.setServer(myIot._mqttServer, PORT);

  /*Set the callback function to execute the callback function when receiving the subscription information*/
  client.setCallback(callback);

  /*Connect to the cloud platform*/
  ConnectCloud();

  /*Publish the status of the door lock status*/
  client.publish(pubTopic, ("{\"id\":" + ClientId + ",\"params\":{\"" + Identifier + "\":" + readDoorStatus() + "},\"method\":\"thing.event.property.post\"}").c_str());
}
uint8_t tempDoorStatus = 0;
uint8_t lastDoorStatus = 0;
void loop()
{
  if (!client.connected())
  {
    ConnectCloud();
  }
  tempDoorStatus = readDoorStatus();
  if (tempDoorStatus != lastDoorStatus)
  {
    lastDoorStatus = tempDoorStatus;
    /*Publish the status of the door lock status*/
    client.publish(pubTopic, ("{\"id\":" + ClientId + ",\"params\":{\"" + Identifier + "\":" + tempDoorStatus + "},\"method\":\"thing.event.property.post\"}").c_str());
    delay(500);
  }
  client.loop();
}