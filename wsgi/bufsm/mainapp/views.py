# from django.shortcuts import render
# from django.template import RequestContext
# from django.contrib.auth import login, authenticate, logout
# from django.contrib.auth.models import User, Group
from django.http import HttpResponseRedirect, HttpResponse
# from django.core import serializers

def test(request):
    return HttpResponse(json.dumps({"teste": "Funcionando :D"}), content_type='application/json')
