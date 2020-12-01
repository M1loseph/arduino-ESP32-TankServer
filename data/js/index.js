import { sendWS } from "./websocket.js";
import { getGamepadInfo, processGamepad } from "./gamepad_processing.js";
import { DEFAULT_CONFIG } from "./configs.js";

document.addEventListener("DOMContentLoaded", () => {
    // checking gamepad info every few milliseconds
    const GAMEPAD_INTERVAL = 33;
    // logging function to see if gamepad has been conneted
    window.addEventListener("gamepadconnected", function (e) {
        console.log(navigator.getGamepads()[e.gamepad.index]);
    });

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
    document.querySelector("#button-led").onclick = () => {
        document.querySelector("#dial-led").classList.remove("hidden");
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

    // music dial
    document.querySelectorAll(".mp3").forEach((elem) => {
        elem.onclick = () => {
            sendWS({ controller: "mp3", command: elem.id });
        }
    });

    // arm dial
    let rangeBullet = document.getElementById("base-bullet");

    document.querySelectorAll('.arm-slider').forEach(slider => {
        slider.addEventListener("input", () => {
            slider.previousElementSibling.innerHTML = slider.value;
        }, false);

        slider.addEventListener("touchend", () => {
            sendWS({ controller: "arm", command: "angle", angle: slider.value, servo: slider.id })
        });
    });

    function sendColor() {
        let hexColor = ledColorPicker.value;
        // remove 1st char
        hexColor = hexColor.substring(1);

        let RGBHex = hexColor.match(/.{1,2}/g);
        let RGB = [
            parseInt(RGBHex[0], 16),
            parseInt(RGBHex[1], 16),
            parseInt(RGBHex[2], 16),
        ];
        sendWS({ controller: "leds", command: "set_custom_color", colors: [RGB[0], RGB[1], RGB[2]] });
    }

    function sendSpeed() {
        sendWS({ controller: "engines", command: "speed", speed: speedSlider.value });
    }

    function sendVolume() {
        sendWS({ controller: "mp3", command: "set_volume", "volume": volumeSlider.value });
    }

    function sendBrightness() {
        sendWS({ controller: "leds", command: "set_brightness", brightness: brightnessSlider.value });
    }

    function sendInterval() {
        sendWS({ controller: "leds", command: "set_interval", interval: intervalSlider.value });
    }
});
