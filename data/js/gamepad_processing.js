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
        const gamepad = gamepads[0]
        const gamepadState = {
            axes: gamepad.axes,
            buttons: [
                gamepad.buttons[0].pressed,
                gamepad.buttons[1].pressed,
                gamepad.buttons[2].pressed,
                gamepad.buttons[3].pressed,
                gamepad.buttons[4].pressed,
                gamepad.buttons[5].pressed,
                gamepad.buttons[6].pressed,
                gamepad.buttons[7].pressed,
                gamepad.buttons[8].pressed,
                gamepad.buttons[9].pressed,
                gamepad.buttons[10].pressed,
                gamepad.buttons[11].pressed,
                gamepad.buttons[12].pressed,
                gamepad.buttons[13].pressed,
                gamepad.buttons[14].pressed,
                gamepad.buttons[15].pressed
            ]
        };
        return gamepadState;
    }
    return null;
}

export { getGamepadInfo , processGamepad };