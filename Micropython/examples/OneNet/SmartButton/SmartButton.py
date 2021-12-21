from DFRobot_Iot import DFRobot_Iot
from umqttsimple import MQTTClient
import machine
import time
import network

WIFI_SSID = 'WIFI_SSID'
WIFI_PASSWORD = 'WIFI_PASSWORD'

ONENET_SERVER = "mqtt.heclouds.com"
ONENET_PORT = 6002
ProductID = "you Product ID"
DeviceId = "you Device Id"
ApiKey = "you ApiKey"

pubTopic = 'SwitchStatus'
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
myIot = DFRobot_Iot(ONENET_SERVER, ProductID, DeviceId, ApiKey)
client = MQTTClient(myIot.client_id, myIot.mqttserver, port = ONENET_PORT, user = myIot.username, password = myIot.password)
client.set_callback(sub_cb)
client.connect()
SwitchStatus = True
while True:
  try:
    client.check_msg()
    if button.value() == 1:
      if SwitchStatus is True:
        SwitchStatus = False
        client.publish(pubTopic,"ON");
      else:
        SwitchStatus = True
        client.publish(pubTopic,"OFF");
      time.sleep(1)
  except OSError as e:
    restart()



