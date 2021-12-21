/*!
 * @file SmartDimmingLight.ino
 *
 * @brief Connect the dimmable LED module to send dimming information through the cloud platform. 
 * @brief Realize the function of remotely adjusting the light
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

#define BEDROOD_LIGHT D4

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
  const uint16_t LightAdjustLevel = jsonBuffer["params"][Identifier];
  Serial.print("LightAdjustLevel=");
  Serial.println(LightAdjustLevel);
  /*Adjust the light intensity*/
  sigmaDeltaWrite(0, LightAdjustLevel);

  /*Publish the intensity information of the light*/
  String tempMseg = "{\"id\":" + ClientId + ",\"params\":{\"" + Identifier + "\":" + (String)LightAdjustLevel + "},\"method\":\"thing.event.property.post\"}";
  char sendMseg[tempMseg.length()];
  strcpy(sendMseg, tempMseg.c_str());
  client.publish(pubTopic, sendMseg);
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
  /*Initialize the drive pin of the dimmable lamp*/
  sigmaDeltaSetup(0, 312500);
  sigmaDeltaAttachPin(BEDROOD_LIGHT, 0);

  /*Turn off the lights first*/
  sigmaDeltaWrite(0, 0);

  /*Connect to WIFI*/
  connectWiFi();

  /*Initialize the configuration of Aliyun*/
  myIot.init(ALIYUN_SERVER, ProductKey, ClientId, DeviceName, DeviceSecret);

  client.setServer(myIot._mqttServer, PORT);

  /*Set the callback function to execute the callback function when receiving the subscription information*/
  client.setCallback(callback);

  /*Connect to the cloud platform*/
  ConnectCloud();

  /*Publish information about turning off the lights*/
  client.publish(pubTopic, ("{\"id\":" + ClientId + ",\"params\":{\"" + Identifier + "\":0},\"method\":\"thing.event.property.post\"}").c_str());
}

void loop()
{
  if (!client.connected())
  {
    ConnectCloud();
  }
  client.loop();
}