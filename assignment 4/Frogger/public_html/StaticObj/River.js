/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


function River() {

    var riverImage = THREE.ImageUtils.loadTexture('water.png');
    riverImage.magFilter = THREE.NearestFilter;
    
    var uniforms = {
        ambient: {type: "c", value: new THREE.Color(0x001A0F)},
        diffuse: {type: "c", value: new THREE.Color(0x008282)},
        spec: {type: "c", value: new THREE.Color(0x808080)},
        shininess: {type: "f", value: 32},
        texMode: {type: "f", value: 1.0}, 
        texture1: { type: "t", value: riverImage}
    };

    var geometry = new THREE.BoxGeometry(10, 4, 2);
    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});
    material.depthTest = false;
    material.transparent = true;
    material.blending = THREE[ "AdditiveBlending" ];

    this.river = new THREE.Mesh(geometry, material);
    this.river.position.y = 2.5;
    scene.add(this.river);
    
    material.depthTest = true;
}
