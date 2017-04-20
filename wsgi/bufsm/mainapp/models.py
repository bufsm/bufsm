from django.db import models

class BusPosition(models.Model):
    lat = models.FloatField()
    lng = models.FloatField()

    timestamp = models.DateTimeField(auto_now=True)

class Linha(models.Model):
    id = models.IntegerField(primary_key=True)

    lastKnownPosition = models.ForeignKey(BusPosition)
