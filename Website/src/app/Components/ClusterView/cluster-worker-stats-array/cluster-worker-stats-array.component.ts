import { SocketService } from './../../../Services/socket.service';
import { ClusterWorkerStatsComponent } from './../cluster-worker-stats/cluster-worker-stats.component';
import { Component, OnInit, ViewChildren, QueryList, AfterViewInit } from '@angular/core';
import { interval } from 'rxjs';

@Component({
  selector: 'app-cluster-worker-stats-array',
  templateUrl: './cluster-worker-stats-array.component.html',
  styleUrls: ['./cluster-worker-stats-array.component.css']
})
export class ClusterWorkerStatsArrayComponent implements AfterViewInit, OnInit {
    @ViewChildren(ClusterWorkerStatsComponent) stats: QueryList<ClusterWorkerStatsComponent>;

    constructor(private socketService: SocketService) { }

    ngOnInit() {

    }

    ngAfterViewInit() {
        interval(1000).subscribe((n) => {
            console.log('Sending System Info Query');

            this.socketService.socket.emit('sys-info');
        });

        this.socketService.socket.on("sys-info-reply", (response => {
            console.log('Recieved System Info: ');
            console.log(response);

            this.stats.first.setRamPercentage((response['Used RAM'] / response['Total RAM']) * 100);
        }));
    }
}
