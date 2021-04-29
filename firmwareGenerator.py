import os
import shutil
from jinja2 import Template, Environment, FileSystemLoader
import app
from Models.RobotModel import RoboticArm, Servo, ServoRange, Wifi

# selection the template
# src = 'firmwareTemplates/ServoReadHardwareTrial_ESP32_multiservo_SimModule' # change to app.py
dest = 'output_generated_firmware/firmware_v1.0/main'  # change to app.py

# Data structure of specification values and default values.
map_of_spec_val = {
    "servo_count_dof": 4,
    "Dpin_list": "{12, 26, 32, 14}",  # default
    "Apin_list": "{13, 27, 33, 25}",  # default
    "servo_range_list": "{ {0, 180}, {0, 180},{0, 180},{0, 180} }",
    "serial_setting": {
        "baud_rate": 115200,
        "sim_module": 9600,
        "wifi_module": 9600,   # For External
        "sim_module_Txd": 16,  # IN ESP32 RX pin
        "sim_module_Rxd": 17,  # IN ESP32 TX pin
    },
    # Default mqtt credentials
    "mqtt_setting": {
        "mqtt_host": '"mqtt.iot.ideamart.io"',
        "mqtt_port": 1883,
        "mqtt_username": '"tester2-tesingdev2-v3_3220"',
        "mqtt_password": '"1572435365_3220"',
        "pub_topic": '"tester2/tesingdev2/v3/common"',
        "sub_topic": '"sub/1119/tester2/tesingdev2/v3/pub"',
    },
    # Only for Cellular communication
    "sim_network_setting": {
        "network_mode": 13,  # 13-GSM (default) , 38-Nb-IoT
        "gprs_setting": {
            "APN_mode": "dialogbb",  # GSM-dialogbb (default) , NB-nbiot
            "username": "",
            "password": ""
        }
    },
    # Only for Wifi communication module
    "wifi_setting": {
        "username": '"wifi_username"',
        "password": '"wifi_password"',
    }
}


def select_template(template_src, generate_dest):
    print('Template_Src:'+template_src)
    print('Template_Dest:'+generate_dest)
    f_name = template_src.split("/")
    main_file_name = f_name[1]+'.ino'
    print('Template_Dest_File_Name:'+main_file_name)
    src_files = os.listdir(template_src)
    for file_name in src_files:
        full_file_name = os.path.join(template_src, file_name)
        if os.path.isfile(full_file_name):
            shutil.copy(full_file_name, generate_dest)
    if os.path.isfile(generate_dest + '/main.ino'):
        os.remove(r''+generate_dest+'/'+main_file_name)
    else:
        os.rename(r''+generate_dest+'/'+main_file_name, r'' +
                  generate_dest + '/main.ino')


# map_line_no = {"//define DOF": 0, "// Wroom ESP32 Servo-motor Control Digital Pins ": 0}

def generate_firmware():
    print(map_of_spec_val["servo_count_dof"])
    print(map_of_spec_val["Dpin_list"])
    print(map_of_spec_val["Apin_list"])
    # load the firmware template
    loader = FileSystemLoader(dest)
    env = Environment(loader=loader)

    # header files of the firmware structure.
    template_headers = ["MainTemplate.h", "Com_Config.h"]
    # assign rendered specification to firmware template
    for template in template_headers:
        firmware_template_main = env.get_template(template)
        rendered_file = firmware_template_main.render(map_of_spec_val)

        # print(rendered_file)
        # write the firmware into destination file
        with open(dest + '/' + template, 'w+') as gen_firmware:
            gen_firmware.write(rendered_file)
