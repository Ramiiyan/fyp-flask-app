import os
import shutil
from jinja2 import Template, Environment, FileSystemLoader
import app
from Models.RobotModel import RoboticArm, Servo, ServoRange, Wifi

# select the template
# src = 'firmwareTemplates/ServoReadHardwareTrial_ESP32_multiservo_SimModule' # change to app.py
dest = 'output_generated_firmware/firmware_v1.0/main'  # change to app.py

map_of_spec_val = {
    "servo_count_dof": 4,
    "Dpin_list": "{12, 26, 32, 14}",  # default
    "Apin_list": "{13, 27, 33, 25}",  # default
    "servo_range_list": "{ {0, 180}, {0, 180},{0, 180},{0, 180} }",
    "serial_setting": {
        "baud_rate": 115200,
        "sim_module": 9600,
        "wifi_module": 9600,
        "sim_module_Txd": 16,  # IN ESP32 RX pin
        "sim_module_Rxd": 17,  # IN ESP32 TX pin
    },
    "sim_network_setting": {
        "network_mode": 13,  # 13-GSM (default) , 38-Nb-IoT
        "gprs_setting": {
            "APN_mode": "dialogbb",  # GSM-dialogbb (default) , NB-nbiot
            "username": "",
            "password": ""
        }
    },
    "mqtt_setting": {
        "mqtt_host": '"mqtt.iot.ideamart.io"',
        "mqtt_port": 1883,
        "mqtt_username": '"tester2-tesingdev2-v3_3220"',
        "mqtt_password": '"1572435365_3220"',
        "pub_topic": '"tester2/tesingdev2/v3/common"',
        "sub_topic": '"sub/1119/tester2/tesingdev2/v3/pub"',
    },
}


def select_template(template_src, generate_dest):
    src_files = os.listdir(template_src)
    for file_name in src_files:
        full_file_name = os.path.join(template_src, file_name)
        if os.path.isfile(full_file_name):
            shutil.copy(full_file_name, generate_dest)
    if os.path.isfile(generate_dest + '/main.ino'):
        os.remove(r''+generate_dest+'/ServoReadHardwareTrial_ESP32_multiservo_SimModule.ino')
    else:
        os.rename(r''+generate_dest+'/ServoReadHardwareTrial_ESP32_multiservo_SimModule.ino', r'' +
                  generate_dest + '/main.ino')


# map_line_no = {"//define DOF": 0, "// Wroom ESP32 Servo-motor Control Digital Pins ": 0}

def generate_firmware():
    print(map_of_spec_val["servo_count_dof"])
    loader = FileSystemLoader(dest)
    env = Environment(loader=loader)

    template_headers = ["MainTemplate.h", "GSM_Setting.h"]  # files that have changes.
    for template in template_headers:
        firmware_template_main = env.get_template(template)
        rendered_file = firmware_template_main.render(map_of_spec_val)

        # print(rendered_file)
        with open(dest + '/' + template, 'w+') as gen_firmware:
            gen_firmware.write(rendered_file)
