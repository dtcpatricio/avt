/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

function Tree() {

    var treeImage = THREE.ImageUtils.loadTexture('tree.png');
    treeImage.magFilter = THREE.NearestFilter;

    var uniforms = {
        ambient: {type: "c", value: new THREE.Color(0xFFFFFF)},
        diffuse: {type: "c", value: new THREE.Color(0x008282)},
        spec: {type: "c", value: new THREE.Color(0x808080)},
        shininess: {type: "f", value: 32},
        billboard: {type: "f", value: 1.0},
        spotDir: {type: "v3", value: sDir},
        spotPos: {type: "v3", value: sPos},
        night: {type: "v3", value: nightMode},
        texMode: {type: "f", value: 1.0},
        texture1: {type: "t", value: treeImage},
        life: {type: "f", value: 0.0}
    };

    var rectGeom = new THREE.BoxGeometry(2, 2, 0.00000000000000001);
    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});
    material.depthTest = false;
    
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            this.tree = new THREE.Mesh(rectGeom, material);
            this.tree.position.x = -1.5 + j *3;
            this.tree.position.y = .8 + i* 5;
            this.tree.position.z = 1.75;
            this.tree.rotation.x += 89;
            scene.add(this.tree);
        }
    }

    material.depthTest = true;

}
