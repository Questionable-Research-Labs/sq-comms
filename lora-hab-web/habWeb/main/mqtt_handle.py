from .models import MqttMsg


def Msg(msg):
    if msg.topic == "ALERT":
        payload = eval(msg.payload)
        return True
