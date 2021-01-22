import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { ClusterWorkerStatsComponent } from './cluster-worker-stats.component';

describe('ClusterWorkerStatsComponent', () => {
  let component: ClusterWorkerStatsComponent;
  let fixture: ComponentFixture<ClusterWorkerStatsComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ ClusterWorkerStatsComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(ClusterWorkerStatsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
