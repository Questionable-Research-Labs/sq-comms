from django.shortcuts import render

# Create your views here.
import json
from django.http import JsonResponse
from .mqtt import client as mqtt_client
from .models import Device


def publish_message(request):
    request_data = json.loads(request.body)
    rc, mid = mqtt_client.publish(request_data['topic'], request_data['msg'])
    return JsonResponse({'code': rc})


def dashboard(request):
    objs = Device.objects.all()
    data = []
    for device in objs:
        if device.alert:
            data.append([device.dId, True])
        else:
            data.append([device.dId, False])
    return render(request, "dashboard.html", context={"data": data, "hab": Device.objects.get(dId="Hab")})