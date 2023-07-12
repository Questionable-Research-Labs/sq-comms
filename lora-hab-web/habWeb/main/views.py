import datetime

from django.conf import settings
from django.db.models import Q
from django.shortcuts import render

# Create your views here.
import json
from django.http import JsonResponse
from .mqtt import client as mqtt_client
from .models import Device, MqttMsg
from geojson import Point, Feature, FeatureCollection, dumps
import random
from math import sin, pi
import datetime





def map(request):
    # huts = []
    # for device in Device.objects.all():
    #     if device.rel_coords != None:
    #         coords = eval(device.rel_coords)
    #         p = Point(coords, properties={"name": f"{coords[0]}, {coords[1]}"})
    #         huts.append(Feature(p, geometry=p, properties=p['properties']))

    dat = MqttMsg.objects.filter(~Q(dists__isnull=True)).order_by("-timestamp")
    dat = dat[0]
    out = [x["est_distance"] * 2.8 for x in eval(eval(dat.dists))["distances"]]
    while True:
        if len(out) != 4:
            out.append(0)
        else:
            break

    return render(request, 'map.html',
                  {'data': out})


def dashboard(request):
    print("Dashboard")
    eva_objs = Device.objects.filter(type='P')
    print(eva_objs)
    act = Device.objects.filter(type='S')
    print(act)
    return render(request, "index.html", context={"eva": eva_objs, "act": act, "hab": Device.objects.get(dId=settings.HAB_ID), "count": Device.objects.all().count()})


def mk_habs_html(ping, st, rs, up, alarm):
    if alarm:
        return f"""<div style="background: #8a3421;padding: 17px;border-radius: 15px;width: 209.2px;">
        <div style="height: 1px;background: var(--bs-white);margin-bottom: 12px;"></div>
        <div class="d-sm-flex justify-content-sm-center align-items-sm-center"><img class="d-sm-flex justify-content-sm-start" src="http://127.0.0.1:8000/static/assets/img/hab.png" style="height: 69px;" />
            <p class="text-white justify-content-sm-end" style="font-size: 11px;padding-left: 8px;"><br />Last Ping: {ping.strf("%d-%m-%y")}<br />Status: {st} <br />RSSI: {rs}  <br />Uptime {up}<br /></p>
        </div>
    </div>"""
    else:
        return f"""<div style="background: #525252;padding: 17px;border-radius: 15px;width: 209.2px;">
            <div style="height: 1px;background: var(--bs-white);margin-bottom: 12px;"></div>
            <div class="d-sm-flex justify-content-sm-center align-items-sm-center"><img class="d-sm-flex justify-content-sm-start" src="http://127.0.0.1:8000/static/assets/img/hab.png" style="height: 69px;" />
                <p class="text-white justify-content-sm-end" style="font-size: 11px;padding-left: 8px;"><br />Last Ping: {ping.strf("%d-%m-%y")}<br />Status: {st} <br />RSSI: {rs}  <br />Uptime {up}<br /></p>
            </div>
        </div>"""


def mk_eva_html(ping1, st1, rs1, up1, ping2, st2, rs2, up2, alarm1, alarm2):
    if alarm1:
        return f"""<<div class="row" style="margin-top: 17px;">
        <div class="col-md-6" style="width: 40%;background: #8a3421;padding: 17px;border-width: 5px;border-radius: 15px;">
            <div style="height: 1px;background: var(--bs-white);margin-bottom: 12px;"></div>
            <div class="d-sm-flex justify-content-sm-start align-items-sm-center"><img class="d-sm-flex justify-content-sm-start" src="http://127.0.0.1:8000/static/assets/img/eva.png" style="height: 69px;" />
                <p class="text-white justify-content-sm-end" style="font-size: 11px;padding-left: 8px;"><br />Last Ping: {ping1} <br />Status: {st1} <br />RSSI: {rs1} <br />Uptime: {up1}<br /></p>
            </div>
        </div>
        <div class="col-md-6" style="width: 20%;"></div>
        <div class="col-md-6" style="width: 40%;background: #525252;padding: 17px;border-radius: 15px;">
            <div style="height: 1px;background: var(--bs-white);margin-bottom: 12px;"></div>
            <div class="d-sm-flex justify-content-sm-start align-items-sm-center"><img class="d-sm-flex justify-content-sm-start" src="http://127.0.0.1:8000/static/assets/img/eva.png" style="height: 69px;" />
                <p class="text-white justify-content-sm-end" style="font-size: 11px;padding-left: 8px;"><br />Last Ping: {ping2} <br />Status: {st2} <br />RSSI: {rs2} <br />Uptime: {up2}<br /></p>
            </div>
        </div>
    </div>"""
    if alarm2:
        return f"""<<div class="row" style="margin-top: 17px;">
        <div class="col-md-6" style="width: 40%;background: #525252;padding: 17px;border-width: 5px;border-radius: 15px;">
            <div style="height: 1px;background: var(--bs-white);margin-bottom: 12px;"></div>
            <div class="d-sm-flex justify-content-sm-start align-items-sm-center"><img class="d-sm-flex justify-content-sm-start" src="http://127.0.0.1:8000/static/assets/img/eva.png" style="height: 69px;" />
                <p class="text-white justify-content-sm-end" style="font-size: 11px;padding-left: 8px;"><br />Last Ping: {ping1} <br />Status: {st1} <br />RSSI: {rs1} <br />Uptime: {up1}<br /></p>
            </div>
        </div>
        <div class="col-md-6" style="width: 20%;"></div>
        <div class="col-md-6" style="width: 40%;background: #8a3421;padding: 17px;border-radius: 15px;">
            <div style="height: 1px;background: var(--bs-white);margin-bottom: 12px;"></div>
            <div class="d-sm-flex justify-content-sm-start align-items-sm-center"><img class="d-sm-flex justify-content-sm-start" src="http://127.0.0.1:8000/static/assets/img/eva.png" style="height: 69px;" />
                <p class="text-white justify-content-sm-end" style="font-size: 11px;padding-left: 8px;"><br />Last Ping: {ping2} <br />Status: {st2} <br />RSSI: {rs2} <br />Uptime: {up2}<br /></p>
            </div>
        </div>
    </div>"""
    else:
        return f"""<<div class="row" style="margin-top: 17px;">
                <div class="col-md-6" style="width: 40%;background: #525252;padding: 17px;border-width: 5px;border-radius: 15px;">
                    <div style="height: 1px;background: var(--bs-white);margin-bottom: 12px;"></div>
                    <div class="d-sm-flex justify-content-sm-start align-items-sm-center"><img class="d-sm-flex justify-content-sm-start" src="http://127.0.0.1:8000/static/assets/img/eva.png" style="height: 69px;" />
                        <p class="text-white justify-content-sm-end" style="font-size: 11px;padding-left: 8px;"><br />Last Ping: {ping1} <br />Status: {st1} <br />RSSI: {rs1} <br />Uptime: {up1}<br /></p>
                    </div>
                </div>
                <div class="col-md-6" style="width: 20%;"></div>
                <div class="col-md-6" style="width: 40%;background: #525252;padding: 17px;border-radius: 15px;">
                    <div style="height: 1px;background: var(--bs-white);margin-bottom: 12px;"></div>
                    <div class="d-sm-flex justify-content-sm-start align-items-sm-center"><img class="d-sm-flex justify-content-sm-start" src="http://127.0.0.1:8000/static/assets/img/eva.png" style="height: 69px;" />
                        <p class="text-white justify-content-sm-end" style="font-size: 11px;padding-left: 8px;"><br />Last Ping: {ping2} <br />Status: {st2} <br />RSSI: {rs2} <br />Uptime: {up2}<br /></p>
                    </div>
                </div>
            </div>"""


def mk_st_html(ping1, st1, rs1, up1, ping2, st2, rs2, up2, alarm1, alarm2):
    if alarm1:
        return f"""<div class="row"  style="margin-top: 17px;">
        <div class="col-md-6" style="width: 40%;background: #8a3421;padding: 17px;border-width: 5px;border-radius: 15px;">
            <div style="height: 1px;background: var(--bs-white);margin-bottom: 12px;"></div>
            <div class="d-sm-flex justify-content-sm-start align-items-sm-center"><img class="d-sm-flex justify-content-sm-start" src="http://127.0.0.1:8000/static/assets/img/node.png" style="height: 69px;" />
                <p class="text-white justify-content-sm-end" style="font-size: 11px;padding-left: 8px;"><br />Last Ping: {ping1} <br />Status: {st1} <br />RSSI: {rs1} <br />Uptime: {up1}<br /></p>
            </div>
        </div>
        <div class="col-md-6" style="width: 20%;"></div>
        <div class="col-md-6" style="width: 40%;background: #525252;padding: 17px;border-radius: 15px;">
            <div style="height: 1px;background: var(--bs-white);margin-bottom: 12px;"></div>
            <div class="d-sm-flex justify-content-sm-start align-items-sm-center"><img class="d-sm-flex justify-content-sm-start" src="http://127.0.0.1:8000/static/assets/img/node.png" style="height: 69px;" />
                <p class="text-white justify-content-sm-end" style="font-size: 11px;padding-left: 8px;"><br />Last Ping: {ping2} <br />Status: {st2} <br />RSSI: {rs2} <br />Uptime: {up2}<br /></p>
            </div>
        </div>
    </div>"""
    if alarm2:
        return f"""<div class="row"  style="margin-top: 17px;">
        <div class="col-md-6" style="width: 40%;background: #525252;padding: 17px;border-width: 5px;border-radius: 15px;">
            <div style="height: 1px;background: var(--bs-white);margin-bottom: 12px;"></div>
            <div class="d-sm-flex justify-content-sm-start align-items-sm-center"><img class="d-sm-flex justify-content-sm-start" src="http://127.0.0.1:8000/static/assets/img/node.png" style="height: 69px;" />
                <p class="text-white justify-content-sm-end" style="font-size: 11px;padding-left: 8px;"><br />Last Ping: {ping1} <br />Status: {st1} <br />RSSI: {rs1} <br />Uptime: {up1}<br /></p>
            </div>
        </div>
        <div class="col-md-6" style="width: 20%;"></div>
        <div class="col-md-6" style="width: 40%;background: #8a3421;padding: 17px;border-radius: 15px;">
            <div style="height: 1px;background: var(--bs-white);margin-bottom: 12px;"></div>
            <div class="d-sm-flex justify-content-sm-start align-items-sm-center"><img class="d-sm-flex justify-content-sm-start" src="http://127.0.0.1:8000/static/assets/img/node.png" style="height: 69px;" />
                <p class="text-white justify-content-sm-end" style="font-size: 11px;padding-left: 8px;"><br />Last Ping: {ping2} <br />Status: {st2} <br />RSSI: {rs2} <br />Uptime: {up2}<br /></p>
            </div>
        </div>
    </div>"""
    else:
        return f"""<div class="row"  style="margin-top: 17px;">
            <div class="col-md-6" style="width: 40%;background: #525252;padding: 17px;border-width: 5px;border-radius: 15px;">
                <div style="height: 1px;background: var(--bs-white);margin-bottom: 12px;"></div>
                <div class="d-sm-flex justify-content-sm-start align-items-sm-center"><img class="d-sm-flex justify-content-sm-start" src="http://127.0.0.1:8000/static/assets/img/node.png" style="height: 69px;" />
                    <p class="text-white justify-content-sm-end" style="font-size: 11px;padding-left: 8px;"><br />Last Ping: {ping1} <br />Status: {st1} <br />RSSI: {rs1} <br />Uptime: {up1}<br /></p>
                </div>
            </div>
            <div class="col-md-6" style="width: 20%;"></div>
            <div class="col-md-6" style="width: 40%;background: #525252;padding: 17px;border-radius: 15px;">
                <div style="height: 1px;background: var(--bs-white);margin-bottom: 12px;"></div>
                <div class="d-sm-flex justify-content-sm-start align-items-sm-center"><img class="d-sm-flex justify-content-sm-start" src="http://127.0.0.1:8000/static/assets/img/node.png" style="height: 69px;" />
                    <p class="text-white justify-content-sm-end" style="font-size: 11px;padding-left: 8px;"><br />Last Ping: {ping2} <br />Status: {st2} <br />RSSI: {rs2} <br />Uptime: {up2}<br /></p>
                </div>
            </div>
        </div>"""

def api(request):
    # request should be ajax and method should be GET.
    if request.method == "GET":
        devices = Device.objects.all()
        habs = devices.filter(type='H')
        evas = devices.filter(type='P')
        stations = devices.filter(type='S')

        # habs_html = [mk_habs_html(hab.last_ping, "ONLINE", hab.rssi, hab.uptime) for hab in habs]

         #evas_html = [mk_eva_html(evas[num].last_ping, "ONLINE", evas[num].rssi, evas[num].uptime, evas[num +1].last_ping, "ONLINE", evas[num +1].rssi, evas[num +1].uptime) for num in range(0, evas.count(), 2)
        noise = False
        evas_html = []

        for num in range(0, evas.count() - 1, 2):
            if evas[num].alert or evas[num + 1].alert:
                noise = True
            evas_html.append(mk_eva_html(evas[num].last_ping, "ONLINE", evas[num].rssi, evas[num].uptime, evas[num + 1].last_ping,
                        "ONLINE", evas[num + 1].rssi, evas[num + 1].uptime, evas[num].alert, evas[num + 1].alert))

        if evas.count() % 2 != 0:
            index = evas.count() - 1
            if evas[index].alert:
                noise = True
            evas_html.append(
                mk_st_html(evas[index].last_ping, "ONLINE", evas[index].rssi, evas[index].uptime, "null", "ONLINE",
                           "null", "null", evas[index].alert, False))


        try:
            stations_html = [mk_st_html(evas[num].last_ping, "ONLINE", evas[num].rssi, evas[num].uptime, evas[num +1].last_ping, "ONLINE", evas[num +1].rssi, evas[num +1].uptime, evas[num].alert, evas[num + 1].alert) for num in range(0, stations.count(), 2)]
        except IndexError:
            evas_html.append(mk_st_html(stations[-1].last_ping, "ONLINE", stations[-1].rssi, stations[-1].uptime, "null", "ONLINE", "null", "null", evas[-1].alert, False))

        return JsonResponse({"noise": noise, "num": devices.count(), "hab": {"ping": habs[0].last_ping, "status": "ONLINE", "rssi": habs[0].rssi, "uptime": habs[0].uptime, "alert":habs[0].alert}, "evas": evas_html, "stations": stations_html}, status=200)

    return JsonResponse({}, status=400)


def geo_api(request):
    huts = []
    query = request.GET
    for hut in Hut.objects.filter(name__icontains=query["search"])[:int(query["limit"])]:
        p = Point((hut.lon, hut.lat), properties={"popupContent":
                                                      f"""<h4>{hut.name}</h4>
                                                        <div class="container">
                                                            <div class="row">
                                                                <div class="col-md-6"><img src="{hut.Thumbnail}" width="100%"/></div>
                                                                <div class="col-md-6"><span><br /><span style="color: rgb(51, 51, 51);">{hut.introduction}</span><br /><br /></span><span><br /><strong><span style="color: rgb(51, 51, 51);">Category:</span></strong><span style="color: rgb(51, 51, 51);">{hut.Category}</span><br /><br /></span><a href="/detail/{hut.id}/0">More Details and Intentions</a></div>
                                                            </div>
                                                        </div>
                                                    </div>`,""",
                                                  'tag': "hut", "title": hut.name, "description": hut.introduction,
                                                  "image": hut.Thumbnail})
        huts.append(Feature(p, geometry=p, properties=p['properties']))

    camps = []
    for camp in Campsite.objects.filter(name__icontains=query["search"])[:int(query["limit"])]:
        p = Point((camp.lon, camp.lat), properties={"popupContent":
                                                      f"""<h4>{camp.name}</h4>
                                                            <div class="container">
                                                                <div class="row">
                                                                    <div class="col-md-6"><img src="{camp.Thumbnail}" width="100%"/></div>
                                                                    <div class="col-md-6"><span><br /><span style="color: rgb(51, 51, 51);">{camp.introduction}</span><br /><br /></span><span><br /><strong><span style="color: rgb(51, 51, 51);">Category:</span></strong><span style="color: rgb(51, 51, 51);">{camp.Category}</span><br /><br /></span><a href="/detail/{camp.id}/1">More Details and Intentions</a></div>
                                                                </div>
                                                            </div>
                                                        </div>`,""",
                                                  'tag': "camp", "title": camp.name, "description": camp.introduction,
                                                  "image": camp.Thumbnail})
        camps.append(Feature(p, geometry=p, properties=p['properties']))
    return HttpResponse(dumps(FeatureCollection(huts + camps)), content_type="application/json")
