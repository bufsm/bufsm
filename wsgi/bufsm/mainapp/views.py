# from django.shortcuts import render
# from django.template import RequestContext
# from django.contrib.auth import login, authenticate, logout
# from django.contrib.auth.models import User, Group
# from django.core import serializers
from django.http import HttpResponseRedirect, HttpResponse
from django.conf import settings
from .models import Linha, BusPosition
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
