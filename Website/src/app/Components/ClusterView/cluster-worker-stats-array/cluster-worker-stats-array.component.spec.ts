import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ClusterWorkerStatsArrayComponent } from './cluster-worker-stats-array.component';

describe('ClusterWorkerStatsArrayComponent', () => {
  let component: ClusterWorkerStatsArrayComponent;
  let fixture: ComponentFixture<ClusterWorkerStatsArrayComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ClusterWorkerStatsArrayComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ClusterWorkerStatsArrayComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
