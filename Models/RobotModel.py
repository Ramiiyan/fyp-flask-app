
class RoboticArm:
    def __init__(self, micro_c, com_module, dof, dof_row_obj, wifi):

        self.micro_c = micro_c
        self.com_module = com_module
        self.dof = dof
        self.dof_row_obj = dof_row_obj
        self.wifi = wifi

    @classmethod
    def json_to_obj(cls, robotic_arm_json):
        return cls(**robotic_arm_json)

    def __repr__(self):
        return f'<RA model {self.micro_c}>'


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
        return f'<ServoRange model {self.min_range}>'


class Wifi:
    def __init__(self, username, password):
        self.username = username
        self.password = password

    @classmethod
    def json_to_obj(cls, wifi_json):
        return cls(**wifi_json)

    def __repr__(self):
        return f'<Wifi model {self.username}>'
