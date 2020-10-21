// connect to websocket server
const WS_ADDRESS = "ws://192.168.4.1/ws";
let webSocket = new WebSocket(WS_ADDRESS);

webSocket.onopen = e => {
    console.log("Connected to WS server");
    console.log(e);
};

webSocket.onerror = e => {
    console.log("Error while connecting to WS server");
    console.log(e);
};

webSocket.onmessage = e => {
    console.log("Received message from server");
    console.log(e);
};

function sendWS(message) {
    console.log(message);
    if (webSocket.readyState === WebSocket.OPEN) {
        webSocket.send(message);
    }
}

export { sendWS };