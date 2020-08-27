from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from models import db, Position

import datetime
import random

number_rows = 5
number_cols = 7
number_robots = 10

# Scheme: "postgres+psycopg2://<USERNAME>:<PASSWORD>@<IP_ADDRESS>:<PORT>/<DATABASE_NAME>"
DATABASE_URI = 'postgres+psycopg2://postgres@128.46.109.133:5432/mydb'

engine = create_engine(DATABASE_URI)
Session = sessionmaker(bind=engine)

session = Session()

occupied_position = []

for robot in range(1, number_robots+1):

    x_pos = random.randint(1, number_rows-2)
    y_pos = random.randint(1, number_cols-2)
    pos = (x_pos, y_pos)

    while pos in occupied_position:
        x_pos = random.randint(1, number_rows-2)
        y_pos = random.randint(1, number_cols-2)
        pos = (x_pos, y_pos)

    occupied_position.append(pos)

    postion = Position(
        robot = robot,
        x = x_pos,
        y = y_pos,
        direction = random.randint(1, 4),
        timestamp = datetime.datetime.now()
    )
    session.add(postion)

session.commit()
session.close()

