from django.conf import settings
from django.db.models import Q
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
    eva_objs = Device.objects.filter(type='P')
    act = Device.objects.filter(type='S')
    return render(request, "dashboard.html", context={"eva": eva_objs, "act": act, "hab": Device.objects.get(dId=settings.HAB_ID), "count": Device.objects.all().count()})


# def getChartData(request):
#     # request should be ajax and method should be GET.
#     if request.method == "GET":
#         # get the nick name from the client side.
#         mid = request.GET.get("mid")
#         # check for the nick name in the database.
#         if Game.objects.filter(game_id=mid).exists():
#             # if match found return timing data
#             game = Game.objects.get(game_id=mid)
#             x, y = "x", "y"
#             nums = eval(game.round_numbers)
#             p1 = [{x: i[0], y: i[1]} for i in enumerate(eval(game.p1_timing))]
#             p2 = [{x: i[0], y: i[1]} for i in enumerate(eval(game.p2_timing))]
#             exp = [{x: n[0], y: 0.06 + n[1] * 1.74 * 0.001} for n in enumerate(nums)]
#             return JsonResponse({"p1": p1, "p2": p2, "exp": exp, "p1n": game.player1, "p2n": game.player2}, status=200)
#         else:
#             # if match not found
#             return JsonResponse({}, status=404)
#
#     return JsonResponse({}, status=400)

