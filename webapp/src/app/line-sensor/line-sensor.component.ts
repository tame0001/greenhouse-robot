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
        console.log(this.message)
      });
   }

  ngOnInit() {
  }

  public ngOnDestroy() {
    this.subscription.unsubscribe();
  }

}
