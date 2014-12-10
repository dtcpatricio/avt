/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


var frog = [];
var left = false;
var right = false;
var up = false;
var down = false;
var delta = 0.03;

function Frog() {

    uniforms = {
        ambient: {type: "c", value: new THREE.Color(0x052D05)},
        diffuse: {type: "c", value: new THREE.Color(0x139913)},
        spec: {type: "c", value: new THREE.Color(0xA1B7A1)},
        shininess: {type: "f", value: 20},
        billboard: {type: "f", value: 0.0},
        night: {type: "v3", value: nightMode},
        spotDir: {type: "v3", value: sDir},
        spotPos: {type: "v3", value: sPos},
        texMode: {type: "f", value: 0.0}
    };

    var geometry = new THREE.SphereGeometry(0.1, 32, 32);
    var bodyGeometry = new THREE.BoxGeometry(0.25, 0.25, 0.25);
    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});

    this.body = new THREE.Mesh(bodyGeometry, material);

    this.body.position.y = -5;
    this.body.position.z = 1.25;
    frog.push(this.body);
    scene.add(this.body);

    this.head = new THREE.Mesh(geometry, material);

    this.head.position.x = this.body.position.x;
    this.head.position.y = this.body.position.y;
    this.head.position.z = this.body.position.z + 0.2;
    frog.push(this.head);
    scene.add(this.head);

}

function frogBoundBox() {

   var xMin = frog[0].position.x - 0.125;
   var xMax = frog[0].position.x + 0.125;
   var yMin = frog[0].position.y - 0.125;
   var yMax = frog[0].position.y + 0.125;
   var min = new THREE.Vector2(xMin, yMin);
   var max = new THREE.Vector2(xMax, yMax);
    
   return new THREE.Box2(min, max);
}

function updateFrog() {

    if (left) {
        if (frog[0].position.x - delta > -4.87) {
            frog[0].position.x -= delta;
            frog[1].position.x -= delta;
            sPos.x += delta;
        }
    }
    if (right) {
        if (frog[0].position.x + delta < 4.87) {
            frog[0].position.x += delta;
            frog[1].position.x += delta;
            sPos.x -= delta;
        }
    }
    if (up) {
        if (frog[0].position.y + delta < 5.3) {
            frog[0].position.y += delta;
            frog[1].position.y += delta;
            sPos.y -= delta;
        }
    }
    if (down) {
        //if (frog[0].position.y - delta > -5.5) {
            frog[0].position.y -= delta;
            frog[1].position.y -= delta;
            sPos.y += delta;
        //}
    }
}

function frogPosition() {
    return frog[0].position;
}

function initialPosition() {
    frog[0].position.x = 0.0;
    frog[0].position.y = -5;
    frog[1].position.x = 0.0;
    frog[1].position.y = -5;
    sPos.x = 0.0;
    sPos.y = 7.5;
}

function setPosition(value) {
    if (frog[0].position.x - delta > -4.75){
        frog[0].position.x -= value;
        frog[1].position.x -= value;
        sPos.x += value;
    }
}


