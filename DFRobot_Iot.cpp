#include "DFRobot_Iot.h"
#include "HMAC_SHA1.h"
#include <String.h>

CHMAC_SHA1 MyHmac_Sha1;
String byteToHexStr(unsigned char byte_arr[], int arr_len)  
{  
    String hexstr;  
    for (int i=0;i<arr_len;i++)  
    {  
        char hex1;  
        char hex2;  
        int value=byte_arr[i]; 
        int v1=value/16;  
        int v2=value % 16;  
  
        if (v1>=0&&v1<=9)  
            hex1=(char)(48+v1);  
        else  
            hex1=(char)(55+v1);  
  
        if (v2>=0&&v2<=9)  
            hex2=(char)(48+v2);  
        else  
            hex2=(char)(55+v2);  
  
        hexstr=hexstr+hex1+hex2;  
    }  
    return hexstr;  
} 

DFRobot_Iot :: DFRobot_Iot(void){
    
}

DFRobot_Iot :: ~DFRobot_Iot(void){
    
}
/*Use ONENET cloud platform*/
void DFRobot_Iot :: init(String OneNetServer,
                            String OneNetProductID, String OneNetDeviceID,
                            String OneNetApiKey, uint16_t OneNetPort)
                            {
    this->_MQTTSERVER    = OneNetServer;
    this->_ProductID     = OneNetProductID;
    this->_DeviceID      = OneNetDeviceID;
    this->_ApiKey        = OneNetApiKey;
    this->_port          = OneNetPort;
    this->_UseServer     = ONENET;
    setConfig();
}
/*Use Aliyun cloud platform*/
void DFRobot_Iot :: init(String AliyunServer, String AliProductKey, 
                            String AliClientId, String AliDeviceName, 
                            String AliDeviceSecret, uint16_t AliPort)
                            {
    this->_MQTTSERVER    = AliyunServer;
    this->_ProductKey    = AliProductKey;
    this->_ClientId      = AliClientId;
    this->_DeviceName    = AliDeviceName;
    this->_DeviceSecret  = AliDeviceSecret;
    this->_port          = AliPort;
    this->_UseServer     = ALIYUN;
    setConfig();
}
/*alculate the connection username and password, etc.*/
void DFRobot_Iot :: setConfig(){
    if(this->_UseServer == ONENET){
        String tempSERVER = this->_MQTTSERVER;
        uint8_t len = tempSERVER.length();
        if(this->_mqttServer == NULL){
            this->_mqttServer = (char *) malloc(len + 1);
        }
        memset(this->_mqttServer, 0, tempSERVER.length() + 1);
        memcpy(this->_mqttServer,tempSERVER.c_str(), tempSERVER.length()+1);
        //strcpy(this->_mqttServer,tempSERVER.c_str());

        String tempID = this->_DeviceID;
        len = tempID.length();
        if(this->_clientId == NULL){
            this->_clientId = (char *) malloc(len + 1);
        }
        memset(this->_clientId, 0, tempID.length() + 1);
        memcpy(this->_clientId, tempID.c_str(), tempID.length());
        //strcpy(this->_clientId,tempID.c_str());

        String tempName = this->_ProductID;
        len = tempName.length();
        this->_username = (char * )malloc(len + 1);
        memset(this->_username, 0, tempName.length() + 1);
        memcpy(this->_username, tempName.c_str(), tempName.length());
        //strcpy(this->_username,tempName.c_str());
    
        String tempPass = this->_ApiKey;
        if(this->_password == NULL){
            this->_password = (char *) malloc(tempPass.length() + 1);
        }
        memset(this->_password, 0, tempPass.length() + 1);
        memcpy(this->_password, tempPass.c_str(), tempPass.length());
        //strcpy(this->_password,tempPass.c_str());
    }else if(this->_UseServer == ALIYUN){
        String tempSERVER = (this->_ProductKey + "." + this->_MQTTSERVER);
        uint8_t len = tempSERVER.length();
        uint16_t timestamp = 49;
        if(this->_mqttServer == NULL){
            this->_mqttServer = (char *) malloc(len + 1);
        }

        memset(this->_mqttServer, 0, tempSERVER.length() + 1);
        memcpy(this->_mqttServer, tempSERVER.c_str(), tempSERVER.length());
        //strcpy(this->_mqttServer,tempSERVER.c_str());
        String tempID = (this->_ClientId + 
                         "|securemode=3"+
                         ",signmethod=" + "hmacsha1"+
                         ",timestamp="+(String)timestamp+"|");
        len = tempID.length();
        if(this->_clientId == NULL){
            this->_clientId = (char *) malloc(len + 1);
        }
        memset(this->_clientId, 0, tempID.length() + 1);
        memcpy(this->_clientId, tempID.c_str(), tempID.length());
        //strcpy(this->_clientId,tempID.c_str());
        String Data = ("clientId" + this->_ClientId + 
                         "deviceName" + this->_DeviceName + 
                         "productKey" + this->_ProductKey + 
                         "timestamp" + (String)timestamp);
        byte tempPassWord[20];
        char tempSecret[this->_DeviceSecret.length()];
        char tempData[Data.length()];
        String tempName = (this->_DeviceName + "&" + this->_ProductKey);
        len = tempName.length();
        this->_username = (char * )malloc(len + 1);
        memset(this->_username, 0, tempName.length() + 1);
        memcpy(this->_username, tempName.c_str(), tempName.length());
        //strcpy(this->_username,tempName.c_str());

        memset(tempData, 0, Data.length() + 1);
        memcpy(tempData, Data.c_str(), Data.length());
        //strcpy(tempData,Data.c_str());
        memset(tempSecret, 0, this->_DeviceSecret.length() + 1);
        memcpy(tempSecret, this->_DeviceSecret.c_str(), this->_DeviceSecret.length());
        //strcpy(tempSecret,this->_DeviceSecret.c_str());
        MyHmac_Sha1.HMAC_SHA1((byte * )tempData,Data.length(),(byte * )tempSecret,this->_DeviceSecret.length(),tempPassWord);
        String tempPass = byteToHexStr(tempPassWord,sizeof(tempPassWord));
        if(this->_password == NULL){
            this->_password = (char *) malloc(tempPass.length() + 1);
        }
        memset(this->_password, 0, tempPass.length() + 1);
        memcpy(this->_password, tempPass.c_str(), tempPass.length());
        //strcpy(this->_password,tempPass.c_str());
    }else{
        
    }

}
