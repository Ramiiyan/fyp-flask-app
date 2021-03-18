from shutil import copyfile
from jinja2 import Template, Environment, FileSystemLoader

original_file_path = 'firmwareTemplates/ServoReadHardwareTrial_ESP32_multiservo_SimModule' \
                 '/ServoReadHardwareTrial_ESP32_multiservo_SimModule.ino'
generate_file_path = 'output_generated_firmware/main/main.ino'
# select the template from
copyfile(original_file_path, 'output_generated_firmware/main/main.ino')
# map_line_no = {"//define DOF": 0, "// Wroom ESP32 Servo-motor Control Digital Pins ": 0}
map_of_spec_val = {
    "servo_count_dof": 6,
    "Dpin_list": {12, 26, 32, 34, 18, 22},  # default
    "Apin_list": {13, 27, 33, 35, 19, 23},  # default
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
        "mqtt_host": "mqtt.iot.ideamart.io",
        "mqtt_port": 1883,

    },
}
# servo_def_l = 0
loader = FileSystemLoader("firmwareTemplates/ServoReadHardwareTrial_ESP32_multiservo_SimModule")
env = Environment(loader=loader)

firmware_template_main = env.get_template("ServoReadHardwareTrial_ESP32_multiservo_SimModule.ino")
rendered_file = firmware_template_main.render(map_of_spec_val)

print(rendered_file)
with open(generate_file_path, 'w+') as gen_firmware:
    gen_firmware.write(rendered_file)


# with open('output_generated_firmware/main/main.ino', 'r') as reader:
#     cur_line_no = 0
#     if reader.mode == "r":
#         template_content = reader.readlines()
#         for line in template_content:
#             # print(line)
#             cur_line_no += 1
#             # key = pattern | value = line number
#             for key, value in map_line_no.items():
#                 if key in line:
#                     print("found in Line :" + str(cur_line_no))
#                     map_line_no[key] = cur_line_no
#                     print(map_line_no)

# template_content[servo_def_l] = "#define SERVO_COUNT 4"
# with open('output_generated_firmware/main/main.ino', 'w+') as writer:
#     for (k1, v1), (k2, v2) in zip(map_line_no.items(), map_of_spec_val.items()):
#         content_line_no = map_line_no[k1]
#         content_sub = k2 + v2
#         print(content_sub)
#         template_content[content_line_no] = content_sub
#     writer.writelines(template_content)
    # print(line_no)


# with open('output_generated_firmware/main/main.ino', 'w+') as writer:
#     for editor in template_content:
#         writer.write(editor)
