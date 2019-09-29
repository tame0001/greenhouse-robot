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

  constructor(private mqttService: MqttService) {

    this.parameterSub = this.mqttService.observe('irobot/parameters').subscribe((
      message: IMqttMessage) => {
        this.mqttMessage = message.payload.toString();
        console.log(this.mqttMessage);
        this.splitedMessage = this.mqttMessage.split(':');
        console.log(this.splitedMessage);
        this.robotID = this.splitedMessage[0];
        this.letfSpeed = this.splitedMessage[1];
        this.rightSpeed = this.splitedMessage[2];
      });

    this.feedbackSub = this.mqttService.observe('irobot/feedback').subscribe((
        message: IMqttMessage) => {
          this.mqttMessage = message.payload.toString();
          console.log(this.mqttMessage);
          this.splitedMessage = this.mqttMessage.split(':');
          this.robotID = this.splitedMessage[0];
          if (this.splitedMessage[1] === '1') {
            this.currentStatus = 'RUNNING';
          } else {
            this.currentStatus = 'STOPPING';
          }

        });
   }

  sendRun() {
    this.unsafePublish('irobot/command/17', 'r');
  }

  sendStop() {
    this.unsafePublish('irobot/command/17', 's');
  }

  sendPing() {
    this.unsafePublish('irobot/command/17', 'p');
  }

  ngOnInit() {
    this.sendPing()
  }

  public unsafePublish(topic: string, message: string): void {
    this.mqttService.unsafePublish(topic, message, {qos: 1, retain: false});
  }

  public ngOnDestroy() {
    this.parameterSub.unsubscribe();
  }

}
