#ifndef __DFROBOT_IOT__
#define __DFROBOT_IOT__

#include "Arduino.h"
#define ALIYUN 0
#define ONENET 1

class DFRobot_Iot{
  public:
    DFRobot_Iot(void);
    ~DFRobot_Iot(void);
    /*ONENET*/
    void init(String OneNetServer,
              String OneNetProductID, String OneNetDeviceID,
              String OneNetApiKey, uint16_t OneNetPort = 6002);
    /*Aliyun*/
    void init(String AliyunServer, String AliProductKey, 
              String AliClientId, String AliDeviceName, 
              String AliDeviceSecret, uint16_t AliPort = 1883);
    void setConfig();
    uint8_t _UseServer = ALIYUN;
    
    /*onenet*/
    String _ApiKey;
    String _ProductID;
    String _DeviceID;
    /*aliyun*/
    String _ProductKey;
    String _ClientId;
    String _DeviceName;
    String _DeviceSecret;
    /*public*/
    String _MQTTSERVER;
    uint16_t _port;
    char * _mqttServer;
    char * _clientId;
    char * _username;
    char * _password;
  private:
    
};

#endif
