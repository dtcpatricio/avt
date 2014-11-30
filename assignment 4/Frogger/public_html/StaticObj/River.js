/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var river;

function River() {

    var uniforms = {
        ambient: {type: "c", value: new THREE.Color(0x001A0F)},
        diffuse: {type: "c", value: new THREE.Color(0x008282)},
        spec: {type: "c", value: new THREE.Color(0x808080)},
        shininess: {type: "f", value: 32}
    };

    var geometry = new THREE.BoxGeometry(10, 4, 2);
    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});

    river = new THREE.Mesh(geometry, material);
    river.position.y = 2.5;
    scene.add(river);
}
