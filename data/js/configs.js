import { COMMANDS } from './tank_commands.js';

const GAMEPAD = {
    LT: 6,
    LB: 4,
    RT: 7,
    RB: 5,
    Y: 3,
    X: 2,
    A: 0,
    B: 1,
    PLAY: 9,
    BACK: 8,
    LEFT_ANALOG_BUTTON: 10,
    RIGHT_ANALOG_BUTTON: 11,
    UP: 12,
    DOWN: 13,
    LEFT: 14,
    DOWN: 15,
    AXIS_LEFT_VER: 1,
    AXIS_LEFT_HOR: 0,
    AXIS_RIGHT_HOR: 2,
    AXIS_RIGHT_VER: 3
};

class AxisPositionEvent {
    constructor(index, pivot, onPositiveString, onPositiveFun, onIdleString, onIdleFun, onNegativeString, onNegativeFun) {
        this.index = index;
        this.pivot = pivot;
        this.onPositiveString = onPositiveString;
        this.onPositiveFun = onPositiveFun;
        this.onIdleString = onIdleString;
        this.onIdleFun = onIdleFun;
        this.onNegativeString = onNegativeString;
        this.onNegativeFun = onNegativeFun;
    }
}

class AxisNumericBehaviour {
    // function needs to take a number between -1 and 1 and process it
    // returns string
    constructor(index, funToGetString, numericFun) {
        this.index = index;
        this.funToGetString = funToGetString;
        this.numericFun = numericFun;
    }
}

class ButtonBehaviour {
    constructor(index, onPressedString, onPressedFun, onReleasedString, onReleasedFun) {
        this.index = index;
        this.onPressedString = onPressedString;
        this.onPressedFun = onPressedFun;
        this.onReleasedString = onReleasedString;
        this.onReleasedFun = onReleasedFun;
    }
}

class ButtonCombination {
    constructor(combination, onPressedString, onPressedFun, onReleasedString, onReleasedFun) {
        this.combination = combination;
        this.onPressedString = onPressedString;
        this.onPressedFun = onPressedFun;
        this.onReleasedString = onReleasedString;
        this.onReleasedFun = onReleasedFun;
    }
}

const debugMessages = {
    NEGATIVE: "NEGATIVE",
    POSITIVE: "POSITIVE",
    IDLE: "IDLE",
    PRESSED: "PRESSED",
    RELEASED: "RELEASED",
    COMB_PRESSED: "COMBINATION PRESSED",
    COMB_RELEASED: "COMBINATION RELEASED"
};

// config 
// needs to have 4 arrays
// 1st axesPosition
// 2nd axesNumeric
// 2nd buttons
// 3rd combinations
const DEFAULT_CONFIG = {
    axesPosition: [new AxisPositionEvent(GAMEPAD.AXIS_LEFT_VER, 0.5, debugMessages.POSITIVE, () => console.log("POSITIVE FUN"), debugMessages.IDLE, () => console.log("IDLE FUN"), debugMessages.NEGATIVE, () => console.log("NEGATIVE FUN"))],
    axesNumeric: [],
    buttons: [new ButtonBehaviour(GAMEPAD.Y, debugMessages.PRESSED, () => console.log("PRESSED FUN!"), debugMessages.RELEASED, () => console.log("RELEASED FUN!"))],
    combinations: [new ButtonCombination([
        [GAMEPAD.X, true],
        [GAMEPAD.A, true]
    ], debugMessages.COMB_PRESSED, null, debugMessages.COMB_RELEASED, null)]
};

const DEBUG_ENGINES_CONFIG = {
    axesPosition: [],
    axesNumeric: [],
    buttons: [
        new ButtonBehaviour(GAMEPAD.LT, COMMANDS.ENGINES.FORWARD_LEFT, null, null, null),
        new ButtonBehaviour(GAMEPAD.LB, COMMANDS.ENGINES.BACKWARD_LEFT, null, null, null),
        new ButtonBehaviour(GAMEPAD.RT, COMMANDS.ENGINES.FORWARD_RIGHT, null, null, null),
        new ButtonBehaviour(GAMEPAD.RB, COMMANDS.ENGINES.BACKWARD_RIGHT, null, null, null),
    ],
    combinations: [
        new ButtonCombination([
            [GAMEPAD.LT, false],
            [GAMEPAD.LB, false]
        ], COMMANDS.ENGINES.STOP_LEFT, null, null, null),
        new ButtonCombination([
            [GAMEPAD.RT, false],
            [GAMEPAD.RB, false]
        ], COMMANDS.ENGINES.STOP_RIGHT, null, null, null)
    ]
};

export { DEFAULT_CONFIG, DEBUG_ENGINES_CONFIG };