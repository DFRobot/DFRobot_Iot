# DFRobot IOT

This library can be connected to the Aliyun and ONENET cloud platforms for the development of IoT devices.
<br>

## DFRobot_Iot Library for Arduino

  - [Summary](#summary)
  - [Feature](#feature)
  - [Installation](#installation)
  - [Methods](#methods)
  - [Compatibility](#compatibility)
  - [Credits](#credits)

## Summary

<pre>
Use this library to develop IoT devices
</pre>

## Feature

<pre>
* Compatible with Aliyun and ONENET cloud platforms
* Communicate with the cloud platform using MQTT
</pre>

## Installation

For Arduino
<pre>
Download this library and unzip it to Arduino librarys folder.
</pre>

For Raspberry
<pre>
Download this library and unzip it to a privileged path.
</pre>

## Methods

```cpp

/**
 * @brief Initialize the device of the ONENET platform.
 *
 * @param MQTT connection address of ONENET platform(mqtt.heclouds.com).
 * @param Product id created on the ONENET platform.
 * @param Device id created on the ONENET platform.
 * @param ApiKey for devices created on the ONENET platform.
 * @param Connection port number, the default is 6002
 */
void init(String OneNetServer,
          String OneNetProductID, String OneNetDeviceID,
          String OneNetApiKey, uint16_t OneNetPort = 6002);

/**
 * @brief Initialize the device of the Aliyun platform.
 *
 * @param MQTT connection address of Aliyun platform(iot-as-mqtt.cn-shanghai.aliyuncs.com).
 * @param ProductKey of the device created on the Aliyun platform.
 * @param Own arbitrarily defined ClientId.
 * @param DeviceName of the device created on the Aliyun platform.
 * @param DeviceSecret of the device created on the Aliyun platform.
 * @param Connection port number, the default is 1883
 */
void init(String AliyunServer, String AliProductKey, 
          String AliClientId, String AliDeviceName, 
          String AliDeviceSecret, uint16_t AliPort = 1883);

/**
 * @brief The address of the mqtt Server that actually connects to the cloud platform.
 *
 */
char * _mqttServer;

/**
 * @brief Client id obtained after calculation..
 *
 */
char * _clientId;

/**
 * @brief User name obtained after calculation.
 *
 */
char * _username;

/**
 * @brief Password obtained after calculation.
 *
 */
char * _password;
```

Python methods is similar to cpp

```py


```

## Compatibility

MCU                | Work Well | Not Work Well | Untested  | Remarks
------------------ | :----------: | :----------: | :---------: | -----
FireBeetle-ESP32  |      √       |             |            | 

## Credits

* author [xiao.wu@dfrobot.com]
