#ifndef WEBPAGE_H
#define WEBPAGE_H

static const char webpage[] =
R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>

<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>ESP32 RC Car</title>

<style>

body{
    margin:0;
    font-family:Arial,Helvetica,sans-serif;
    background:#202124;
    color:white;
    text-align:center;
}

h1{
    margin-top:25px;
}

.container{
    width:100%;
    margin:auto;
}

button{
    width:120px;
    height:70px;
    font-size:22px;
    margin:8px;
    border:none;
    border-radius:12px;
    background:#2196F3;
    color:white;
}

button:hover{
    background:#1565C0;
}

.stop{
    background:#d32f2f;
}

.slider{
    width:300px;
}

</style>

</head>

<body>

<h1>🚗 ESP32 RC CAR</h1>

<div class="container">

<button
onmousedown="pressForward()"
onmouseup="releaseForward()"
ontouchstart="event.preventDefault(); pressForward();"
ontouchend="event.preventDefault(); releaseForward();">
▲
</button>

<br>

<button
onmousedown="pressLeft()"
onmouseup="releaseLeft()"
ontouchstart="event.preventDefault(); pressLeft();"
ontouchend="event.preventDefault(); releaseLeft();">
◀
</button>

<button
class="stop"
onclick="send('/stop')">
■
</button>

<button
onmousedown="pressRight()"
onmouseup="releaseRight()"
ontouchstart="event.preventDefault(); pressRight();"
ontouchend="event.preventDefault(); releaseRight();">
▶
</button>

<br>

<button
onmousedown="pressBackward()"
onmouseup="releaseBackward()"
ontouchstart="event.preventDefault(); pressBackward();"
ontouchend="event.preventDefault(); releaseBackward();">
▼
</button>

<br><br>

<h2>Speed</h2>

<input
class="slider"
type="range"
min="0"
max="100"
value="50"
id="speed"
oninput="updateSpeed()">

<p id="speedValue">
50 %
</p>

<h2>Battery</h2>

<p id="battery">
Reading...
</p>

</div>

<script>

function send(cmd)
{
    fetch(cmd);
}

function pressForward()
{
    send("/forward");
}

function releaseForward()
{
    send("/stop");
}

function pressBackward()
{
    send("/backward");
}

function releaseBackward()
{
    send("/stop");
}

function pressLeft()
{
    send("/left");
}

function releaseLeft()
{
    send("/steerstop");
}

function pressRight()
{
    send("/right");
}

function releaseRight()
{
    send("/steerstop");
}

function updateSpeed()
{
    let value = document.getElementById("speed").value;

    document.getElementById("speedValue").innerHTML =
        value + " %";

    send("/speed?value=" + value);
}

function updateBattery()
{
    fetch("/battery")
    .then(response => response.json())
    .then(data =>
    {
        document.getElementById("battery").innerHTML =
            "🔋 " +
            data.voltage.toFixed(2) +
            " V (" +
            data.percent +
            "%)";
    })
    .catch(() =>
    {
        document.getElementById("battery").innerHTML =
            "Battery unavailable";
    });
}

setInterval(updateBattery, 1000);

updateBattery();

</script>

</body>

</html>
)rawliteral";

#endif