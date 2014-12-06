/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function Tunnels() {
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

    //var geometry = new THREE.BoxGeometry(3, 4, 2);
    var geometry = new THREE.CylinderGeometry(2, 2, 3, 32);
    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            this.tunnel = new THREE.Mesh(geometry, material);
            this.tunnel.rotation.z -= 4.7;
            this.tunnel.position.y = -2.5 + 5*i;
            this.tunnel.position.x = -6.5 + 13*j;
            this.tunnel.position.z = 1;
            scene.add(this.tunnel);
        }
    }
}
