import datetime

from django.db import models


class MqttMsg(models.Model):
    # required feilds
    type = models.CharField(max_length=6, null=False, blank=False)
    fromID = models.CharField(max_length=50, null=False, blank=False)
    timestamp = models.DateTimeField(auto_now_add=True)

    # optinal
    msg = models.CharField(max_length=1024, null=True, blank=True)
    sensor_data = models.CharField(max_length=5000, null=True, blank=True)


class Device(models.Model):
    dId = models.CharField(max_length=50, unique=True)
    last_msg = models.CharField(max_length=50, null=True, blank=True)
    last_sensor_data = models.CharField(max_length=5000, null=True, blank=True)
    alert = models.BooleanField(default=False)
    last_ping = models.DateTimeField(default=datetime.datetime.today())
    rssi = models.FloatField(default=-90)
    uptime = models.FloatField(default=0)
    type = models.CharField(choices=[('H', 'Hab'), ('S', 'Station'), ('P', 'Person')], default='P',
                            max_length=1)
