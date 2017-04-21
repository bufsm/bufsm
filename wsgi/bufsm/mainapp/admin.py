from django.contrib import admin
from .models import BusPosition, Linha, Test

admin.site.register(BusPosition)
admin.site.register(Linha)
admin.site.register(Test)
