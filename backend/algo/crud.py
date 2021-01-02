from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from models import db, Position

import datetime

# Scheme: "postgres+psycopg2://<USERNAME>:<PASSWORD>@<IP_ADDRESS>:<PORT>/<DATABASE_NAME>"
DATABASE_URI = 'postgres+psycopg2://postgres@128.46.109.133:5432/mydb'

engine = create_engine(DATABASE_URI)
Session = sessionmaker(bind=engine)

# db.metadata.drop_all(engine)
# db.metadata.create_all(engine)


session = Session()

postion = Position(
    robot = 10,
    x = 3,
    y = 5,
    direction = 2,
    timestamp = datetime.datetime.now()
)

# session.add(postion)
# session.commit()

positions = session.query(Position).filter(Position.robot==3).order_by(Position.timestamp.desc()).first()
print(positions)

session.close()

