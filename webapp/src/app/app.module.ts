import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { MqttModule, IMqttServiceOptions } from 'ngx-mqtt';
import { HttpClientModule} from '@angular/common/http';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { FormsModule } from '@angular/forms';

import { LineSensorComponent } from './line-sensor/line-sensor.component';
import { RobotParameterComponent } from './robot-parameter/robot-parameter.component';

export const MQTT_SERVICE_OPTIONS: IMqttServiceOptions = {
  // hostname: '128.46.109.133',
  hostname: '192.168.1.191',
  port: 9001,
  path: '/'
};

@NgModule({
  declarations: [
    AppComponent,
    LineSensorComponent,
    RobotParameterComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    FormsModule,
    MqttModule.forRoot(MQTT_SERVICE_OPTIONS)
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
