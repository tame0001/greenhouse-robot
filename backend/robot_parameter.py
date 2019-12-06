from flask_restful import Resource, reqparse

class ParameterRequest(Resource):

    def get(self, robot_id):

        return {
            'robot_id': robot_id,
            'driving_time': 0,
            'turning_time': 0,
            'code': 0
        }, 200
    
    def post(self, robot_id):

        parser = reqparse.RequestParser()
        parser.add_argument('driving_time')
        parser.add_argument('turning_time')
        args = parser.parse_args()


        print(args)
        
        return {
            'message': 'update new parameter values for robot id: {}'.format(robot_id),
            'code': 0
        }, 201