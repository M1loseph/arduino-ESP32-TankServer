import { DEFAULT_CONFIG, DEBUG_DRIVING_CONFIG } from './configs.js'
import { get_gamepad_info, process_gamepad } from './gamepad_processing.js'

// connect to websocket server
const WS_ADDRESS = "ws://192.168.4.1/ws"
const GAMEPAD_INTERVAL = 33
let webSocket = new WebSocket(WS_ADDRESS);


webSocket.onopen = function (e) {
    console.log("Connected to WS server");
    console.log(e);
};


webSocket.onerror = function (e) {
    console.log("Error while connecting to WS server");
    console.log(e);
};


webSocket.onmessage = function (e) {
    console.log("Received message from server");
    console.log(e);
};


// logging function to see if gamepad has been conneted
window.addEventListener("gamepadconnected", function (e) {
    let gp = navigator.getGamepads()[e.gamepad.index];
    "Gamepad connected at index %d: %s. %d buttons, %d axes.",
        gp.index, gp.id, gp.buttons.length, gp.axes.length
});


// checking gamepad info every few milliseconds
setInterval(function () {
    let gamepadState = get_gamepad_info();
    if (gamepadState) {
        let messages;
        let functions;

        [messages, functions] = process_gamepad(gamepadState, DEBUG_DRIVING_CONFIG);

        console.log(messages);

        functions.forEach(f => f());

        if (webSocket.readyState === WebSocket.OPEN) {
            messages.forEach(m => {
                webSocket.send(m);
            });
        }
    }
}, GAMEPAD_INTERVAL);
