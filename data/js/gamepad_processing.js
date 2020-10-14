let previous = null;

function processGamepad(current, currentConfig) {

    let messages = [];
    let functions = [];

    if (previous !== null) {

        currentConfig.axesPosition.forEach(positionBeh => {

            const currentPos = current.axes[positionBeh.index];
            const previousPos = previous.axes[positionBeh.index];
            const pivot = positionBeh.pivot;

            if (currentPos < -pivot && previousPos >= -pivot) {
                messages.push(positionBeh.onNegativeString);
                functions.push(positionBeh.onNegativeFun);
            } else if (currentPos > pivot && previousPos <= pivot) {
                messages.push(positionBeh.onPositiveString);
                functions.push(positionBeh.onPositiveFun);
            } else if (-pivot <= currentPos && currentPos <= pivot && (previousPos > pivot || previousPos < -pivot)) {
                messages.push(positionBeh.onIdleString);
                functions.push(positionBeh.onIdleFun);
            }
        });

        currentConfig.axesNumeric.forEach(numericBeh => {
            // TODO => IMPLEMENT
        });

        currentConfig.buttons.forEach(buttonBeh => {

            const isCurrentPressed = current.buttons[buttonBeh.index];
            const isPreviousPressed = previous.buttons[buttonBeh.index];

            // if previously button wasn't pressed, but current is -> behavious onPressed
            // same goes for button being released
            if (!isPreviousPressed && isCurrentPressed) {
                messages.push(buttonBeh.onPressedString);
                functions.push(buttonBeh.onPressedFun)
            } else if (isPreviousPressed && !isCurrentPressed) {
                messages.push(buttonBeh.onReleasedString);
                functions.push(buttonBeh.onReleasedFun)
            }
        });

        currentConfig.combinations.forEach(combinationBeh => {

            function helper(gamepad) {
                return (acc, [index, expected]) => {
                    return acc && gamepad.buttons[index] === expected;
                }
            }

            const isCurrentCombination = combinationBeh.combination.reduce(helper(current), true);
            const isPrevCombination = combinationBeh.combination.reduce(helper(previous), true);

            if (!isPrevCombination && isCurrentCombination) {
                messages.push(combinationBeh.onPressedString);
                functions.push(combinationBeh.onPressedFun);
            } else if (isPrevCombination && !isCurrentCombination) {
                messages.push(combinationBeh.onReleasedString);
                functions.push(combinationBeh.onReleasedFun);
            }
        });
    }
    // save gamepad state
    previous = current;

    // remove nulls
    messages = messages.filter(m => m !== null);
    functions = functions.filter(f => f !== null);

    return [messages, functions];
}

function getGamepadInfo() {
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

export { getGamepadInfo , processGamepad };