from DFRobot_Iot import DFRobot_Iot
from umqttsimple import MQTTClient
import machine
import time
import network

WIFI_SSID = 'hitest'
WIFI_PASSWORD = '12345678'

ONENET_SERVER = "mqtt.heclouds.com"
ONENET_PORT = 6002
ProductID = "you Product ID"
DeviceId = "you Device Id"
ApiKey = "you ApiKey"

pubTopic = 'testTopic'

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
myIot = DFRobot_Iot(ONENET_SERVER, ProductID, DeviceId, ApiKey)
client = MQTTClient(myIot.client_id, myIot.mqttserver, port = ONENET_PORT, user = myIot.username, password = myIot.password)
client.set_callback(sub_cb)
client.connect()
timeLimit = 10#10s
lastTime = 0

while True:
  try:
    client.check_msg()
    if(time.time() - lastTime) > timeLimit:
      print("publish hello")
      client.publish(pubTopic,'hello')
      lastTime = time.time()
  except OSError as e:
    restart()



