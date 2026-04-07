const ws = new WebSocket('ws://172.29.18.255:2000');
ws.onopen = () => {
    console.log('WebSocket connecté !');
};

function sendMessage() {
    const messageInput = document.getElementById('messageInput');
    const user = document.getElementById('username');
    if (messageInput.value.length > 0 && user.value.length > 0) {
        ws.send(messageInput.value + " (de " + user.value + ")");
        console.log('Message envoyé :', messageInput.value);
    }

    else {
        console.error('WebSocket n\'est pas connecté.');
    }
}

function afficherMessage(message) {
    const chat = document.getElementById('messages');
    const box = document.createElement('div');
    box.textContent = message;
    chat.appendChild(box);
}

ws.onmessage = (recu) => {
    console.log('Message du serveur :', recu.data);
    afficherMessage(recu.data);
};