// connect to websocket server
const WS_ADDRESS = "ws://192.168.4.1/ws";
let webSocket = new WebSocket(WS_ADDRESS);

webSocket.onopen = e => {
    console.log("Connected to WS server");
    sendWS({ controller: "config", command: "get" })
};

webSocket.onerror = e => {
    console.log("Error while connecting to WS server");
    console.log(e);
};

const sendWSMany = (list) => {
    list.forEach(elem => sendWS(elem));
}

const sendWS = (message) => {
    const stringified = JSON.stringify(message);
    console.log(stringified);
    if (webSocket.readyState === WebSocket.OPEN) {
        webSocket.send(stringified);
    }
}

const setOnRecive = (fun) => {
    webSocket.onmessage = fun
}

export { sendWS, sendWSMany, setOnRecive };