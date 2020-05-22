import random

def random_coordinate():
    x_pos = random.randint(1, number_rows-2)
    y_pos = random.randint(1, number_cols-2)
    return (x_pos, y_pos)

def display_position(position_array):
    for row in range(len(position_array)):
        print('{}'.format(position_array[row]))
    

number_rows = 6
number_cols = 10

position = []

for x in range(number_rows):
    position.append([])
    for y in range(number_cols):
        position[x].append('xx')

robot_meta = []
robot_number = range(1, 25)
for robot in robot_number:
    x_pos, y_pos = random_coordinate()
    while position[x_pos][y_pos] != 'xx':
        x_pos, y_pos = random_coordinate()
    
    position[x_pos][y_pos] = '{:02d}'.format(robot)
    meta_data = {
        'id': robot,
        'x': x_pos,
        'y': y_pos,
        'dir': random.randint(1, 4)
    }
    robot_meta.append(meta_data)

display_position(position)



