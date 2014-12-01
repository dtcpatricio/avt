/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function Road() {
    
    var roadImage = THREE.ImageUtils.loadTexture('road_texture.png');
    roadImage.magFilter = THREE.NearestFilter;
    
    var uniforms = {
        ambient: {type: "c", value: new THREE.Color(0x050505)},
        diffuse: {type: "c", value: new THREE.Color(0x030303)},
        spec: {type: "c", value: new THREE.Color(666666)},
        shininess: {type: "f", value: 32},
        texMode: {type: "f", value: 1.0},
        texture1: { type: "t", value: roadImage}
    };

    var geometry = new THREE.BoxGeometry(10, 4, 2);
    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});

    this.road = new THREE.Mesh(geometry, material);
    this.road.position.y = -2.5;

    scene.add(this.road);
}