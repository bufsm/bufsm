# from django.shortcuts import render
# from django.template import RequestContext
# from django.contrib.auth import login, authenticate, logout
# from django.contrib.auth.models import User, Group
# from django.core import serializers
from django.http import HttpResponseRedirect, HttpResponse
from django.conf import settings
from .models import Linha, BusPosition, Test
import os, json

def getLinha(request, idLinha):

    try:
        linha = Linha.objects.get(id=idLinha)
    except Exception as e:
        return HttpResponse(status=400)

    lastPosition = linha.lastKnownPosition

    obj = {"lat": lastPosition.lat, "lng": lastPosition.lng,
            "timestamp": lastPosition.timestamp.isoformat()}

    return HttpResponse(json.dumps(obj), content_type='application/json')

def writeLinha(request, idLinha, token, lat, lng):

    try:
        linha = Linha.objects.get(id=idLinha)
    except Exception as e:
        return HttpResponse(status=400)

    if token != settings.WRITE_TOKEN:
        return HttpResponse(status=403)

    position = BusPosition(lat=lat, lng=lng)
    position.save()

    if position.timestamp > linha.lastKnownPosition.timestamp:
        linha.lastKnownPosition = position
        linha.save()

    return HttpResponse(status=200)

def testLinha(request, idLinha):

    index = Test.objects.all().first()

    testPoints = [
        {"lat": -29.710173, "lng": -53.716594},
        {"lat": -29.705787, "lng": -53.715961},
        {"lat": -29.705293, "lng": -53.715718},
        {"lat": -29.705244, "lng": -53.716063},
        {"lat": -29.705722, "lng": -53.716054},
        {"lat": -29.709991, "lng": -53.716707},
        {"lat": -29.710737, "lng": -53.716693},
        {"lat": -29.711810, "lng": -53.716515},
        {"lat": -29.712242, "lng": -53.719246},
        {"lat": -29.715677, "lng": -53.718637},
        {"lat": -29.715267, "lng": -53.715848},
        {"lat": -29.716060, "lng": -53.715677},
        {"lat": -29.719159, "lng": -53.715061},
        {"lat": -29.719554, "lng": -53.717932},
        {"lat": -29.721895, "lng": -53.717508},
        {"lat": -29.722654, "lng": -53.723044},
        {"lat": -29.729176, "lng": -53.721712},
        {"lat": -29.728947, "lng": -53.719800},
        {"lat": -29.730092, "lng": -53.719642},
        {"lat": -29.728939, "lng": -53.719791},
        {"lat": -29.728435, "lng": -53.716317},
        {"lat": -29.725455, "lng": -53.716864},
        {"lat": -29.725059, "lng": -53.714266},
        {"lat": -29.724784, "lng": -53.712402},
        {"lat": -29.724646, "lng": -53.710987},
        {"lat": -29.722872, "lng": -53.711337},
        {"lat": -29.720057, "lng": -53.711882},
        {"lat": -29.720684, "lng": -53.716483},
        {"lat": -29.719519, "lng": -53.716707},
        {"lat": -29.719269, "lng": -53.715010},
        {"lat": -29.716008, "lng": -53.715652},
        {"lat": -29.715818, "lng": -53.715619},
        {"lat": -29.712755, "lng": -53.716135},
        {"lat": -29.710173, "lng": -53.716594}
    ]

    if index.index == len(testPoints):
        index.index = 0

    res = testPoints[index.index]

    index.index += 1
    index.save()

    return HttpResponse(json.dumps(res), content_type='application/json')
