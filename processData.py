#!/bin/python3

import os
import serial
import time
from time import strftime
import phao.mqtt.client as mqtt


host = "192.168.2.179"
port = 1883
topic = "/wasserstand/percentValue"

def openSerialConnection():
    s = serial.Serial('/dev/ttyUSB0', 9600)
    s.open()
    time.sleep(5)
    try:
        response = s.readline()
        print(response)
    except KeyboardInterrupt():
        s.close()
    return s

def sendToMQTT():
    connection = openSerialConnection()
    client = mqtt.Client()
    client.connect(host, port)
    client.publish(topic, connection.readline())
    time.delay(1)
    client.loop()

sendToMQTT()


