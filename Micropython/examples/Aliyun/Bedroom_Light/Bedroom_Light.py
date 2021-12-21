from DFRobot_Iot import DFRobot_Iot
from umqttsimple import MQTTClient
from machine import Pin
import machine
import time
import network
import json

WIFI_SSID = 'WIFI_SSID'
WIFI_PASSWORD = 'WIFI_PASSWORD'

ALIYUN_SERVER = ".iot-as-mqtt.cn-shanghai.aliyuncs.com"
ALIYUN_PORT = 1883
DeviceName = "you device Name"
ClientId = "12345"
ProductKey = "you product key"
DeviceSecret = "you device secret"

subTopic = 'you subscribe Topic'
led = Pin(2,Pin.OUT)

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
  if topic == str.encode(subTopic):
    temp = json.loads(msg)
    status = temp.get("params").get("LightStatus")
    if status == 1:
      print("ON Light")
      led.value(1)
    elif status == 0:
      print("OFF Light")
      led.value(0)
    else:
      print(status)
  
def restart():
  time.sleep(10)
  machine.reset()
  
connectWIFI()
myIot = DFRobot_Iot(ALIYUN_SERVER, DeviceName, ClientId, ProductKey, DeviceSecret)
client = MQTTClient(myIot.client_id, myIot.mqttserver, port = ALIYUN_PORT, user = myIot.username, password = myIot.password, keepalive = 120)
client.set_callback(sub_cb)
client.connect()
client.subscribe(subTopic)

while True:
  try:
    client.check_msg()
  except OSError as e:
    restart()


