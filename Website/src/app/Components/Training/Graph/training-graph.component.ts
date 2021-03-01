import { Component, OnInit } from '@angular/core';
import { Chart } from 'chart.js';
import 'chartjs-plugin-zoom';
import 'chartjs-chart-graph';

import { SocketService } from './../../../Services/socket.service';

@Component({
    selector: 'app-training-graph',
    templateUrl: './training-graph.component.html',
    styleUrls: ['./training-graph.component.css']
})
export class TrainingGraphComponent implements OnInit {
    private chart: Chart;
    private branches = [];
    private names: string[][] = [];

    constructor(private socketService: SocketService) {}

    ngOnInit(): void {        
        this.createChart();

        this.addBaseState(0, .5, "001.001");
        this.addState(0, 0, 0, .5, "001.001", 1, .25, "001.002");
        this.addState(1, 0, 0, .5, "001.001", 1, .75, "002.001");
        this.addState(1, 1, 1, .75, "002.001", 2, .4, "002.002");

        // this.addBaseState(2, .3, "003.001");
        // this.addState(2, 2, 0, .3, "003.001", 1, .1, "003.002");

        // this.addState(3, 1, 1, .75, "002.001", 2, .9, "004.001");

        this.chart.update();
    }

    private random_rgba() : string {
        return 'rgba(' + Math.round(Math.random() * 255) + ',' + Math.round(Math.random() * 255) + ',' + Math.round(Math.random() * 255) + ',' + 0.8 + ')';
    }

    private createBranch(newBranchIndex: number, fromBranchIndex: number, fromIteration: number, fromError: number, fromName: string) : void {
        this.branches[newBranchIndex] = {
            offset: fromIteration,
            fromDataBranch: fromBranchIndex,

            fill: false,
            borderWidth: 5,
            borderColor: this.random_rgba(),
            pointBorderColor: 'rgba(255, 0, 0, 0.6)',
            pointBorderWidth: 8,

            data: [{x: fromIteration, y: fromError}],

        };

        this.names[newBranchIndex] = [fromName];
    }

    public addState(branchIndex: number, fromBranchIndex: number, fromIteration: number, fromError: number, fromName: string, iteration: number, error: number, name: string) : void {
        if(branchIndex >= this.branches.length)
            this.createBranch(branchIndex, fromBranchIndex, fromIteration, fromError, fromName);

        this.names[branchIndex][iteration - this.branches[branchIndex]["offset"]] = name;
        this.branches[branchIndex]['data'].push({x: iteration, y: error});
    }

    private addBaseState(branchIndex: number, error: number, name: string) : void {
        if(branchIndex < this.branches.length) {
            console.error("Cannot add base state to a branch that exists!");
            return;
        }

        this.createBranch(branchIndex, 0, 0, error, name);
    }

    public updateChart() : void {
        this.chart.update();
    }

    private createChart() : void {
        this.chart = new Chart('canvas', {
            type: 'line',
            data: {
                datasets: this.branches
            },

            options: {
                title: {
                    text: 'Training Chart',
                    display: true
                },
                
                responsive: true,
                maintainAspectRatio: false,

                plugins: {
                    zoom: {
                        pan: {
                            enabled: true,
                            mode: 'xy'
                        },
                        zoom: {
                            enabled: true,
                            mode: 'xy'
                        }
                    }
                },

                legend: {
                    display: false
                },

                elements: {
                    line: {
                        tension: 0
                    }
                },
                
                hover: {
                    animationDuration: 200,
                    mode: "single"
                },

                scales: {
                    xAxes: [
                        {
                            type: 'linear',
                            display: true,

                            ticks: {
                                beginAtZero: true,

                                // callback: (value: number) => { 
                                //     if (Number.isInteger(value))
                                //         return value; 
                                // },

                                stepSize: 1
                            },

                            scaleLabel: {
                                display: true,
                                labelString: 'Iterations'
                            }
                        }
                    ], 

                    yAxes: [
                        {
                            type: 'linear',
                            display: true,

                            ticks: {
                                beginAtZero: true
                            },

                            scaleLabel: {
                                display: true,
                                labelString: 'Error'
                            }
                        }
                    ]
                },

                tooltips: {
                    mode: "single",

                    custom: (tooltip) => {
                        if(!tooltip) return;

                        tooltip.displayColors = false;
                    },

                    callbacks: {
                        label: (tooltipItem, data) : string[] => {
                            return [this.names[tooltipItem.datasetIndex][tooltipItem.index], "Error: " + tooltipItem.yLabel.toString()];
                        },

                        title: (tooltipItem, data) : any => {
                            return;
                        }
                    }
                },

                onClick: (event) => {
                    var activePoints = this.chart.getElementAtEvent(event);

                    // console.log(this.chart.getDatasetAtEvent(event));
                    console.log(activePoints)

                    // if(!activePoints.length) return;

                    // let point = activePoints[0];

                    // var index = point['_index'];
                    // var datasetIndex = point['_datasetIndex'];

                    // console.log(point);
                    // console.log(index);
                    // console.log(datasetIndex);
                }
            }
        });
    }
}
