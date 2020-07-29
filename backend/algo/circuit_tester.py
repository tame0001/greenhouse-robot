import paho.mqtt.client as mqtt
import time

class ControlCenter:

    def __init__(self):
        self.mqtt = mqtt.Client()
        self.mqtt.on_connect = self.on_connect
        self.mqtt.on_message = self.on_message
        self.mqtt.connect("192.168.1.191", 1883, 60)
        self.robot1 = PotCarrier(1)
        self.robot2 = PotCarrier(2)
        self.robot7 = PotCarrier(7)


    def on_connect(self, client, userdata, flags, rc):
        print("Connected with result code "+str(rc))
        self.mqtt.subscribe("irobot/feedback")

    def on_message(self, client, userdata, msg):
        in_payload = msg.payload.decode('ascii').split(':')
        robot_id = in_payload[0]
        status = in_payload[1]
        # print(f'Robot ID {robot_id} reports status {status}')
        if robot_id == '1':
            self.robot1.process_status(status, self.mqtt)
        elif robot_id == '2':
            self.robot2.process_status(status, self.mqtt)
        elif robot_id == '7':
            self.robot7.process_status(status, self.mqtt)

class PotCarrier:

    def __init__(self, robot_id):
        self.id = robot_id
        self.count_cmd = 0
        self.commands = ['r', 'r', 'e']
        self.cmd_pointer = 0
        self.cmd_topic = f'irobot/command/{self.id}'
        print(f'Create robot id {self.id}. Command will be sent to topic {self.cmd_topic}')

    def process_status(self, status, publisher):   

        # print(f'Robot {self.id} processes status {status}')

        if status == '0':
            self.count_cmd = self.count_cmd + 1
            print(f'Robot {self.id} completed {self.count_cmd} commands')
            self.cmd_pointer = self.cmd_pointer + 1
            if self.cmd_pointer >= len(self.commands):
                self.cmd_pointer = 0
            time.sleep(5)
            print('Sending new command {} to robot {}'.format(self.commands[self.cmd_pointer], self.id))
            publisher.publish(self.cmd_topic, self.commands[self.cmd_pointer])
            

server = ControlCenter()

server.mqtt.loop_forever()