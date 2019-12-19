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

  private parameterSub: Subscription;
  private feedbackSub: Subscription;
  public mqttMessage: string;
  public currentStatus: string;
  public splitedMessage: any;
  public letfSpeed: string;
  public rightSpeed: string;
  public drivingTime: string;
  public turningTime: string;

  private conmmandTopic: string = 'irobot/command/'.concat(this.robotID);
  private backendURL: string = 'http://localhost:5000';

  constructor(private mqttService: MqttService, private http:HttpClient) { 

    this.parameterSub = this.mqttService.observe('irobot/parameters').subscribe((
      message: IMqttMessage) => {
        this.mqttMessage = message.payload.toString();
        // console.log(this.mqttMessage);
        this.splitedMessage = this.mqttMessage.split(':');
        // console.log(this.splitedMessage);
        // this.robotID = this.splitedMessage[0];
        if (this.splitedMessage[0] == this.robotID) {
          this.letfSpeed = this.splitedMessage[1];
          this.rightSpeed = this.splitedMessage[2];
        } 
      });

    this.feedbackSub = this.mqttService.observe('irobot/feedback').subscribe((
        message: IMqttMessage) => {
          this.mqttMessage = message.payload.toString();
          // console.log(this.mqttMessage);
          this.splitedMessage = this.mqttMessage.split(':');
          // this.robotID = this.splitedMessage[0];
          if (this.splitedMessage[0] == this.robotID) {
            switch (this.splitedMessage[1]) {
              case '0':
                this.currentStatus = 'STOPPING';
                break;
              case '1':
                this.currentStatus = 'RUNNING';
                break;
              case '2':
                this.currentStatus = 'TURNING 180';
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

  sendPing() {
    this.unsafePublish(this.conmmandTopic, 'p');
  }

  sendLeft() {
    this.unsafePublish(this.conmmandTopic, 'q');
  }

  sendRight() {
    this.unsafePublish(this.conmmandTopic, 'e');
  }

  sendTurn() {
    this.unsafePublish(this.conmmandTopic, 'r');
  }

  getParameters() {
    this.http.get<any>(this.backendURL.concat('/parameter/').concat(this.robotID)).subscribe(data => {
      // console.log(data.robot_id);
      this.drivingTime = data.driving_time;
      this.turningTime = data.turning_time;
    })
  }

  ngOnInit() {
    this.getParameters();
  }

  public unsafePublish(topic: string, message: string): void {
    this.mqttService.unsafePublish(topic, message, {qos: 1, retain: false});
  }

  public ngOnDestroy() {
    this.parameterSub.unsubscribe();
  }
}
