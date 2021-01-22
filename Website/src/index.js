const { response } = require('express');

var clientIO = require('socket.io').listen(3200);
var cppIO = require('socket.io').listen(3000);

var toCpp = [
    "sys-info"
];

var toAngularClient = [
    "sys-info-reply"
];

clientIO.sockets.on('connection', function(socket) {
    console.log("Site client connected");

    for(let i = 0; i < toCpp.length; i++) {
        socket.on(toCpp[i], () => {
            cppIO.sockets.emit(toCpp[i]);
        });
    }
});

cppIO.sockets.on('connection', function(socket) {
    console.log("CPP client connected");

    for(let i = 0; i < toAngularClient.length; i++) {
        socket.on(toAngularClient[i], data => {
            console.log(data);
            clientIO.sockets.emit(toAngularClient[i], data);
        })
    }
});