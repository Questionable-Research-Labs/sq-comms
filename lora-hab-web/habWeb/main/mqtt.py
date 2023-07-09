from django.conf import settings
import paho.mqtt.client as mqtt
from .models import MqttMsg


def on_connect(mqtt_client, userdata, flags, rc):
   if rc == 0:
       print('Connected successfully')
       mqtt_client.subscribe('#')
   else:
       print('Bad connection. Code:', rc)


def on_message(mqtt_client, userdata, msg):
    if msg.topic == "ALERT":
        payload = eval(msg.payload)
        obj = MqttMsg()
        obj.type = msg.topic
        obj.fromID = payload["from"]
        obj.msg = payload["msg"]
        obj.device_type = payload["class"]
        obj.save()
        print(f'Received message on topic: {msg.topic} It has been processed.')

    elif msg.topic == "SENSOR":
        payload = eval(msg.payload)
        obj = MqttMsg()
        obj.type = msg.topic
        obj.fromID = payload["from"]
        obj.sensor_data = payload["readings"]
        obj.save()
        print(f'Received message on topic: {msg.topic} It has been processed.')

    else:
        print(f'Received message on topic: {msg.topic} It has been been murdered.')


client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(settings.MQTT_USER, settings.MQTT_PASSWORD)
client.connect(
    host=settings.MQTT_SERVER,
    port=settings.MQTT_PORT,
    keepalive=settings.MQTT_KEEPALIVE
)
