import random

def random_coordinate():
    x_pos = random.randint(1, number_rows-2)
    y_pos = random.randint(1, number_cols-2)
    return (x_pos, y_pos)

number_rows = 6
number_cols = 10

position = []

for x in range(number_rows):
    position.append([])
    for y in range(number_cols):
        position[x].append('xx')

robot_number = range(1, 25)
for robot in robot_number:
    x_pos, y_pos = random_coordinate()
    while position[x_pos][y_pos] != 'xx':
        x_pos, y_pos = random_coordinate()
    
    position[x_pos][y_pos] = '{:02d}'.format(robot)

for row in range(number_rows):
    print('{}'.format(position[row]))

