const express = require('express');
const http = require('http');
const { WebSocketServer } = require('ws');

const app = express();
const server = http.createServer(app); // remplace app.listen()
const websocket = new WebSocketServer({ server }); // partage le même port

app.use(express.json());
app.use(express.static('public'));

// Gestion WebSocket

websocket.on('connection', (ws) => {
    console.log('Client connecté !');

    // Quand on reçoit un message
    ws.on('message', (data) => {
        console.log('Message reçu :', data.toString());

        // Répondre au client
        websocket.clients.forEach(user => {
            if (user !== ws) {
                user.send('Reçu : ' + data.toString());
            }
        });
    });

    ws.on('close', () => {
        console.log('Client déconnecté');
    });
});



// server.listen() remplace app.listen()
server.listen(2000, () => {
    let monIp = require("ip").address();
    console.log(`Server running on http://${monIp}:2000`);
    console.log(`WebSocket sur ws://${monIp}:2000`);
});