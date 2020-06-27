import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { TrainingGraphComponent } from './training-graph.component';

describe('TrainingGraphComponent', () => {
  let component: TrainingGraphComponent;
  let fixture: ComponentFixture<TrainingGraphComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ TrainingGraphComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(TrainingGraphComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
