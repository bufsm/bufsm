from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^linha/(?P<idLinha>[0-9]+)$', views.getLinha),
    url(r'^linha/(?P<idLinha>[0-9]+)/(?P<token>.+)/(?P<lat>.+)/(?P<lng>.+)$', views.writeLinha),
]
