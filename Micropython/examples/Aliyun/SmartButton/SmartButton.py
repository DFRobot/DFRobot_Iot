from DFRobot_Iot import DFRobot_Iot
from umqttsimple import MQTTClient
from machine import Pin
import machine
import time
import network

WIFI_SSID = 'WIFI_SSID'
WIFI_PASSWORD = 'WIFI_PASSWORD'

ALIYUN_SERVER = ".iot-as-mqtt.cn-shanghai.aliyuncs.com"
ALIYUN_PORT = 1883
DeviceName = "you device Name"
ClientId = "12345"
ProductKey = "you product key"
DeviceSecret = "you device secret"

pubTopic = 'you publish Topic'
button = Pin(27,Pin.IN)

def connectWIFI():
  station = network.WLAN(network.STA_IF)
  station.active(True)
  station.connect(WIFI_SSID,WIFI_PASSWORD)
  while station.isconnected() == False:
    pass
  print('Connection successful')
  print(station.ifconfig())
  
def sub_cb(topic,msg):
  print((topic,msg))
  
def restart():
  time.sleep(10)
  machine.reset()
  
connectWIFI()
myIot = DFRobot_Iot(ALIYUN_SERVER, DeviceName, ClientId, ProductKey, DeviceSecret)
client = MQTTClient(myIot.client_id, myIot.mqttserver, port = ALIYUN_PORT, user = myIot.username, password = myIot.password, keepalive = 120)
client.set_callback(sub_cb)
client.connect()
SwitchStatus = True
while True:
  try:
    client.check_msg()
    if button.value() == 1:
      if SwitchStatus is True:
        SwitchStatus = False
        msg = '{"id":'+ClientId+',"params":{"ButtonStatus":'+str(1)+'},"method":"thing.event.property.post"}'
        client.publish(pubTopic,msg,qos=1)
        print("Publish ON")
      else:
        SwitchStatus = True
        msg = '{"id":'+ClientId+',"params":{"ButtonStatus":'+str(0)+'},"method":"thing.event.property.post"}'
        client.publish(pubTopic,msg,qos=1)
        print("Publish OFF")
      time.sleep(1)
  except OSError as e:
    restart()




