import random
import paho.mqtt.client as mqtt
import time

class RobotPosition:

    def __init__(self, x, y, direction=0):
        self.x = int(x)
        self.y = int(y)
        self.direction = int(direction)

    def change_position(self, x, y, direction=0):
        self.x = int(x)
        self.y = int(y)
        self.direction = int(direction)

    def interpret_dirction(self):

        ''' Direction
            1 = +x
            2 = +y
            3 = -x
            4 = -y '''

        if self.direction == 1:
            return '+x'
        elif self.direction == 2:
            return '+y'
        elif self.direction == 3:
            return '-x'
        elif self.direction == 4:
            return '-y'
    
    def update_by_command(self, command):

        if self.direction == 1:
            if command == 'R':
                self.x = self.x + 1
            elif command == 'Q':
                self.direction = 2
            elif command == 'E':
                self.direction = 4
            elif command == 'T':
                self.direction = 3
        
        elif self.direction == 2:
            if command == 'R':
                self.y = self.y + 1
            elif command == 'Q':
                self.direction = 3
            elif command == 'E':
                self.direction = 1
            elif command == 'T':
                self.direction = 4

        elif self.direction == 3:
            if command == 'R':
                self.x = self.x - 1
            elif command == 'Q':
                self.direction = 4
            elif command == 'E':
                self.direction = 2
            elif command == 'T':
                self.direction = 1

        elif self.direction == 4:
            if command == 'R':
                self.y = self.y - 1
            elif command == 'Q':
                self.direction = 1
            elif command == 'E':
                self.direction = 3
            elif command == 'T':
                self.direction = 2

class PotCarrier:

    def __init__(self, robot_id, x, y, direction, publisher):
        self.id = robot_id
        self.position = RobotPosition(x, y, direction)
        self.command_list = []
        self.cmd_topic = f'irobot/command/{self.id}'
        print(f'Create robot id {self.id}. Command will be sent to topic {self.cmd_topic}')
        self.publisher = publisher

    def print_current_pos(self):

        print('Robot ID {} is now at({}, {}) heading to {}'.format(
            self.id,
            self.position.x,
            self.position.y,
            self.position.interpret_dirction()
        ))

    def gen_movement_cmd(self, target_pos):

        x_distance = target_pos.x - self.position.x
        y_distance = target_pos.y - self.position.y
        direction = self.position.direction

        commands = []
        ''' Command
            Q = Turn Left
            E = Turn Right
            T = Turn Around
            R = Forward '''

        if abs(x_distance) > 0:

            if (direction == 1 and x_distance < 0) or (direction == 3 and x_distance > 0):
                commands.append('T')

            elif (direction == 2 and x_distance < 0) or (direction == 4 and x_distance > 0):
                commands.append('Q')

            elif (direction == 2 and x_distance > 0) or (direction == 4 and x_distance < 0):
                commands.append('E')

            for i in range(abs(x_distance)):
                commands.append('R')
        
            direction = 1 if x_distance > 0 else 3

        if abs(y_distance) > 0:

            if (direction == 2 and y_distance < 0) or (direction == 4 and y_distance > 0):
                commands.append('T')

            elif (direction == 1 and y_distance > 0) or (direction == 3 and y_distance < 0):
                commands.append('Q')

            elif (direction == 1 and y_distance < 0) or (direction == 3 and y_distance > 0):
                commands.append('E')

            for i in range(abs(y_distance)):
                commands.append('R')

            direction = 2 if y_distance > 0 else 4
        
        return commands

    def go_to_pos(self, target_pos):

        self.command_list = self.gen_movement_cmd(target_pos)
        current_command = self.command_list.pop(0)
        self.position.update_by_command(current_command)
        self.print_current_pos() 
        self.publisher.publish(self.cmd_topic, current_command.lower())
 

    def next_cmd(self):
        if len(self.command_list) > 0:
            current_command = self.command_list.pop(0)
            self.position.update_by_command(current_command)
            self.print_current_pos() 
            self.publisher.publish(self.cmd_topic, current_command.lower())
            
class ControlCenter:

    def __init__(self):
        self.mqtt = mqtt.Client()
        self.mqtt.on_connect = self.on_connect
        self.mqtt.on_message = self.on_message
        self.mqtt.connect("192.168.1.191", 1883, 60)
        self.mqtt.loop_start()
        # self.robot1 = PotCarrier(1)
        # self.robot2 = PotCarrier(2)
        self.robot7 = PotCarrier(7, 1, 1, 1, self.mqtt)


    def on_connect(self, client, userdata, flags, rc):
        print("Connected with result code "+str(rc))
        self.mqtt.subscribe("irobot/feedback")

    def on_message(self, client, userdata, msg):
        in_payload = msg.payload.decode('ascii').split(':')
        robot_id = in_payload[0]
        status = in_payload[1]
        # print(f'Robot ID {robot_id} reports status {status}')

        if status == '0':
            
            if robot_id == '1':
                self.robot1.next_cmd()
            elif robot_id == '2':
                self.robot2.next_cmd()
            elif robot_id == '7':
                self.robot7.next_cmd()


if __name__ == '__main__':

    # robot = PotCarrier(7, 1, 1, 1)
    # robot.print_current_pos()
    target = RobotPosition(2, 3)

    server = ControlCenter()
    server.robot7.print_current_pos()
    # server.mqtt.loop()
    # server.mqtt.loop_forever()

    while True:
        x = input('Enter x: ')
        y = input('Enter y: ')
        target.change_position(x, y)
        server.robot7.go_to_pos(target)
        if input('Enter 0 to quit: ') == '0':
            break
        




