/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function RoadSide() {
    var uniforms = {
        ambient: {type: "c", value: new THREE.Color(0x333333)},
        diffuse: {type: "c", value: new THREE.Color(0x838383)},
        spec: {type: "c", value: new THREE.Color(0x838383)},
        shininess: {type: "f", value: 32},
        billboard: {type: "f", value: 0.0},
        spotDir: {type: "v3", value: sDir},
        spotPos: {type: "v3", value: sPos},
        night: {type: "v3", value: nightMode},
        texMode: {type: "f", value: 0.0},
        life: {type: "f", value: 0.0}
    };

    var geometry = new THREE.BoxGeometry(10, 1, 2);
    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});

    this.roadside = new THREE.Mesh(geometry, material);
    this.roadside.position.y = -5;
    scene.add(this.roadside);
}
