from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^$', views.test),
    # url(r'^sair$', views.sair),
    # url(r'^home$', views.homeUsuario),
    # url(r'^usuario/update$', views.atualizaUsuario),
    # url(r'^usuario/verifica/(?P<username>.+)$', views.verificaUsuario),
]
