/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var startX;
var startY;
var xFlare;
var yFlare;
var tracking = 0.0;
var alpha = 0.0;
var alphaAux = 0.0;
var betaAux = 20.0;
var beta = 20.0;
var r = 5.25;

function handleMouseDown(e) {
    // start tracking the mouse
    var evt = (e === null ? event : e);
    var clickType = 'LEFT';
    if (evt.which) {
        if (evt.which === 3)
            clickType = 'RIGHT';
    }
    else if (evt.button) {
        if (evt.button === 2)
            clickType = 'RIGHT';
    }

    startX = e.clientX;
    startY = e.clientY;

    if (clickType === 'LEFT')
        tracking = 1.0;
    else if (clickType === 'RIGHT')
        tracking = 2.0;
}

function handleMouseUp(event) {
    if (tracking === 1.0 && camType === 3.0) {
        alpha -= event.clientX - startX;
        beta += event.clientY - startY;
    }
    else if (tracking === 2.0) {

        xFlare = startX * 3 / glutGet(GLUT_WINDOW_WIDTH);
        yFlare = startY * 3 / glutGet(GLUT_WINDOW_HEIGHT);
    }

    tracking = 0;
}


function handleMouseMove(event) {

    var deltaX = -event.clientX + startX;
    var deltaY = event.clientY - startY;
    // left mouse button: move camera
    if (tracking === 1 && camType === 3.0) {
        alphaAux = alpha + deltaX;
        betaAux = beta + deltaY;
        if (betaAux > 85.0)
            betaAux = 85.0;
        else if (betaAux < -85.0)
            betaAux = -85.0;
    }

    // right mouse button: zoom
    else if (tracking === 2) {

        xFlare += startX * 3.0 / glutGet(GLUT_WINDOW_WIDTH);
        yFlare += startY * 3.0 / glutGet(GLUT_WINDOW_HEIGHT);
        if (xFlare >= 3.0)
            xFlare = 3.0 - 1.0;
        if (xFlare < 0.0)
            xFlare = 0.0;
        if (yFlare >= 3.0)
            yFlare = 3.0 - 1.0;
        if (yFlare < 0.0)
            yFlare = 0.0;
    }
    updateEye();
}

function updateEye() {

    if (camType === 3.0) {
        var x = r * Math.sin(alphaAux * 3.14 / 180.0) * Math.cos(betaAux * 3.14 / 180.0);
        var z = r * Math.cos(alphaAux * 3.14 / 180.0) * Math.cos(betaAux * 3.14 / 180.0);
        var y = r * Math.sin(betaAux * 3.14 / 180.0);

        camera.position.set(frog[0].position.x + x, frog[0].position.y - 1 + y, frog[0].position.z + 1 + z);

    }
}

