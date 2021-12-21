import hmac
import uhashlib
import math
import time
def calculation_sign(ClientId, DeviceName, ProductKey, DeviceSecret,timestamp):
  data = "".join(("clientId",ClientId, "deviceName", DeviceName,
                  "productKey", ProductKey, "timestamp", timestamp))
  ret = hmac.new(str.encode(DeviceSecret),
                 str.encode(data),uhashlib.sha1).hexdigest()
  ret = bytes.decode(ret)
  return ret


class DFRobot_Iot:
  def __init__(self, Server, ProductIDorDeviceName, DeviceIDorClientId, ApiKeyorProductKey, DeviceSecret = None ,Port = 6002):
    if DeviceSecret is None:
      self.mqttserver = Server
      self.client_id  = DeviceIDorClientId
      self.username   = ProductIDorDeviceName
      self.password   = ApiKeyorProductKey
      self.port       = Port
    else:
      timestamp = str(49)
      self.mqttserver = ApiKeyorProductKey+Server
      self.client_id  = "".join((DeviceIDorClientId,
                                "|securemode=3",
                                ",signmethod=", "hmacsha1",
                                ",timestamp=", timestamp,
                                "|"))
      self.username   = "".join((ProductIDorDeviceName, "&", ApiKeyorProductKey))
      self.password   = calculation_sign(DeviceIDorClientId,ProductIDorDeviceName,ApiKeyorProductKey,DeviceSecret,timestamp)




