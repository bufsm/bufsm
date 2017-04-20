from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^linha/(?P<idLinha>[0-9]+)/get$', views.getLinha),
    url(r'^linha/(?P<idLinha>[0-9]+)/write/(?P<token>.+)/(?P<lat>.+)/(?P<lng>.+)$', views.writeLinha),
]
