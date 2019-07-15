import json
import base64
import requests
import time
import paho.mqtt.client as mqtt


# Slack hook to post messages on bufsm behavior
SLACK_URL = "https://hooks.slack.com/services/KEY"


def publish(toSend):
    client_eclipse.publish("bufsm/p", payload=toSend, qos=2, retain=True)

    global error_bufsm
    if error_bufsm:
        client_eclipse.publish("bufsm/m", payload=None, qos=2, retain=True)  # Clear error
        client_eclipse.loop_write()
        error_bufsm = False

        data = {"text": "Run, Forrest, run! :busstop::bus::man-running:!"}
        requests.post(SLACK_URL, json.dumps(data))


def publish_error():
    print("Publishing Timeout")
    client_eclipse.publish("bufsm/m", payload="e", qos=2, retain=True)

    data = {"text": "Houston, :bus::interrobang::computer::man-running::boom::disappointed_relieved:!"}

    requests.post(SLACK_URL, json.dumps(data))


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code " + str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("bufsm/devices/bufsm_01/up")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    # print(msg.topic, ":", msg.payload)

    global last_message
    last_message = time.time()

    payload_str = msg.payload
    payload_json = json.loads(payload_str.decode('utf-8'))

    payload_encoded = payload_json["payload_raw"]
    payload = base64.b64decode(payload_encoded)

    # print("Received: ", payload)
    publish(payload)


def on_disconnect(client, userdata, rc):
    client.reconnect()


client_ttn = mqtt.Client()
client_ttn.on_connect = on_connect
client_ttn.on_message = on_message
client_ttn.on_disconnect = on_disconnect

client_eclipse = mqtt.Client()
client_eclipse.connect("mqtt.solveiot.com.br", 1883, 60)
client_eclipse.loop_start()
client_eclipse.on_disconnect = on_disconnect

client_ttn.username_pw_set("bufsm", "TTN Password")
client_ttn.connect("brazil.thethings.network", 1883, 60)

last_message = time.time()
error_bufsm = False

while True:
    client_ttn.loop()

    if not error_bufsm and time.time() - last_message > 30:
        publish_error()
        error_bufsm = True
