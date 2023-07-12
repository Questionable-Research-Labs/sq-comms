import datetime

from django.conf import settings
import paho.mqtt.client as mqtt
from .models import MqttMsg, Device

true = True
false = False

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
        obj.save()

        device = Device.objects.get_or_create(dId=obj.fromID)[0]
        device.last_msg = obj.msg
        device.last_ping = datetime.datetime.now()

        device.alert = payload["active"]

        device.save()

        print(f'Received message on topic: {msg.topic} It has been processed.')

    elif msg.topic == "SENSOR":
        null = 0
        payload = eval(msg.payload)
        obj = MqttMsg()
        obj.type = msg.topic
        obj.fromID = payload["from"]
        obj.sensor_data = payload["readings"]
        obj.save()

        device = Device.objects.get_or_create(dId=obj.fromID)[0]
        device.last_sensor_data = obj.sensor_data
        device.last_ping = datetime.datetime.now()

        device.save()

        print(f'Received message on topic: {msg.topic} It has been processed.')
    elif msg.topic == "PING":
        payload = eval(msg.payload)

        device = Device.objects.get_or_create(dId=payload["from"])[0]
        # device.rssi = payload["rssi"]
        try:
            device.uptime = payload["uptime"]
        except KeyError:
            device.uptime = payload["5ptime"]
        device.last_ping = datetime.datetime.now()

        device.save()

        print(f'Received message on topic: {msg.topic} It has been processed.')
    elif msg.topic == "DISTANCES":
        exp = {"distances": [{"from": "", "est_distance": 5.38993728e8}], "alerted_chip": ""}
        payload = eval(msg.payload)
        obj = MqttMsg()
        obj.type = msg.topic
        obj.fromID = payload["alerted_chip"]
        obj.dists = msg.payload
        obj.save()

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
