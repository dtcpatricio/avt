/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var log = [];

function TimberLog() {
    var uniforms = {
        ambient: {type: "c", value: new THREE.Color(0x33210D)},
        diffuse: {type: "c", value: new THREE.Color(0xB26E1A)},
        spec: {type: "c", value: new THREE.Color(0x66452B)},
        shininess: {type: "f", value: 20},
        texMode: {type: "f", value: 0.0}
    };

    var geometry = new THREE.CylinderGeometry(0.4, 0.4, 1.5, 32);
    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});
    
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            this.log = new THREE.Mesh(geometry, material);
            this.log.rotation.z -= 4.7;
            this.log.position.x = -3 + 3 * j + i;
            this.log.position.y = 3.2 - 1.4 * i;
            this.log.position.z = 1;
            scene.add(this.log);
            log.push(this.log);
            
        }
    }
    
}

function updateLogs() {
    for(i = 0; i < log.length; i++){
        if(log[i].position.x > -6.0)
            log[i].position.x -= 0.04;
        else
            log[i].position.x = 6.0;
    }
}

function logBoundBox(obj){
 
    var xMin = obj.position.x - 0.6;
    var xMax = obj.position.x + 0.6;
    var yMin = obj.position.y - 0.2;
    var yMax = obj.position.y + 0.2;
    var min = new THREE.Vector2(xMin, yMin);
    var max = new THREE.Vector2(xMax, yMax);
    
    return new THREE.Box2(min, max);
}

function allLogs(){
    return log;
}

