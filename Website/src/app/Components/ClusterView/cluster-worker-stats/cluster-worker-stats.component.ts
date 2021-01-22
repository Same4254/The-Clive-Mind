import { Component, OnInit } from '@angular/core';
import { MatProgressBarModule } from '@angular/material/progress-bar';

import { SocketService } from '../../../Services/socket.service'

@Component({
  selector: 'cluster-worker-stats',
  templateUrl: './cluster-worker-stats.component.html',
  styleUrls: ['./cluster-worker-stats.component.css']
})
export class ClusterWorkerStatsComponent implements OnInit {
    public ramUsage = 0;

    constructor(private socketService : SocketService) { }

    ngOnInit(): void {

    }

    public setRamPercentage(ramUsage) {
        this.ramUsage = ramUsage;
    }
}
