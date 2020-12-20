import { sendWS, sendWSMany, setOnRecive } from "./websocket.js";
import { getGamepadInfo, processGamepad } from "./gamepad_processing.js";
import { DEFAULT_CONFIG } from "./configs.js";

document.addEventListener("DOMContentLoaded", () => {

    const GAMEPAD_INTERVAL = 33;

    setInterval(function () {
        let gamepadState = getGamepadInfo();
        if (gamepadState) {
            let messages;
            let functions;

            [messages, functions] = processGamepad(gamepadState, DEFAULT_CONFIG);
            functions.forEach(f => f());
            messages.forEach(m => {
                sendWS(m);
            });
        }
    }, GAMEPAD_INTERVAL);

    // main menu
    document.querySelector("#button-music").onclick = () => {
        document.querySelector("#dial-music").classList.remove("hidden");
    };
    document.querySelector("#button-arm").onclick = () => {
        document.querySelector("#dial-arm").classList.remove("hidden");
    };
    document.querySelector("#button-engines").onclick = () => {
        document.querySelector("#dial-engines").classList.remove("hidden");
    };
    document.querySelector("#button-leds").onclick = () => {
        document.querySelector("#dial-leds").classList.remove("hidden");
    };
    document.querySelector("#button-sd").onclick = () => {
        document.querySelector("#dial-sd").classList.remove("hidden");
    };

    // close
    document.querySelectorAll(".dial-exit").forEach(button => {
        button.onclick = () => {
            document.querySelectorAll(".dial").forEach(dial => dial.classList.add("hidden"));
        };
    });

    // all sliders
    document.querySelectorAll("input[type='range']").forEach(slider => {
        slider.addEventListener("input", () => {
            slider.previousElementSibling.innerHTML = slider.value;
        }, false);
    });

    // music dial
    document.querySelectorAll(".mp3").forEach(button => {
        button.onclick = () => {
            sendWS({ controller: "mp3", command: button.dataset.command });
        };
    });

    document.getElementById("volume-slider").addEventListener("change", event => {
        sendWS({ controller: "mp3", command: event.target.dataset.command, volume: parseInt(event.target.value) });
    });

    // arm dial
    document.querySelectorAll(".arm-slider").forEach(slider => {
        slider.addEventListener("change", () => {
            sendWS({ controller: "arm", command: "angle", angle: parseInt(slider.value), servo: slider.dataset.servo });
        });
    });

    // leds dial
    document.querySelectorAll(".leds").forEach(button => {
        button.onclick = () => {
            sendWS({ controller: "leds", command: button.dataset.command });
        };
    });

    document.querySelector("#eurobeat").onclick = () => {
        sendWSMany([
            { controller: "leds", command: "color_length", length: 0 },
            { controller: "leds", command: "repetitions", repetitions: 0 },
            { controller: "leds", command: "eurobeat" }
        ])
    };

    document.querySelector("#random").onclick = () => {
        sendWSMany([
            { controller: "leds", command: "color_length", length: 0 },
            { controller: "leds", command: "repetitions", repetitions: 0 },
            { controller: "leds", command: "random" }
        ])
    };

    document.querySelector("#red-white").onclick = () => {
        sendWSMany([
            { controller: "leds", command: "custom_color", colors: [255, 0, 0], index: 0 },
            { controller: "leds", command: "custom_color", colors: [255, 255, 255], index: 1 },
            { controller: "leds", command: "repetitions", repetitions: 2 },
            { controller: "leds", command: "color_length", length: 10 },
            { controller: "leds", command: "custom" }
        ])
    };

    document.querySelector("#blue-yellow").onclick = () => {
        sendWSMany([
            { controller: "leds", command: "custom_color", colors: [255, 255, 0], index: 0 },
            { controller: "leds", command: "custom_color", colors: [0, 0, 255], index: 1 },
            { controller: "leds", command: "repetitions", repetitions: 2 },
            { controller: "leds", command: "color_length", length: 10 },
            { controller: "leds", command: "custom" }
        ])
    };

    document.querySelector("#update-interval-slider").addEventListener("change", event => {
        sendWS({ controller: "leds", command: event.target.dataset.command, interval: parseInt(event.target.value) });
    });

    document.querySelector("#brightness-slider").addEventListener("change", event => {
        sendWS({ controller: "leds", command: event.target.dataset.command, brightness: parseInt(event.target.value) });
    });

    document.querySelector("#length-slider").addEventListener("change", event => {
        sendWS({ controller: "leds", command: event.target.dataset.command, length: parseInt(event.target.value) });
    });

    document.querySelector("#custom").addEventListener("change", (event) => {
        const hexColor = event.target.value.substring(1);

        const RGBHex = hexColor.match(/.{1,2}/g);
        const RGB = [
            parseInt(RGBHex[0], 16),
            parseInt(RGBHex[1], 16),
            parseInt(RGBHex[2], 16),
        ];
        sendWSMany([
            { controller: "leds", command: "custom_color", colors: [RGB[0], RGB[1], RGB[2]], index: 0 },
            { controller: "leds", command: "repetitions", repetitions: 1 },
            { controller: "leds", command: "custom" }
        ]);
    }, false);

    // engines dial
    document.querySelectorAll(".engines-slider").forEach(slider => {
        slider.onchange = event => {
            sendWS({ controller: "engines", command: slider.dataset.command, engine: slider.dataset.engine, speed: parseInt(event.target.value) })
        };
    });

    // SD dial
    document.querySelector("#execute-sd").onclick = (event) => {
        const textInput = document.getElementById("file");
        const fileName = "/" + textInput.value;
        textInput.value = "";
        sendWS({ controller: "sd", command: "execute", file: fileName });
    };

    enableArrows();

    window.addEventListener("gamepadconnected", (e) => {
        console.log(navigator.getGamepads()[e.gamepad.index]);
        disableArrows();
    });

    window.addEventListener("gamepaddisconnected", (e) => {
        console.log("Gamepad disconnected from index %d: %s", e.gamepad.index, e.gamepad.id);
        enableArrows();
    });

    setOnRecive(updateGUIFromWebsocket);
});

function updateGUIFromWebsocket(message) {
    const data = JSON.parse(message.data)
    console.log(data);

    data.forEach(json => {
        switch (json.name) {
            case "arm":
                const armSliders = Array.from(document.querySelectorAll(".arm-slider"));
                json.data.forEach(servoData => {
                    const slider = armSliders.find(elem => elem.dataset.servo === servoData.servo);
                    if (slider != null) {
                        slider.min = servoData.min;
                        slider.max = servoData.max;
                        slider.value = servoData.angle;
                        slider.previousElementSibling.innerHTML = slider.value;
                    } else {
                        console.error(`Couls not find slider with servo ${servoData.servo}`);
                    }
                });
                break;
            case "engines":
                const enginesSliders = Array.from(document.querySelectorAll(".engines-slider"));
                json.data.forEach(engine => {
                    const slider = enginesSliders.find(elem => elem.dataset.engine === engine.engine);
                    if (slider != null) {
                        slider.min = engine.min;
                        slider.max = engine.max;
                        slider.value = engine.speed;
                        updateBullet(slider);
                    } else {
                        console.error(`No such slider: ${engine.engine}`);
                    }
                });
                break;
            case "leds":
                const intervalSlider = document.querySelector("#update-interval-slider");
                intervalSlider.value = json.data.interval;
                const brightnessSlider = document.querySelector("#brightness-slider");
                brightnessSlider.value = json.data.brightness;
                const lengthSlider = document.querySelector("#length-slider");
                lengthSlider.max = json.data.colors.length;
                lengthSlider.value = lengthSlider.max;
                updateBullet(intervalSlider);
                updateBullet(brightnessSlider);
                updateBullet(lengthSlider);
                break;
            case "mp3":
                const slider = document.querySelector("#volume-slider");
                slider.value = json.data.volume;
                updateBullet(slider);
        }
    });
}

function enableArrows() {

    let stack = [];

    document.querySelectorAll(".arrow-button").forEach(button => {
        button.classList.remove('disabled-arrow-button');
        button.ontouchend = () => {
            stack = stack.filter(item => item != button);
            if (stack.length > 0) {
                last = stack[stack.length - 1];
                sendWS({ controller: "engines", command: last.dataset.command, engine: last.dataset.engine });
            }
            else {
                sendWS({ controller: "engines", command: "stop", engine: "both" });
            }
        };
        button.ontouchstart = () => {
            sendWS({ controller: "engines", command: button.dataset.command, engine: button.dataset.engine });
            stack.push(button);
        };
    });
}

function disableArrows() {
    sendWS({ controller: "engines", command: "stop", engine: "both" });
    document.querySelectorAll(".arrow-button").forEach(button => {
        button.ontouchstart = null;
        button.ontouchend = null;
        button.classList.add('disabled-arrow-button');
    });
}

function updateBullet(slider) {
    slider.previousElementSibling.innerHTML = slider.value;
}