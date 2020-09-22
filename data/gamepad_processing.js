import commands from './tank_commands.js'
import './gamepad.js'
import GAMEPAD from './gamepad.js';

const PIVOT = 0.5;

class AxisPositionBehaviour {
    constructor(index, onPositive, onIdle, onNegative) {
        this.index = index;
        this.onPositive = onPositive;
        this.onIdle = onIdle;
        this.onNegative = onNegative;
    }
}

class AxisNumericBehaviour {
    // function needs to take a number between -1 and 1 and process it
    // returns string
    constructor(index, callback) {
        this.index = index;
        this.fun = callback;
    }
}

class ButtonBehaviour {
    constructor(index, onPressed, onReleased) {
        this.index = index;
        this.onPressed = onPressed;
        this.onReleased = onReleased;
    }
}

class ButtonCombination {
    constructor(indexes, combination, onPressed, onReleased) {
        this.indexes = indexes;
        this.combination = combination;
        this.onPressed = onPressed;
        this.onReleased = onReleased;
    }
}

// config 
// needs to have 3 arrays
// 1st axes
// 2nd buttons
// 3rd combinations

const debugMessages = {
    NEGATIVE: "NEGATIVE",
    POSITIVE: "POSITIVE",
    IDLE: "IDLE",
    PRESSED: "PRESSED",
    RELEASED: "RELEASED",
}

const DEFAULT_CONFIG = {
    axesPosition: [new AxisPositionBehaviour(GAMEPAD.AXIS_LEFT_VER, debugMessages.POSITIVE, debugMessages.IDLE, debugMessages.NEGATIVE)],
    axesNumeric: [],
    buttons: [new ButtonBehaviour(GAMEPAD.Y, debugMessages.PRESSED, debugMessages.RELEASED)],
    combinations: []
};

// code
let currentConfig = DEFAULT_CONFIG;
let previous = null;

function process_gamepad(current) {
    // array of strings
    let messages = [];
    // console.log(gamepad);
    // console.log(previous);

    if (previous !== null) {

        currentConfig.axesPosition.forEach(function(positionBeh) {
            let currentPos = current.axes[positionBeh.index];
            let previousPos = previous.axes[positionBeh.index];

            // console.log(currentPos);
            // console.log(previousPos);

            // console.log(-PIVOT <= currentPos <= PIVOT);

            if (currentPos < -PIVOT && previousPos >= -PIVOT && positionBeh.onNegative !== null)
                messages.push(positionBeh.onNegative);

            if (currentPos > PIVOT && previousPos <= PIVOT && positionBeh.onPositive !== null)
                messages.push(positionBeh.onPositive);

            if (-PIVOT <= currentPos && currentPos <= PIVOT && (previousPos > PIVOT || previousPos < -PIVOT) && positionBeh.onIdle !== null) {
                messages.push(positionBeh.onIdle);
            }
        });

        currentConfig.axesNumeric.forEach(function(numericBeh) {
            console.log(numericBeh);
        });

        currentConfig.buttons.forEach(function(buttonBeh) {
            let isCurrentPressed = current.buttons[buttonBeh.index];
            let isPreviousPressed = previous.buttons[buttonBeh.index];

            // if previously button wasn't pressed, but current is -> behavious onPressed
            if (!isPreviousPressed && isCurrentPressed && buttonBeh.onPressed !== null)
                messages.push(buttonBeh.onPressed);

            // if previously button was pressed, but cureen isn't -> behavious onPressed
            if (isPreviousPressed && !isCurrentPressed && buttonBeh.onReleased !== null)
                messages.push(buttonBeh.onReleased);

            // console.log(buttonBeh);
        });

        currentConfig.combinations.forEach(function(combination) {
            console.log(combination);
        });
    }
    // save gamepad state
    previous = current;

    // array of commands
    return messages;
}

function get_gamepad_info() {
    const gamepads = navigator.getGamepads();
    if (gamepads[0]) {
        const gamepadState = {
            axes: gamepads[0].axes,
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

export { get_gamepad_info, process_gamepad }