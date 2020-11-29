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

const sendWS = (message) => {
    const stringified = JSON.stringify(message);
    console.log(stringified);
    if (webSocket.readyState === WebSocket.OPEN) {
        webSocket.send(stringified);
    }
}

export { sendWS };