import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { LineSensorComponent } from './line-sensor.component';

describe('LineSensorComponent', () => {
  let component: LineSensorComponent;
  let fixture: ComponentFixture<LineSensorComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ LineSensorComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(LineSensorComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
