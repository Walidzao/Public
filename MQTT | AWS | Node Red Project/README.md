#  MQTT|NodeRed Applciation
## This python app contained in this folder only contains part of the implementation. Since Node Red And AWS Are No code solutions, but requires permission and network configurations. 
#### Description: 

Using the IOT core solution from AWS we can collect and transmit data from sensors to the cloud for storage and analysis.

I installed Node-RED and MQTT on the Raspberry Pi, and for this specific project i used text based data instead of data comming from an electronic sensor liek a temperature or light sensor.
Node-RED is a visual programming tool that allows you to easily connect hardware devices and APIs, while MQTT is a lightweight messaging protocol that can be used to transmit data over a network.
Using Node-RED, I created a flow that reads data from the registered MQTT topic and republishes it again to AWS IOT core.
Next, I set up an AWS IoT Core account and created a 'thing' representing my Raspberry Pi device. I generated a certificate and private key for the device, and configured the Raspberry Pi to use these for secure communication with the AWS IoT Core service.

Using Node-RED, I created another flow that subscribes to the MQTT topic and sends the sensor data to AWS IoT Core using the MQTT protocol. In AWS IoT Core, I created a 'rule' that routes the incoming data to Amazon Dynamo DB for long-term storage.

Another code which connects to  MQTT using the aws-iot-device-sdk-python enables to connect to the AWS IOT core endpoint and publish and subscribe to the chosen topic that is aslo configured in node red. this way we have the infocomming from raspberry pi broadcasted to node red for a robust maniplulation using a no code solution and also sent to a cloud service using the generated TLS certifites from AWS. The data that is read from the topic we are subscribing to is stored in Dynamo DB using the rule with an attached policy from AWS.

Node-RED is a visual programming tool that allows you to easily connect hardware devices and APIs, and MQTT is a lightweight messaging protocol that can be used to transmit data over a network. I used Node-RED to create a flow that reads data from a text file on the Raspberry Pi and publishes it to an MQTT topic.

This graphic-based coding platform lets you quickly put together useful operations and graphic user interfaces (GUIs), whether you’re the freshest greenhorn or a seasoned veteran. You can use it to switch your internet-connected lights on schedule, or at the touch of a button through a web-app available to any device on your home network. You can use it as an information dashboard for the weather forecast,

