import { Component, OnInit, OnDestroy } from '@angular/core';
import { Subscription } from 'rxjs';
import { MqttService, IMqttMessage } from 'ngx-mqtt';

@Component({
  selector: 'app-line-sensor',
  templateUrl: './line-sensor.component.html',
  styleUrls: ['./line-sensor.component.css']
})

export class LineSensorComponent implements OnInit, OnDestroy {
  private subscription: Subscription;
  public message: string;

  constructor(private mqttService: MqttService) {
    this.subscription = this.mqttService.observe('irobot/parameters').subscribe((
      message: IMqttMessage) => {
        this.message = message.payload.toString();
        console.log(this.message);
      });
   }

  sendRun() {
    this.unsafePublish('irobot/command/17', 'r');
  }

  sendStop() {
    this.unsafePublish('irobot/command/17', 's');
  }

  ngOnInit() {
  }

  public unsafePublish(topic: string, message: string): void {
    this.mqttService.unsafePublish(topic, message, {qos: 1, retain: false});
  }

  public ngOnDestroy() {
    this.subscription.unsubscribe();
  }

}
