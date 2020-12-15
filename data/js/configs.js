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

// config 
// needs to have 4 arrays
// 1st axesPosition
// 2nd axesNumeric
// 3rd buttons
// 4th combinations
const DEFAULT_CONFIG = {
    axesPosition: [
        new AxisPositionEvent(GAMEPAD.AXIS_LEFT_HOR, 0.7,
            { controller: "arm", command: "minus", servo: "base" }, null,
            { controller: "arm", command: "stop", servo: "base" }, null,
            { controller: "arm", command: "plus", servo: "base" }, null),

        new AxisPositionEvent(GAMEPAD.AXIS_LEFT_VER, 0.7,
            { controller: "arm", command: "plus", servo: "shoulder" }, null,
            { controller: "arm", command: "stop", servo: "shoulder" }, null,
            { controller: "arm", command: "minus", servo: "shoulder" }, null),

        new AxisPositionEvent(GAMEPAD.AXIS_RIGHT_HOR, 0.7,
            { controller: "arm", command: "plus", servo: "rotation" }, null,
            { controller: "arm", command: "stop", servo: "rotation" }, null,
            { controller: "arm", command: "minus", servo: "rotation" }, null),

        new AxisPositionEvent(GAMEPAD.AXIS_RIGHT_VER, 0.7,
            { controller: "arm", command: "plus", servo: "claw" }, null,
            { controller: "arm", command: "stop", servo: "claw" }, null,
            { controller: "arm", command: "minus", servo: "claw" }, null),
    ],
    axesNumeric: [],
    buttons: [
        new ButtonBehaviour(GAMEPAD.LT, { controller: "engines", command: "forward", engine: "left" }, null, null, null),
        new ButtonBehaviour(GAMEPAD.LB, { controller: "engines", command: "backward", engine: "left" }, null, null, null),
        new ButtonBehaviour(GAMEPAD.RT, { controller: "engines", command: "forward", engine: "right" }, null, null, null),
        new ButtonBehaviour(GAMEPAD.RB, { controller: "engines", command: "backward", engine: "right" }, null, null, null),
        new ButtonBehaviour(GAMEPAD.A, { controller: "arm", command: "minus", servo: "elbow" }, null, null, null),
        new ButtonBehaviour(GAMEPAD.X, { controller: "arm", command: "plus", servo: "elbow" }, null, null, null),
        new ButtonBehaviour(GAMEPAD.Y, { controller: "arm", command: "plus", servo: "wrist" }, null, null, null),
        new ButtonBehaviour(GAMEPAD.B, { controller: "arm", command: "minus", servo: "wrist" }, null, null, null),
    ],
    combinations: [
        new ButtonCombination([
            [GAMEPAD.LT, false],
            [GAMEPAD.LB, false]
        ], {controller: "engines", command: "stop", engine: "left"}, null, null, null),

        new ButtonCombination([
            [GAMEPAD.RT, false],
            [GAMEPAD.RB, false]
        ], { controller: "engines", command: "stop", engine: "right" }, null, null, null),

        new ButtonCombination([
            [GAMEPAD.X, false],
            [GAMEPAD.A, false]
        ], { controller: "arm", command: "stop", servo: "elbow" }, null, null, null),

        new ButtonCombination([
            [GAMEPAD.Y, false],
            [GAMEPAD.B, false]
        ], { controller: "arm", command: "stop", servo: "wrist" }, null, null, null)
    ]
};

export { DEFAULT_CONFIG };