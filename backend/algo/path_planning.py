def gen_movement_cmd(start, stop, dirrection):
    ''' Direction
    1 = +x
    2 = +y
    3 = -x
    4 = -y '''
    x_distance = stop[0]-start[0]
    y_distance = stop[1]-start[1]

    commands = []

    if dirrection

    for i in range(x_distance):
        commands.append('F')

    print(commands)


if __name__ == '__main__':
    x1 = int(input('Initial X coordinate: '))
    y1 = int(input('Initial Y coordinate: '))
    start = (x1, y1)
    # print(start)

    x2 = int(input('Target X coordinate: '))
    y2 = int(input('Target Y coordinate: '))
    stop = (x2, y2)
    # print(stop)

    dirrection = int(input('X axis direction (1, 2, 3, 4): '))

    gen_movement_cmd(start, stop, dirrection)