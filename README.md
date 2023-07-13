# Greenhouse Robots

This was a project I was working on as a part of my master thesis before the pandemic.
Unfortunately, everyone needed to work from home and the project has to be on paused.
The original purpose of this project was to eliminate micro-climate in the greenhouse.
But I feel there are many more applications for robots in the greenhouse.
Future greenhouse will have a lot of sensors and robots to take actions instead of humans.
One day, this project will be active again.

## Current Project Structure

[main](./main/) was the original version of robot source code.
Then the source code was redesigned to be [robot_code_v2](./robot_code_v2/).
The robots are controlled by ESP32 controllers and communicate with MQTT and REST protocols.

[webapp](./webapp/) is the web-based application to monitor and control the robot fleet.
It was developed on Angular.

[backend](./backend/) is a backend server that process and send command to each robot in the fleet.
It was developed on Flask-RESTful.

[experiment](./experiment/) was used for experimenting concepts before intregrating to main source code.
