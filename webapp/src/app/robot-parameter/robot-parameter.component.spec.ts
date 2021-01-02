import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { RobotParameterComponent } from './robot-parameter.component';

describe('RobotParameterComponent', () => {
  let component: RobotParameterComponent;
  let fixture: ComponentFixture<RobotParameterComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ RobotParameterComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(RobotParameterComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
