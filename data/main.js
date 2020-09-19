// commands for driving
const FORWARD_LEFT = 'FORWARDL ';
const BACKWARD_LEFT = 'BACKWARDL ';
const FORWARD_RIGHT = 'FORWARDR ';
const BACKWARD_RIGHT = 'BACKWARDR ';
const STOP_LEFT = 'STOPL ';
const STOP_RIGHT = 'STOPR ';
// buttons associated with driving
const LB = 4;
const LF = 6;
const RB = 5;
const RF = 7;

// speed buttons and commands
const FASTER = 'FASTER ';
const SLOWER = 'SLOWER ';
const STEADY = 'STEADY ';

const SPEED_UP_BUTTON = 12;
const SPEED_DOWN_BUTTON = 13;

// MEGA commands
const MOVE = 'MOVE_';

const MP3 = 'MP3_';
const PLAY_BUTTON = 9;
const STOP_BUTTON = 8;

const PIVOT = 0.5;
const JOYSTICKS = ['ELBOW1_', 'ELBOW2_', 'WRIST_', 'CLAW_'];
const BUTTONS = [{ name: 'SHOULDER_', forward: 0, backward: 3 }, { name: 'BASE_', forward: 2, backward: 1 }];

const SERVO_BACKWARD = -1;
const SERVO_STOP = 0;
const SERVO_FORAWRD = 1;

// code
let previous = null;

function processGamepad(gamepad) {
    let message = "";

    if (previous !== null) {

        let gb = gamepad.buttons;
        let pb = previous.buttons;

        // backward
        if (gb[LB] && !pb[LB])
            message += BACKWARD_LEFT;

        if (gb[RB] && !pb[RB])
            message += BACKWARD_RIGHT;

        // forward
        if (gb[LF] && !pb[LF])
            message += FORWARD_LEFT;

        if (gb[RF] && !pb[RF])
            message += FORWARD_RIGHT;

        // stop if both are released
        if (!gb[LB] && !gb[LF] && (pb[LB] || pb[LF]))
            message += STOP_LEFT;

        if (!gb[RB] && !gb[RF] && (pb[RB] || pb[RF]))
            message += STOP_RIGHT;

        // change speed if button pressed
        if (gb[SPEED_UP_BUTTON] && !pb[SPEED_UP_BUTTON]) {
            message += FASTER;
        } else if (gb[SPEED_DOWN_BUTTON] && !pb[SPEED_DOWN_BUTTON]) {
            message += SLOWER;
        } else if (!gb[SPEED_DOWN_BUTTON] && !gb[SPEED_UP_BUTTON] && (pb[SPEED_UP_BUTTON] || pb[SPEED_DOWN_BUTTON]))
            message += STEADY;

        // joysticks
        for (let i = 0; i < JOYSTICKS.length; i++) {
            if (gamepad.axes[i] > PIVOT && previous.axes[i] < PIVOT) {
                message += MOVE + JOYSTICKS[i] + SERVO_FORAWRD + ' ';
            } else if (gamepad.axes[i] < -PIVOT && previous.axes[i] > -PIVOT) {
                message += MOVE + JOYSTICKS[i] + SERVO_BACKWARD + ' ';
            } else if (gamepad.axes[i] < PIVOT && gamepad.axes[i] > -PIVOT && (previous.axes[i] > PIVOT || previous.axes[i] < -PIVOT)) {
                message += MOVE + JOYSTICKS[i] + SERVO_STOP + ' ';
            }
        }
        // and YXAB buttons
        for (let i = 0; i < BUTTONS.length; i++) {
            let f = BUTTONS[i].forward;
            let b = BUTTONS[i].backward;
            name = BUTTONS[i].name;
            if (gb[f] && !pb[f]) {
                message += MOVE + name + SERVO_FORAWRD + ' ';
            } else if (gb[b] && !pb[b]) {
                message += MOVE + name + SERVO_BACKWARD + ' ';
            } else if (!gb[f] && !gb[b] && (pb[f] || pb[b])) {
                message += MOVE + name + SERVO_STOP + ' ';
            }
        }

        // mp3 shit
        if (gb[PLAY_BUTTON] && !pb[PLAY_BUTTON])
            message += MOVE + MP3 + '1_0_0';

        if (gb[STOP_BUTTON] && !pb[STOP_BUTTON])
            message += MOVE + MP3 + '22_0_0';
    }

    // save gamepad state
    previous = gamepad;

    if (message === "") {
        message = null;
    } else {
        message = message.trim().split(' ').map(e => ({ payload: e.replace(/_/g, ' ') }));
    }

    // array of commands
    return message;
}

function getGamepadInfo() {
    const gamepads = navigator.getGamepads();
    if (gamepads[0]) {
        const gamepadState = {
            axes: [
                gamepads[0].axes[0].toFixed(2),
                gamepads[0].axes[1].toFixed(2),
                gamepads[0].axes[2].toFixed(2),
                gamepads[0].axes[3].toFixed(2),
            ],
            buttons: [
                gamepads[0].buttons[0].pressed,
                gamepads[0].buttons[1].pressed,
                gamepads[0].buttons[2].pressed,
                gamepads[0].buttons[3].pressed,
                gamepads[0].buttons[4].pressed,
                gamepads[0].buttons[5].pressed,
                gamepads[0].buttons[6].pressed,
                gamepads[0].buttons[7].pressed,
                gamepads[0].buttons[8].pressed,
                gamepads[0].buttons[9].pressed,
                gamepads[0].buttons[10].pressed,
                gamepads[0].buttons[11].pressed,
                gamepads[0].buttons[12].pressed,
                gamepads[0].buttons[13].pressed,
                gamepads[0].buttons[14].pressed,
                gamepads[0].buttons[15].pressed
            ]
        };
        return gamepadState;
    }
    return null;
}

const webSocketAddress = "ws://192.168.4.1/ws"
let webSocket = new WebSocket(webSocketAddress);

// sendint gamepad info every 33 millis
setInterval(function() {
    if (webSocket.readyState === WebSocket.OPEN) {
        let gamepadState = getGamepadInfo();
        if (gamepadState) {
            let messages = processGamepad(gamepadState);
            if (messages) {
                console.log(messages)
                messages.forEach(element => {
                    webSocket.send(element);
                });
            }
        }
    } else {
        console.log("Websocket is not connected")
    }
}, 33);