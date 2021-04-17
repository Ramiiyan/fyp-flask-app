
class RoboticArm:
    def __init__(self, micro_c, com_module, dof, dof_row_obj, wifi, mqtt_default, mqtt_config):

        self.micro_c = micro_c
        self.com_module = com_module
        self.dof = dof
        self.dof_row_obj = dof_row_obj
        self.wifi = wifi
        self.mqtt_default = mqtt_default
        self.mqtt_config = mqtt_config

    @classmethod
    def json_to_obj(cls, robotic_arm_json):
        return cls(**robotic_arm_json)

    def __repr__(self):
        return f'<RA model {self.micro_c}>'


# dof_row_obj
class Servo:
    def __init__(self, selected_type, servo_range):

        self.selected_type = selected_type
        self.servo_range = servo_range

    @classmethod
    def json_to_obj(cls, servo_json):
        return cls(**servo_json)

    def __repr__(self):
        return f'<Servo model {self.selected_type}>'


class ServoRange:
    def __init__(self, min_range, max_range):
        self.min_range = min_range
        self.max_range = max_range

    @classmethod
    def json_to_obj(cls, servo_range_json):
        return cls(**servo_range_json)

    def __repr__(self):
        # "{ {0, 180}, {0, 180},{0, 180},{0, 180} }"
        min_max = '{ '
        for i in range(len(self.min_range)):
            min_max += '{'+str(self.min_range[i])+', '+str(self.max_range[i])+'}'
            if i != len(self.min_range)-1:
                min_max += ', '
        min_max += ' }'

        return f'{min_max}'


class Wifi:
    def __init__(self, username, password):
        self.username = username
        self.password = password

    @classmethod
    def json_to_obj(cls, wifi_json):
        return cls(**wifi_json)

    def __repr__(self):
        return f'<Wifi model {self.username}>'


class MqttConfig:
    def __init__(self, host, port, username, password, pub_topic, sub_topic):
        self.username = f'"{username}"'
        self.password = f'"{password}"'
        self.host = f'"{host}"'
        self.port = port
        self.pub_topic = f'"{pub_topic}"'
        self.sub_topic = f'"{sub_topic}"'

    @classmethod
    def json_to_obj(cls, mqtt_json):
        return cls(**mqtt_json)

    def __repr__(self):
        return f'<Mqtt model {self.username}>'
