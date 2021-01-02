from sqlalchemy import Column, Integer, String, DateTime
from sqlalchemy.ext.declarative import declarative_base

db = declarative_base()

class Position(db):
    __tablename__ = 'positions'
    id = Column(Integer, primary_key=True)
    robot = Column(Integer)
    x = Column(Integer)
    y = Column(Integer)
    direction = Column(Integer)
    timestamp = Column(DateTime)

    def __repr__(self):
        return 'Robot ID {} was in ({}, {}) heading {} at {}'.format(
            self.robot, self.x, self.y, self.direction, self.timestamp
        )

    def convert_json(self):
        return {
            'id': self.robot,
            'x': self.x,
            'y':self.y,
            'direction': self.direction,
            'timestamp': self.timestamp 
        }