from flask_restful import Resource, reqparse
import xml.etree.ElementTree as xml

xml_path = 'parameters.xml'
class ParameterRequest(Resource):

    def read_parameter_file(self, robot_id):
        parameter = {}
        with open(xml_path) as xml_file:
            xml_root = xml.parse(xml_file).getroot()
            try:
                robot = xml_root.find('./robot[@id="{}"]'.format(robot_id))
                # print(robot)
                # print(robot.attrib)
                # print(robot.tag)
                for config in robot:
                    # print(config)
                    parameter[config.tag] = int(config.text)
            except Exception:
                return None
            
        return parameter

    def write_parameter_file(self, robot_id, parameter):
        # print(robot_id)
        # print(parameter)
        with open(xml_path) as xml_file:
            xml_tree = xml.parse(xml_file)
            xml_root = xml_tree.getroot()
            robot = xml_root.find('./robot[@id="{}"]'.format(robot_id))
            # print(robot)
            # print(robot.getchildren())
            # print(robot.find('driving_time'))
        if robot is not None:
            for config in parameter:
                # print(robot.find('{}'.format(config)))
                robot.find('{}'.format(config)).text = parameter[config]
        
            xml_tree.write(xml_path)
            return True
        else:
            return False


    def get(self, robot_id):
      
        parameter = self.read_parameter_file(robot_id)
        # print(parameter)
        if parameter:
            return {
                'robot_id': robot_id,
                'driving_time': parameter['driving_time'],
                'turning_time': parameter['turning_time'],
                'code': 0
            }, 200
        else:
            return {
                'message': 'This robot id {} has no parameter record'.format(robot_id)
            }, 404

    
    def post(self, robot_id):

        parser = reqparse.RequestParser()
        parser.add_argument('driving_time')
        parser.add_argument('turning_time')
        args = parser.parse_args()
        # print(args)
        # print(self.write_parameter_file(robot_id ,args))
        if self.write_parameter_file(robot_id ,args):
            return {
                'message': 'update new parameter values for robot id: {}'.format(robot_id),
                'code': 0
            }, 201
        else:
            return {
                'message': 'This robot id {} has no parameter record'.format(robot_id)
            }, 404
