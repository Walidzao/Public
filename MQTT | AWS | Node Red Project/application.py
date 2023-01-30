import time
from AWSIoTPythonSDK.MQTTLib import AWSIoTMQTTClient


def helloworld(self,params,packet):
	print('Received message from iot core')
	print('Topic: '+ packet.topic)
	print("Payload: ", (packet.payload))

#Connection to the AWS IoT Core with Root CA certificate and provisioning claim credentials (private key and certificates)

# For certificate based connection
myMQTTClient = AWSIoTMQTTClient("WalidClientid")
# For TLS mutual authentication
myMQTTClient.configureEndpoint("a292p0abjdlhld-ats.iot.us-east-1.amazonaws.com", 8883) #Provide your AWS IoT Core endpoint (Example: "abcdef12345-ats.iot.us-east-1.amazonaws.com")

myMQTTClient.configureCredentials("/home/pi/AWSIot/root-ca.pem", "/home/pi/AWSIot/private.pem.key", "/home/pi/AWSIot/certificate.pem.crt") #Set path for Root CA and provisioning claim credentials
myMQTTClient.configureOfflinePublishQueueing(-1)
myMQTTClient.configureDrainingFrequency(2)
myMQTTClient.configureConnectDisconnectTimeout(10)
myMQTTClient.configureMQTTOperationTimeout(5)

print("Initiating IoT Core Topic...")

myMQTTClient.connect()

with open('temp.txt') as f:
        while True:
            line = f.readline()
            time.sleep(.5)
            print("Sending Temperature: ", line)

            myMQTTClient.publish(
                topic="rasp/raspberry",
                QoS=1,
                payload='{"Temperature":"'+line+'"}')

myMQTTClient.publish(topic= 'rasp/raspberry',QoS=1, payload='{"Temperature":"30"}')
#Create keys and certificate by publishing a request in a MQTT topic

myMQTTClient.subscribe("rasp/raspberry", 1, helloworld)