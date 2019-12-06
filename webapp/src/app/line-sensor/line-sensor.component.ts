import { Component, OnInit, OnDestroy } from '@angular/core';
import { Subscription } from 'rxjs';
import { MqttService, IMqttMessage } from 'ngx-mqtt';

@Component({
  selector: 'app-line-sensor',
  templateUrl: './line-sensor.component.html',
  styleUrls: ['./line-sensor.component.css']
})

export class LineSensorComponent implements OnInit, OnDestroy {
  private parameterSub: Subscription;
  private feedbackSub: Subscription;
  public mqttMessage: string;
  public currentStatus: string;
  public splitedMessage: any;
  public robotID: string;
  public letfSpeed: string;
  public rightSpeed: string;
  public conmmandTopic: string;

  constructor(private mqttService: MqttService) {

    this.parameterSub = this.mqttService.observe('irobot/parameters').subscribe((
      message: IMqttMessage) => {
        this.mqttMessage = message.payload.toString();
        console.log(this.mqttMessage);
        this.splitedMessage = this.mqttMessage.split(':');
        // console.log(this.splitedMessage);
        this.robotID = this.splitedMessage[0];
        this.letfSpeed = this.splitedMessage[1];
        this.rightSpeed = this.splitedMessage[2];
        this.conmmandTopic = 'irobot/command/'.concat(this.robotID);
      });

    this.feedbackSub = this.mqttService.observe('irobot/feedback').subscribe((
        message: IMqttMessage) => {
          this.mqttMessage = message.payload.toString();
          console.log(this.mqttMessage);
          this.splitedMessage = this.mqttMessage.split(':');
          this.robotID = this.splitedMessage[0];
          switch (this.splitedMessage[1]) {
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
              this.currentStatus = 'STOPPING';
              break;
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

  ngOnInit() {
    // this.sendPing();
  }

  public unsafePublish(topic: string, message: string): void {
    this.mqttService.unsafePublish(topic, message, {qos: 1, retain: false});
  }

  public ngOnDestroy() {
    this.parameterSub.unsubscribe();
  }

}