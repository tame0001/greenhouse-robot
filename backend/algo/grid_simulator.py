import random
from path_planning import gen_movement_cmd, list_obstacle

def random_coordinate():
    x_pos = random.randint(1, number_rows-2)
    y_pos = random.randint(1, number_cols-2)
    return (x_pos, y_pos)

def display_position(position_array):
    for row in range(len(position_array)):
        print('{}'.format(position_array[row]))
        
def find_free_spot(position_array):
    free_spot = []
    for row in range(1, number_rows-1):
        for col in range(1, number_cols-1):
            if position_array[row][col] == 'xx':
                free_spot.append((row, col))
    return free_spot
 
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


while True:
    
    rio = input('Select robot id to move (1-24) or 0 to quit: ')
    try:
        rio = int(rio)
    except err:
        print('please enter an integer')
        continue

    if rio < 0 or rio > 24:
        print('Invalid robot id')
        continue
    elif rio == 0:
        break


    free_spots = find_free_spot(position)
    for spot in range(len(free_spots)):
        print('({}): {}'.format(spot+1, free_spots[spot]))
    
    target_spot = input('Select target spot to move (1-8): ')
    try:
        target_spot = int(target_spot)
    except err:
        print('please enter an integer')
        continue

    if target_spot < 1 or target_spot > 8:
        print('Invalid spot')
        continue
    
    target_position = free_spots[target_spot-1]
    current_position = (robot_meta[rio-1]['x'], robot_meta[rio-1]['y'])
    current_direction = robot_meta[rio-1]['dir']
    
    print('Move robot id {} from {} to {}'.format(rio, current_position, target_position))
    print('The current direction is {}'.format(current_direction))
    print('Following is the command for robot id {}'.format(rio))
    commands, current_direction = gen_movement_cmd(current_position, target_position, current_direction)
    print(commands)
    print('The current direction is {}'.format(current_direction))
    
    obstacle = list_obstacle(position, commands, current_position, robot_meta[rio-1]['dir'])
    print('During an operation these following robots have to move')
    print(obstacle)
    
    robot_meta[rio-1]['x'] = target_position[0]
    robot_meta[rio-1]['y'] = target_position[1]
    robot_meta[rio-1]['dir'] = current_direction
    position[current_position[0]][current_position[1]] = 'xx'
    position[target_position[0]][target_position[1]] = '{:02d}'.format(rio)
    
    print('The grid after operation')
    display_position(position)
    print('------------------------------------------------------------------------')
  