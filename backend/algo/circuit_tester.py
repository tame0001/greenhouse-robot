import paho.mqtt.client as mqtt
import time

class PotCarrier:

    def __init__(self):
        self.count_turn = 0
        self.next_command = 'q'

    def process_status(self, client, userdata, msg):

        in_payload = msg.payload.decode('ascii').split(':')
        robot_id = in_payload[0]
        status = in_payload[1]
        topic = 'irobot/command/{}'.format(robot_id)     

        if status == '0':
            self.count_turn = self.count_turn + 1
            print('number of turnning completed {}'.format(self.count_turn))

            # if self.next_command == 'r':
            #     self.next_command = 'q'
            # else:
            #     self.next_command = 'r'

            time.sleep(5)
            print('sending new command {}'.format(self.next_command))
            mqttc.publish(topic, self.next_command)

        else:
            if (status == '2'):
                self.next_command = 'q'
            else:
                self.next_command = 'r'
            print('next command will be {}'.format(self.next_command))
            




def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    mqttc.subscribe("irobot/feedback")

robot = PotCarrier()

mqttc = mqtt.Client()
mqttc.on_connect = on_connect
mqttc.on_message = robot.process_status

mqttc.connect("192.168.1.191", 1883, 60)

mqttc.loop_forever()