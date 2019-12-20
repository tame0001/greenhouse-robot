from flask import Flask
from flask_restful import Api
from flask_cors import CORS

from robot_parameter import ParameterRequest

app = Flask(__name__)
CORS(app)
api = Api(app)

api.add_resource(ParameterRequest, '/parameter/<robot_id>')

if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)