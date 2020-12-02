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
            sendWS({ controller: "mp3", command: button.id });
        };
    });

    // arm dial
    document.querySelectorAll(".arm-slider").forEach(slider => {
        slider.addEventListener("change", () => {
            sendWS({ controller: "arm", command: "angle", angle: parseInt(slider.value), servo: slider.id });
        });
    });

    // leds dial
    document.querySelectorAll(".leds").forEach(button => {
        button.onclick = () => {
            sendWS({ controller: "leds", command: button.id });
        };
    });

    document.querySelector("#set_interval").addEventListener("change", event => {
        sendWS({ controller: "leds", command: event.target.id, interval: parseInt(event.target.value) });
    });

    document.querySelector("#set_custom_color").addEventListener("change", (event) => {
        const hexColor = event.target.value.substring(1);

        const RGBHex = hexColor.match(/.{1,2}/g);
        const RGB = [
            parseInt(RGBHex[0], 16),
            parseInt(RGBHex[1], 16),
            parseInt(RGBHex[2], 16),
        ];
        sendWS({ controller: "leds", command: "set_custom_color", colors: [RGB[0], RGB[1], RGB[2]] });
    }, false);

    // engines dial
    document.querySelector("#speed").addEventListener("change", event => {
        sendWS({ controller: "engines", command: event.target.id, engines: "both", speed: parseInt(event.target.value) })
    });

    document.querySelector("#forward").ontouchstart = (event) => {
        sendWS({ controller: "engines", command: event.target.id, engines: "both" })
    };

    document.querySelector("#backward").ontouchstart = (event) => {
        sendWS({ controller: "engines", command: event.target.id, engines: "both" })
    };

    document.querySelector("#rotate_left").ontouchstart = (event) => {
        sendWS({ controller: "engines", command: "rotate", engines: "left" })
    };

    document.querySelector("#rotate_right").ontouchstart = (event) => {
        sendWS({ controller: "engines", command: "rotate", engines: "right" })
    };

    document.querySelectorAll(".arrow-button").forEach(button => {

    });
});
