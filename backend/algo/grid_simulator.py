from path_planning import gen_movement_cmd, list_obstacle

from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from models import db, Position

import datetime

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
 
number_rows = 5
number_cols = 7
number_robots = 10

position_meta = []

DATABASE_URI = 'postgres+psycopg2://postgres@128.46.109.133:5432/mydb'

engine = create_engine(DATABASE_URI)
Session = sessionmaker(bind=engine)

session = Session()

for x in range(number_rows):
    position_meta.append([])
    for y in range(number_cols):
        position_meta[x].append('xx')

robot_meta = []
robot_number = range(1, number_robots+1)
for robot in robot_number:
    position = session.query(Position).filter(Position.robot==robot).order_by(Position.timestamp.desc()).first()
    x_pos = position.x
    y_pos = position.y

    position_meta[x_pos][y_pos] = '{:02d}'.format(robot)
    meta_data = {
        'id': robot,
        'x': x_pos,
        'y': y_pos,
        'dir': position.direction
    }
    robot_meta.append(meta_data)

display_position(position_meta)


while True:
    
    selected_robot = input('Select robot id to move (1-{}) or 0 to quit: '.format(number_robots))
    try:
        selected_robot = int(selected_robot)
    except err:
        print('please enter an integer')
        continue

    if selected_robot < 0 or selected_robot > number_robots:
        print('Invalid robot id')
        continue
    elif selected_robot == 0:
        break


    free_spots = find_free_spot(position_meta)
    for spot in range(len(free_spots)):
        print('({}): {}'.format(spot+1, free_spots[spot]))
    
    target_spot = input('Select target spot to move (1-{}): '.format(len(free_spots)))
    try:
        target_spot = int(target_spot)
    except err:
        print('please enter an integer')
        continue

    if target_spot < 1 or target_spot > len(free_spots):
        print('Invalid spot')
        continue
    
    target_position = free_spots[target_spot-1]
    current_position = (robot_meta[selected_robot-1]['x'], robot_meta[selected_robot-1]['y'])
    current_direction = robot_meta[selected_robot-1]['dir']
    
    print('Move robot id {} from {} to {}'.format(selected_robot, current_position, target_position))
    print('The current direction is {}'.format(current_direction))
    print('Following is the command for robot id {}'.format(selected_robot))
    commands, current_direction = gen_movement_cmd(current_position, target_position, current_direction)
    print(commands)
    print('The current direction is {}'.format(current_direction))
    
    obstacle = list_obstacle(position_meta, commands, current_position, robot_meta[selected_robot-1]['dir'])
    print('During an operation these following robots have to move')
    print(obstacle)
    
    robot_meta[selected_robot-1]['x'] = target_position[0]
    robot_meta[selected_robot-1]['y'] = target_position[1]
    robot_meta[selected_robot-1]['dir'] = current_direction
    position_meta[current_position[0]][current_position[1]] = 'xx'
    position_meta[target_position[0]][target_position[1]] = '{:02d}'.format(selected_robot)

    postion = Position(
        robot = selected_robot,
        x = target_position[0],
        y = target_position[1],
        direction = current_direction,
        timestamp = datetime.datetime.now()
    )
    session.add(postion)
    session.commit()
    
    print('The grid after operation')
    display_position(position_meta)
    print('------------------------------------------------------------------------')

session.close()
  