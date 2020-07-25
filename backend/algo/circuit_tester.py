import paho.mqtt.client as mqtt

import time

next_command = 'q'
count = 0

def cal_next_command():
    if next_command == 'r':
        print("change next command to q")
        # next_command = 'q'
    else:
        print("change next command to r")
        # next_command = 'r'

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    mqttc.subscribe("irobot/feedback")

def on_message(client, userdata, msg):
    # print(msg.topic)
    # print(msg.payload.decode('ascii'))
    # print(msg.payload.decode('ascii').split(":"))
    in_payload = msg.payload.decode('ascii').split(':')
    robot_id = in_payload[0]
    status = in_payload[1]
    topic = 'irobot/command/{}'.format(robot_id)
    
    if status == '0':
        # count = count+1
        print("robot is stop")
        print('next command will be {}'.format(next_command))
        time.sleep(20)
        # if current_command == 'q':
        #     print("change new command to forward")
        #     new_command = 'r'
        #     print("test again")
            
        # else:
        #     print("change new command to left")
        #     new_command = 'q'
        print('sending new command {}'.format(next_command))
        mqttc.publish(topic, next_command)
        cal_next_command()
        print("number of turn {}".format(count))


    # elif status == '2':
    #     next_command = 'q'
    #     print("next command is {}".format(new_command))

    # elif status == '3':
    #     next_command = 'r'
    #     print("next command is {}".format(new_command))


mqttc = mqtt.Client()
mqttc.on_connect = on_connect
mqttc.on_message = on_message

mqttc.connect("192.168.1.191", 1883, 60)



mqttc.loop_forever()