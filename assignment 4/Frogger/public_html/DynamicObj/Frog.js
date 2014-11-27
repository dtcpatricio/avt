/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


var frog;
var left = false;
var right = false;
var up = false;
var down = false;

function Frog() {

    var uniforms = {
        ambient: {type: "c", value: new THREE.Color(0x052D05)},
        diffuse: {type: "c", value: new THREE.Color(0x139913)},
        spec: {type: "c", value: new THREE.Color(0xA1B7A1)},
        shininess: {type: "f", value: 20}
    };

    var geometry = new THREE.SphereGeometry(0.25, 32, 32);
    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});


    frog = new THREE.Mesh(geometry, material);
    frog.position.y = -5;
    frog.position.z = 1.25;
    scene.add(frog);
}

function updateFrog() {
    var delta = 0.03;
    if (left) {
        frog.position.x -= delta;
    }
    if (right) {
        frog.position.x += delta;
    }
    if (up) {
        frog.position.y += delta;
    }
    if (down) {
        frog.position.y -= delta;
    }

}