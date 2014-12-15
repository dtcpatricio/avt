/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


function RiverSides() {
    var uniforms = {
        ambient: {type: "c", value: new THREE.Color(0x003300)},
        diffuse: {type: "c", value: new THREE.Color(0x198219)},
        spec: {type: "c", value: new THREE.Color(0x739673)},
        shininess: {type: "f", value: 32},
        billboard: {type: "f", value: 0.0},
        spotDir: {type: "v3", value: sDir},
        spotPos: {type: "v3", value: sPos},
        night: {type: "v3", value: nightMode},
        texMode: {type: "f", value: 0.0}
    };

    var geometry = new THREE.BoxGeometry(10, 1, 2);
    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});

    this.rs1 = new THREE.Mesh(geometry, material);
    this.rs2 = new THREE.Mesh(geometry, material);

    this.rs1.position.y = 0;
    this.rs2.position.y = 5;

    scene.add(this.rs1);
    scene.add(this.rs2);
}