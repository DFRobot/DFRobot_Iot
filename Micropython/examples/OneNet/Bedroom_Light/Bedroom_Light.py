from DFRobot_Iot import DFRobot_Iot
from simple import MQTTClient
from machine import Pin
import machine
import time
import network

WIFI_SSID = 'WIFI_SSID'
WIFI_PASSWORD = 'WIFI_PASSWORD'

ONENET_SERVER = "mqtt.heclouds.com"
ONENET_PORT = 6002
ProductID = "you ProductID"
DeviceId = "you DeviceId"
ApiKey = "you ApiKey"

subTopic = 'SwitchStatus'
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
  if topic == b'SwitchStatus' and msg == b'OFF':
    print('OFF')
    led.value(0)
  elif topic == b'SwitchStatus' and msg == b'ON':
    print('ON')
    led.value(1)
  else:
    None
  
def restart():
  time.sleep(10)
  machine.reset()
  
connectWIFI()
myIot = DFRobot_Iot(ONENET_SERVER, ProductID, DeviceId, ApiKey)
client = MQTTClient(myIot.client_id, myIot.mqttserver, port = ONENET_PORT, user = myIot.username, password = myIot.password)
client.set_callback(sub_cb)
client.connect()
client.subscribe(subTopic)

while True:
  try:
    client.check_msg()
  except OSError as e:
    restart()


