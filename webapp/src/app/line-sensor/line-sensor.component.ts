import { Component, OnInit, OnDestroy, Input } from '@angular/core';
import { Subscription } from 'rxjs';
import { MqttService, IMqttMessage } from 'ngx-mqtt';

@Component({
  selector: 'app-line-sensor',
  templateUrl: './line-sensor.component.html',
  styleUrls: ['./line-sensor.component.css']
})

export class LineSensorComponent implements OnInit, OnDestroy {

  @Input() robotID: string;

  lineSensorSub: Subscription;
  mqttMessage: string;
  splitedMessage: any;
  lineSensor: string = 'xxxxxxxx';

  constructor(private mqttService: MqttService) {

    this.lineSensorSub = this.mqttService.observe('irobot/linesensor').subscribe((
      message: IMqttMessage) => {
        this.mqttMessage = message.payload.toString();
        // console.log(this.mqttMessage);
        this.splitedMessage = this.mqttMessage.split(':');
        // console.log(this.splitedMessage);
        // this.robotID = this.splitedMessage[0];
        if (this.splitedMessage[0] == this.robotID) {
          this.lineSensor = this.splitedMessage[1];
        } 
      });
   }

  ngOnInit() {

  }

  public unsafePublish(topic: string, message: string): void {
    this.mqttService.unsafePublish(topic, message, {qos: 1, retain: false});
  }

  public ngOnDestroy() {
    this.lineSensorSub.unsubscribe();
  }

}
