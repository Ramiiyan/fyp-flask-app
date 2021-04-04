
from flask import Flask, render_template,request
from flask.json import jsonify
from engineio.payload import Payload
from flask_socketio import SocketIO, emit
from flask_mqtt import Mqtt
from flask_cors import CORS


app = Flask(__name__)
CORS(app)

app.config['SECRET'] = 'mysecretkeydkjasndjkasndkjoejigtoinfijsdfubyievyibdw'
app.config['MQTT_BROKER_URL'] = 'mqtt.iot.ideamart.io'
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_USERNAME'] = 'tester2-tesingdev2-v3_3220'
app.config['MQTT_PASSWORD'] = '1572435365_3220'
app.config['MQTT_KEEPALIVE'] = 5

mqtt = Mqtt(app)

Payload.max_decode_packets = 50
socketio = SocketIO(app, cors_allowed_origins="*")

pubTopicMqtt = "tester2/tesingdev2/v3/common"   # Subscribe topic for server
subTopicMqtt = "sub/1119/tester2/tesingdev2/v3/pub"  # Publish topic for server

# @app.route('/')
# def index():
#     return render_template('test1.html')


@app.route('/rovoSpec', methods=['POST'])
def specification():
    print(request.get_json())

    return "this is flask reply!"


@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    # mqtt.subscribe('#')
    mqtt.subscribe('tester2/tesingdev2/v3/common')
    print("MQTT Connected.")


# @socketio.on('socket')
# def test_message(message):
#     print('connect response EVENT >>>>')
#     # emit('my response', {'data': message['data']}, broadcast=True)
#     print("connect response from client: " + message)


@socketio.on('connect')
def test_connect():
    emit('connect_socket', "I'm server, Socket Connected")
    print('IN CONNECT >>>>')


@socketio.on('my event')
def test_message(message):
    print('IN MY EVENT >>>>')
    print("received msg from client !!!:" + message['data'])
    mqtt.publish(subTopicMqtt, message['data'])
    # emit('my response', {'data': message})


@socketio.on('unSubscribe_all')
def test_disconnect():
    print('IN DISCONNECT >>>>')
    print('Client disconnected')
    mqtt.unsubscribe_all()


@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
    data = dict(
        topic=message.topic,
        payload=message.payload.decode()
    )
    print("my MSG : " + message.payload.decode())
    # emit a mqtt_message event to the socket containing the message data
    # if data['topic'] == pubTopicMqtt:
    socketio.emit('mqtt_message', data=data)
    # socketIo.sleep(0)
    # elif data['topic'] == subTopicMqtt:
    #     socketIo.emit('mqtt_message', data=data)
    # else:
    #     print("unauthorized data:" + data[''])


@mqtt.on_log()
def handle_logging(client, userdata, level, buf):
    print(level, buf)


if __name__ == '__main__':
    socketio.run(app)
