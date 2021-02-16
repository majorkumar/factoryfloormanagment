import paho.mqtt.client as mqtt
import time

def on_message(client, userdata, message):
	print("msg rcvd ", str(message.payload.decode("utf-8")))
	print("msg topic=",message.topic)
	print("msg qos=", message.qos)
	print("msg retain flag= ", message.retain)

borker_address = "192.168.11.111"

print("creating new instance ")
client = mqtt.Client("P1")
while(1):
	client.on_message=on_message
	client.connect("192.168.11.111")
	client.loop_start()
	print("subscribing to topic", "esp32")
	client.subscribe("esp32")

	time.sleep(4)
