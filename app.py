from flask import Flask, render_template
import eventlet
import json
from flask_mqtt import Mqtt
from flask_socketio import SocketIO

eventlet.monkey_patch()
app = Flask(__name__, template_folder='./templates')

app.config['SECRET'] = '\xfa1\xce\xcf_>\xd52\x80\xae\x04`\xd9\x1c\xb8\x83`\\\xf4w\x17\xb1\xb6\xce'
app.config['TEMPLATES_AUTO_RELOAD'] = True
app.config['MQTT_BROKER_URL'] = 'mqtt.iot.ideamart.io'
app.config['MQTT_BROKER_PORT'] = 1883
app.config['MQTT_USERNAME'] = 'tester2-tesingdev2-v3_3220'
app.config['MQTT_PASSWORD'] = '1572435365_3220'
app.config['MQTT_KEEPALIVE'] = 5
app.config['MQTT_TLS_ENABLED'] = False


# app.config.from_pyfile('mqttConfiguration.cfg')

mqtt = Mqtt(app)
socketIo = SocketIO(app)


@app.route('/')
def index():
    return render_template('sample.html')


@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc):
    mqtt.subscribe('tester2/tesingdev2/v3/common')

# @socketIo.on('publish')
# def handle_publish(json_str):
#     data = json.loads(json_str)
#     mqtt.publish(data['topic'], data['message'])


# @socketIo.on('subscribe')
# def handle_subscribe(json_str):
#     data = json.loads(json_str)
#     mqtt.subscribe('tester2/tesingdev2/v3/common')


# @socketIo.on('unsubscribe_all')
# def handle_unsubscribe_all():
#     mqtt.unsubscribe_all()


@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
    data = dict(
        topic=message.topic,
        payload=message.payload.decode()
    )
    print("my MSG : ")
    print(message.payload.decode())
    print("\n")
    # emit a mqtt_message event to the socket containing the message data
    socketIo.emit('mqtt_message', data=data)


@mqtt.on_log()
def handle_logging(client, userdata, level, buf):
    print(level, buf)


# if __name__ == '__main__':
#     socketIo.run(app, host='localhost', port=5000, use_reloader=False, debug=True)
