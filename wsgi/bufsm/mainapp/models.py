from django.db import models

class BusPosition(models.Model):
    lat = models.FloatField()
    lng = models.FloatField()

    timestamp = models.DateTimeField(auto_now=True)

    def __str__(self):
        return self.timestamp.strftime('%d/%m/%y-%H:%M:%S %Z')

class Linha(models.Model):
    id = models.IntegerField(primary_key=True)

    lastKnownPosition = models.ForeignKey(BusPosition)

    def __str__(self):
        return "Linha " + str(self.id)

# class Test(models.Model): # Temporary model, just for test
#
#     index = models.IntegerField()
