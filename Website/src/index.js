// io.on('connection', (socket) => {
//     socket.

//     console.log('a user connected');
//     socket.on('disconnect', () => {
//         console.log('user disconnected');
//     });

//     socket.on("Hello", (data) => {
//         console.log(data);
//     });
// });

// http.listen(3000, () => {
//     console.log('listening on *:3000');
// });

// console.log("hi");

const app = require('express')();
const http = require('http').createServer(app);
const io = require('socket.io')(http);

var clientIO = require('socket.io')(http);
var cppIO = require('socket.io').listen(3000);

http.listen(3200);

var forwardToCPP = ["create"];
var forwardToClients = ["created"];

clientIO.sockets.on('connection', function(socket) {
    console.log("Site client connected");

    for(label in forwardToCPP) {
        socket.on(label, data => {
            cppIO.emit(label, data);
        });
    }
});

clientIO.sockets.on('disconnection', (data) => {
    console.log("Site client disconnected");
});

cppIO.sockets.on('connection', function(socket) {
    console.log("CPP client connected");

    for(label in forwardToClients) {
        socket.on(label, data => {
            clientIO.emit(label, data);
        });
    }
});

cppIO.sockets.on("disconnection", (data) => {
    console.log("CPP disconnect");
})