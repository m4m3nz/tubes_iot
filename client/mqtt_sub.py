#!/usr/bin/env python
# coding: utf-8

# In[2]:


from paho.mqtt import client as mqtt
from influxdb import InfluxDBClient
import datetime
import time
topics = ["IoT_Sensor/1","IoT_Sensor/2","IoT_Sensor/3","IoT_Sensor/4","IoT_Sensor/5",
"IoT_Sensor/6"]
assign=[]

def on_connect(client, userdata, flags, rc):
    if rc==0:
        print("Connected to MQTT teknik-fisika.or.id")
        print("Starting to receive message")
        for topic in topics :
            client.subscribe(topic)
    else:
        print("Bad connection")

def on_message_voltage(client, userdata, message):  
    d = datetime.datetime.now()
    unixtime = int(time.mktime(d.timetuple()))
    current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    json_body= [
    {
        "measurement": "Solar",
        "tags": {
        "host": "Sensor_1",
                },
        #"time": current_time,
        "fields": {
        "Voltage": float(message.payload.decode('utf-8')),
        }
    }
    ]
    influx_client.write_points(json_body,time_precision='u')
    print("Voltage: " + message.topic + " " + str(message.qos) + " " + str(message.payload.decode('utf-8')))
    
    
def on_message_status(client, userdata, message):
    d = datetime.datetime.now()
    unixtime = int(time.mktime(d.timetuple()))
    current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    json_body = [
    {
        "measurement": "Solar",
        "tags": {
        "host": "Sensor_1",

                },
        #"time": current_time,
        "fields": {
        "Percentage": float(message.payload.decode('utf-8')),
     }
        
    }
    ]
    
    influx_client.write_points(json_body, time_precision='u')
    print("Percentage: " + message.topic + " " + str(message.qos) + " " + str(message.payload.decode('utf-8')))

def on_message_current(client, userdata, message):
    d = datetime.datetime.now()
    unixtime = int(time.mktime(d.timetuple()))
    current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    json_body = [
    {
        "measurement": "Solar",
        "tags": {
        "host": "Sensor_1",

                },
        #"time": current_time,
        "fields": {
        "Current": float(message.payload.decode('utf-8')),
     }
        
    }
    ]
    influx_client.write_points(json_body, time_precision='u')
    print("Current: " + message.topic + " " + str(message.qos) + " " + str(message.payload.decode('utf-8')))


def on_message_power(client, userdata, message):  
    d = datetime.datetime.now()
    unixtime = int(time.mktime(d.timetuple()))
    current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    json_body= [
    {
        "measurement": "Solar",
        "tags": {
        "host": "Sensor_1",
                },
        #"time": current_time,
        "fields": {
        "Power": float(message.payload.decode('utf-8')),
        }
    }
    ]
    influx_client.write_points(json_body,time_precision='u')
    print("Power: " + message.topic + " " + str(message.qos) + " " + str(message.payload.decode('utf-8')))

def on_message_humidity(client, userdata, message):
    d = datetime.datetime.now()
    unixtime = int(time.mktime(d.timetuple()))
    current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    json_body = [
    {
        "measurement": "Solar",
        "tags": {
        "host": "Sensor_1",

                },
        #"time": current_time,
        "fields": {
        "Humidity": float(message.payload.decode('utf-8')),
     }
        
    }
    ]
    influx_client.write_points(json_body, time_precision='u')
    print("Humidity: " + message.topic + " " + str(message.qos) + " " + str(message.payload.decode('utf-8')))

def on_message_temperature(client, userdata, message):
    d = datetime.datetime.now()
    unixtime = int(time.mktime(d.timetuple()))
    current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    json_body = [
    {
        "measurement": "Solar",
        "tags": {
        "host": "Sensor_1",

                },
        #"time": current_time,
        "fields": {
        "Temperature": float(message.payload.decode('utf-8')),
     }
        
    }
    ]
    influx_client.write_points(json_body, time_precision='u')
    print("Temperature: " + message.topic + " " + str(message.qos) + " " + str(message.payload.decode('utf-8')))

def on_message(client, userdata, message):
    print(message.topic + " " + str(message.qos) + " " + str(message.payload))

mqttc = mqtt.Client()
message = [on_message_voltage,on_message_status,on_message_current, on_message_power, on_message_humidity, on_message_temperature]
influx_client = InfluxDBClient('localhost', 8086, database='mqtt_test3')

try :
    for i,j in zip(topics,message):
        mqttc.message_callback_add(i,j)
    mqttc.connect("teknik-fisika.or.id",1883,60)
    mqttc.on_connect = on_connect
    mqttc.loop_forever()

except KeyboardInterrupt:
    print("Exiting loop")




# In[ ]:




