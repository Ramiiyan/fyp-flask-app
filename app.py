import eventlet
from flask import Flask, render_template
import json
from flask_mqtt import Mqtt
from engineio.payload import Payload
from flask_socketio import SocketIO, emit
from flask_cors import CORS


eventlet.monkey_patch()
app = Flask(__name__)

app.config['SECRET'] = 'mysecretkeydkjasndjkasndkjoejigtoinfijsdfubyievyibdw'
app.config['TEMPLATES_AUTO_RELOAD'] = True
app.config['MQTT_BROKER_URL'] = 'mqtt.iot.ideamart.io'
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_USERNAME'] = 'XXXXXXXXXXXXXXXXXXXXXXXXX'
app.config['MQTT_PASSWORD'] = 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX'
app.config['MQTT_KEEPALIVE'] = 5
app.config['MQTT_TLS_ENABLED'] = False
# app.config['MQTT_LAST_WILL_TOPIC'] = 'tester2/tesingdev2/v3/common'
# app.config['MQTT_LAST_WILL_MESSAGE'] = 'bye'
app.config['MQTT_LAST_WILL_QOS'] = 2

# app.config.from_pyfile('mqttConfiguration.cfg')
# CORS(app, resources={r'/*': {'origins': '*'}})
mqtt = Mqtt(app)


Payload.max_decode_packets = 50
socketIo = SocketIO(app, cors_allowed_origins="*", engineio_logger=True, logger=True)


# # define PUB_TOPIC           "tester2/tesingdev2/v3/common"
# # define SUB_TOPIC           "sub/1119/tester2/tesingdev2/v3/pub"

pubTopicMqtt = "tester2/tesingdev2/v3/common"   # Subscribe topic for server
subTopicMqtt = "sub/1119/tester2/tesingdev2/v3/pub"  # Publish topic for server

# @app.route('/')
# def index():
#     # return render_template('Test.html')
#     return 'hello world'


@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    # mqtt.subscribe('#')
    mqtt.subscribe('tester2/tesingdev2/v3/common')
    print("mqtt Connected.")
    # socketIo.emit('mqtt_message', 'hello')


@socketIo.on('connect_socket')
def handle_socket_connection():
    print('Client is connected.')
    emit('mqtt_message', data='connected')
    # socketIo.emit('initiate_socket', 'Flask Server: Socket connection request sent.')
    # socketIo.sleep(0)
#
#
# # @socketIo.on('initiate_socket')
# # def handle_socket_message(message):
# #     print("Message from client:" + message)


@socketIo.on('on_publish')
def handle_publish(json_str):
    print("Receiving Data...")
    data = json.loads(json_str)
    print("Received data from user : " + data)    # From Client to Server
#
#     # socketIo.sleep(0)
#     # mqtt.publish(subTopic, data['message'])   # Publishing user Data to Arduino


@socketIo.on('subscribe')
def handle_subscribe(json_str):
    data = json.loads(json_str)
    print("RECEIVED DATA subscribe: " + data)
    # mqtt.subscribe(pubTopicMqtt, data['qos'])


@socketIo.on('unsubscribe_all')
def handle_unsubscribe_all():
    mqtt.unsubscribe_all()
    print('unsubscribed.')


@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
    data = dict(
        topic=message.topic,
        payload=message.payload.decode()
    )
    print("my MSG : " + message.payload.decode())
    # emit a mqtt_message event to the socket containing the message data
    # if data['topic'] == pubTopicMqtt:
    emit('mqtt_message', data=data)
    # socketIo.sleep(0)
    # elif data['topic'] == subTopicMqtt:
    #     socketIo.emit('mqtt_message', data=data)
    # else:
    #     print("unauthorized data:" + data[''])


@mqtt.on_log()
def handle_logging(client, userdata, level, buf):
    print(level, buf)


if __name__ == '__main__':
    socketIo.run(app=app, debug=True)
