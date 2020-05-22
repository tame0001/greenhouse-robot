def gen_movement_cmd(start, stop, dirrection):
    ''' Direction
    1 = +x
    2 = +y
    3 = -x
    4 = -y '''
    x_distance = stop[0]-start[0]
    y_distance = stop[1]-start[1]

    commands = []
    ''' Command
    Q = Turn Left
    E = Turn Right
    E = Turn Around
    R = Forward '''

    if abs(x_distance) > 0:

        if (dirrection == 1 and x_distance < 0) or (dirrection == 3 and x_distance > 0):
            commands.append('T')

        elif (dirrection == 2 and x_distance < 0) or (dirrection == 4 and x_distance > 0):
            commands.append('Q')

        elif (dirrection == 2 and x_distance > 0) or (dirrection == 4 and x_distance < 0):
            commands.append('E')

        for i in range(abs(x_distance)):
            commands.append('R')

        dirrection = 1 if x_distance > 0 else 3


    if abs(y_distance) > 0:

        if (dirrection == 2 and y_distance < 0) or (dirrection == 4 and y_distance > 0):
            commands.append('T')

        elif (dirrection == 1 and y_distance > 0) or (dirrection == 3 and y_distance < 0):
            commands.append('Q')

        elif (dirrection == 1 and y_distance < 0) or (dirrection == 3 and y_distance > 0):
            commands.append('E')

        for i in range(abs(y_distance)):
            commands.append('R')

        dirrection = 2 if y_distance > 0 else 4
        

    # print(commands)
    return commands, dirrection

def list_obstacle(position_array, commands, start_pos, dirrection):
    obstacles = []
    current_x = start_pos[0]
    current_y = start_pos[1]
    current_direction = dirrection
    for command in commands:
        if command == 'T':
            current_direction = (current_direction+2)%4 if current_direction != 2 else 4
        elif command == 'Q':
            current_direction = current_direction+1 if current_direction != 4 else 1
        elif command == 'E':
            current_direction = current_direction-1 if current_direction != 1 else 4
        elif command == 'R':
            if current_direction == 1:
                current_x += 1
            elif current_direction == 2:
                current_y += 1
            elif current_direction == 3:
                current_x -= 1
            elif current_direction == 4:
                current_y -= 1
            obstacles.append(position_array[current_x][current_y])
    while 'xx' in obstacles:
        obstacles.remove('xx')
    return obstacles
        


if __name__ == '__main__':
    x1 = int(input('Initial X coordinate: '))
    y1 = int(input('Initial Y coordinate: '))
    start = (x1, y1)
    # print(start)

    x2 = int(input('Target X coordinate: '))
    y2 = int(input('Target Y coordinate: '))
    stop = (x2, y2)
    # print(stop)

    dirrection = int(input('Direction (1, 2, 3, 4): '))

    gen_movement_cmd(start, stop, dirrection)
