#!/usr/bin/env python
# coding: utf-8

# In[ ]:


import paho.mqtt.client as mqtt
client = mqtt.Client()
client.connect("teknik-fisika.or.id" ,1883, 60)

while True:
    payload = input("Pesan :")
    client.publish("IoT_Command", payload)

client.loop_forever()

