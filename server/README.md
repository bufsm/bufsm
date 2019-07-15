# Server

Scripts that ran on the server side.
 
##bufsm.py

Reads data from the TTN MQTT broker and write it to our own broker, so that
everyone can subscribe and get the info.
It also sends info about BUFSM behavior to a private Slack channel, so who is
running the system can receive notifications if something stop working right away.

## bufsm_location.py

Saves all the BUFSM published coordinates and its timestamp into a CSV file, 
so that it can be used for later analysis and research.


> PS: code writen using Extreme Go Horse methodology
