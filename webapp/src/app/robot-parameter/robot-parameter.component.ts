import { Component, OnInit, OnDestroy, Input } from '@angular/core';
import { Subscription } from 'rxjs';
import { MqttService, IMqttMessage } from 'ngx-mqtt';
import { HttpClient} from '@angular/common/http';

@Component({
  selector: 'app-robot-parameter',
  templateUrl: './robot-parameter.component.html',
  styleUrls: ['./robot-parameter.component.css']
})

export class RobotParameterComponent implements OnInit, OnDestroy{

  @Input() robotID: string;

  parameterSub: Subscription;
  feedbackSub: Subscription;
  mqttMessage: string;
  splitedMessage: any;
  letfSpeed: string = '-';
  rightSpeed: string = '-';
  temperatureOnBoard: any = '-';
  batteryLevel: any = '-';
  drivingTime: string;
  turningTime: string;
  conmmandTopic: string;
  isBeacon: boolean;
  isLineLED: boolean;

  showConfigureBool: boolean = false;
  currentStatus: string = 'OFFLINE';
  backendURL: string = 'http://localhost:5000';
  // backendURL: string = 'http://128.46.109.133:5000';

  constructor(private mqttService: MqttService, private http:HttpClient) {    

    this.parameterSub = this.mqttService.observe('irobot/parameters').subscribe((
      message: IMqttMessage) => {
        this.mqttMessage = message.payload.toString();
        // console.log(this.mqttMessage);
        this.splitedMessage = this.mqttMessage.split(':');
        // console.log(this.splitedMessage);
        if (this.splitedMessage[0] == this.robotID && this.splitedMessage[1] == '0') {
          this.letfSpeed = this.splitedMessage[2];
          this.rightSpeed = this.splitedMessage[3];
          this.temperatureOnBoard = (Number(this.splitedMessage[4]) * 0.125).toFixed(1);
          this.batteryLevel = (Number(this.splitedMessage[5]) * 5.5 / 1280).toFixed(1);

          if (this.currentStatus == 'OFFLINE') {
            this.currentStatus = 'STOPPING';
          }
        } 
      });

    this.feedbackSub = this.mqttService.observe('irobot/feedback').subscribe((
        message: IMqttMessage) => {
          this.mqttMessage = message.payload.toString();
          // console.log(this.mqttMessage);
          this.splitedMessage = this.mqttMessage.split(':');
          // console.log(this.splitedMessage);
          if (this.splitedMessage[0] == this.robotID) {
            switch (this.splitedMessage[2]) {
              case '0':
                this.currentStatus = 'STOPPING';
                break;
              case '1':
                this.currentStatus = 'RUNNING';
                break;
              case '2':
                this.currentStatus = 'MOVING';
                break;
              case '3':
                this.currentStatus = 'TURNING LEFT';
                break;
              case '4':
                this.currentStatus = 'TURNING RIGHT';
                break;
              default:
                this.currentStatus = 'OFFLINE';
                break;
            }
          }

        });
  }

  sendRun() {
    this.unsafePublish(this.conmmandTopic, 'w');
  }

  sendStop() {
    this.unsafePublish(this.conmmandTopic, 's');
  }

  sendLeft() {
    this.unsafePublish(this.conmmandTopic, 'q');
  }

  sendRight() {
    this.unsafePublish(this.conmmandTopic, 'e');
  }

  sendForward() {
    this.unsafePublish(this.conmmandTopic, 'r');
  }

  getParameters() {
    this.http.get<any>(this.backendURL.concat('/parameter/').concat(this.robotID)).subscribe(data => {
      // console.log(data.robot_id);
      this.drivingTime = data.driving_time;
      this.turningTime = data.turning_time;
    });
  }

  showConfigure() {
    this.showConfigureBool = true;
  }

  hideConfigure() {
    this.showConfigureBool = false;
  }

  updateParameters() {
    // console.log(this.drivingTime);
    // console.log(this.turningTime);
    this.http.post(this.backendURL.concat('/parameter/').concat(this.robotID), 
      {
        "driving_time": this.drivingTime,
        "turning_time": this.turningTime
      }).subscribe(
        data => {
          console.log(data);
        },
        error => {
          console.log(error);
        }
      );
      this.unsafePublish(this.conmmandTopic, 'u');
  }

  ngOnInit() {
    this.conmmandTopic = 'irobot/command/'.concat(this.robotID);
    // this.getParameters();
  }

  isLineLEDChange() {
    if (this.isLineLED == true) {
      this.unsafePublish(this.conmmandTopic, 'p');
    }
    else {
      this.unsafePublish(this.conmmandTopic, 'o');
    }
  }

  isBeaconChange() {
    if (this.isBeacon == true) {
      this.unsafePublish(this.conmmandTopic, 'i');
    }
    else {
      this.unsafePublish(this.conmmandTopic, 'u');
    }
  }

  public unsafePublish(topic: string, message: string): void {
    this.mqttService.unsafePublish(topic, message, {qos: 1, retain: true});
  }

  public ngOnDestroy() {
    this.parameterSub.unsubscribe();
  }
}
