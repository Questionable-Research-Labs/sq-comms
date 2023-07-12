# Generated by Django 4.2.3 on 2023-07-12 13:34

import datetime
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('main', '0003_device'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='mqttmsg',
            name='device_type',
        ),
        migrations.AddField(
            model_name='device',
            name='last_ping',
            field=models.DateTimeField(default=datetime.datetime(2023, 7, 12, 13, 34, 37, 29787)),
        ),
        migrations.AddField(
            model_name='device',
            name='rel_coords',
            field=models.CharField(blank=True, max_length=200, null=True),
        ),
        migrations.AddField(
            model_name='device',
            name='rssi',
            field=models.FloatField(default=-90),
        ),
        migrations.AddField(
            model_name='device',
            name='type',
            field=models.CharField(choices=[('H', 'Hab'), ('S', 'Station'), ('P', 'Person')], default='P', max_length=1),
        ),
        migrations.AddField(
            model_name='device',
            name='uptime',
            field=models.FloatField(default=0),
        ),
        migrations.AddField(
            model_name='mqttmsg',
            name='dists',
            field=models.CharField(blank=True, max_length=100000000, null=True),
        ),
    ]
