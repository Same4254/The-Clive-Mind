import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { TrainingGraphComponent } from './Components/Training/Graph/training-graph.component';

import { MatButtonModule } from '@angular/material/button';
import { MatIconModule } from '@angular/material/icon';
import { MatProgressBarModule } from '@angular/material/progress-bar';

import { ClusterWorkerStatsComponent } from './Components/ClusterView/cluster-worker-stats/cluster-worker-stats.component';
import { ClusterWorkerStatsArrayComponent } from './Components/ClusterView/cluster-worker-stats-array/cluster-worker-stats-array.component';

@NgModule({
  declarations: [
    AppComponent,
    TrainingGraphComponent,
    ClusterWorkerStatsComponent,
    ClusterWorkerStatsArrayComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    BrowserAnimationsModule,

    MatButtonModule,
    MatIconModule,
    MatProgressBarModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
