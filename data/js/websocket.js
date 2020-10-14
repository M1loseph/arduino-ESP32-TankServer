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

// logging function to see if gamepad has been conneted
// window.addEventListener("gamepadconnected", function (e) {
//     let gp = navigator.getGamepads()[e.gamepad.index];
//     "Gamepad connected at index %d: %s. %d buttons, %d axes.",
//         gp.index, gp.id, gp.buttons.length, gp.axes.length
// });

function sendWS(message) {
    console.log(message);
    if (webSocket.readyState === WebSocket.OPEN) {
        webSocket.send(message);
    }
}

export { sendWS };