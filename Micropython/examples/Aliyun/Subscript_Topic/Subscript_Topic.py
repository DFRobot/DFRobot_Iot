from DFRobot_Iot import DFRobot_Iot
from umqttsimple import MQTTClient
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

subTopic = 'you subscribe Topic'

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
client.subscribe(subTopic)

while True:
  try:
    client.check_msg()
  except OSError as e:
    restart()



