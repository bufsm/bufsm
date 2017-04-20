# from django.shortcuts import render
# from django.template import RequestContext
# from django.contrib.auth import login, authenticate, logout
# from django.contrib.auth.models import User, Group
from django.http import HttpResponseRedirect, HttpResponse
# from django.core import serializers
from .models import Linha, BusPosition
import json

def linha(request, idLinha):

    try:
        linha = Linha.objects.get(id=idLinha)
    except Exception as e:
        return HttpResponse(json.dumps({"error": str(e)}), content_type='application/json')

    lastPosition = linha.lastKnownPosition

    obj = {"lat": lastPosition.lat, "lng": lastPosition.lng,
            "timestamp": lastPosition.timestamp.isoformat()}

    return HttpResponse(json.dumps(obj), content_type='application/json')


def test(request):
    return HttpResponse(json.dumps({"teste": "Funcionando :D"}), content_type='application/json')
