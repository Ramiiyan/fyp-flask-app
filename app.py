# import io
# from zipfile import ZipFile
# import os
import shutil
from flask import Flask, render_template, request, json, send_file
from flask.json import jsonify
from engineio.payload import Payload
from flask_socketio import SocketIO, emit
from flask_mqtt import Mqtt
from flask_cors import CORS

import firmwareGenerator

from Models.RobotModel import RoboticArm, Servo, ServoRange, Wifi, MqttConfig

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


@app.route('/rovoSpec', methods=['POST'])
def specification():
    # select firmware based on microController type & connectivity module
    src = 'firmwareTemplates/ServoReadHardwareTrial_ESP32_multiservo_SimModule'
    dest = 'output_generated_firmware/firmware_v1.0/main'
    firmwareGenerator.select_template(src, dest)

    print(f'Raw data: {request.get_json()}')
    robot_model = RoboticArm.json_to_obj(request.get_json())
    print(f'DOF: {robot_model.dof}')
    print("***************************")
    servo = Servo.json_to_obj(robot_model.dof_row_obj)
    print(f'servoType: {servo.selected_type}')
    print("***************************")
    servo_range = ServoRange.json_to_obj(servo.servo_range)
    print(f'servo_range_format: {servo_range}')
    print("***************************")
    wifi = Wifi.json_to_obj(robot_model.wifi)
    print(f'Wifi username:{wifi.username}')
    print("***************************")
    mqtt_config = MqttConfig.json_to_obj(robot_model.mqtt_config)

    # Fetch data to template
    firmwareGenerator.map_of_spec_val["servo_count_dof"] = robot_model.dof
    firmwareGenerator.map_of_spec_val["servo_range_list"] = servo_range
    if robot_model.mqtt_default:
        firmwareGenerator.map_of_spec_val["mqtt_setting"] = firmwareGenerator.map_of_spec_val["mqtt_setting"]
    else:
        firmwareGenerator.map_of_spec_val["mqtt_setting"]["mqtt_host"] = str(mqtt_config.host)
        firmwareGenerator.map_of_spec_val["mqtt_setting"]["mqtt_port"] = str(mqtt_config.port)
        firmwareGenerator.map_of_spec_val["mqtt_setting"]["mqtt_username"] = str(mqtt_config.username)
        firmwareGenerator.map_of_spec_val["mqtt_setting"]["mqtt_password"] = str(mqtt_config.password)
        firmwareGenerator.map_of_spec_val["mqtt_setting"]["pub_topic"] = str(mqtt_config.pub_topic)
        firmwareGenerator.map_of_spec_val["mqtt_setting"]["sub_topic"] = str(mqtt_config.sub_topic)
        print(firmwareGenerator.map_of_spec_val["mqtt_setting"])
    # print("robot:" + str(robot_model.dof))
    # print("dict val:" + str(firmwareGenerator.map_of_spec_val["servo_count_dof"]))

    # Generate firmware
    firmwareGenerator.generate_firmware()
    # Generate zip
    generated_package = "output_generated_firmware/firmware_v1.0"
    shutil.make_archive("output_zip/firmware_v1.0", 'zip', generated_package)
    f = open('output_zip/firmware_v1.0.zip', 'rb')
    return "hello"


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
