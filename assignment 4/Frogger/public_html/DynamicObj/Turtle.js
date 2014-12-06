/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var turtles = [];

function Turtles() {
    var uniforms = {
        ambient: {type: "c", value: new THREE.Color(0x543808)},
        diffuse: {type: "c", value: new THREE.Color(0xC7991C)},
        spec: {type: "c", value: new THREE.Color(0xFCF0CC)},
        shininess: {type: "f", value: 20},
        billboard: {type: "f", value: 0.0},
        spotDir: {type: "v3", value: sDir},
        spotPos: {type: "v3", value: sPos},
        night: {type: "v3", value: nightMode},
        texMode: {type: "f", value: 0.0},
        life: {type: "f", value: 0.0}
    };

    var uniformsHead = {
        ambient: {type: "c", value: new THREE.Color(0x050505)},
        diffuse: {type: "c", value: new THREE.Color(0x030303)},
        spec: {type: "c", value: new THREE.Color(0x666666)},
        shininess: {type: "f", value: 20}
    };

    var bodyGeometry = new THREE.SphereGeometry(0.35, 32, 32);
    var headGeometry = new THREE.SphereGeometry(0.2, 32, 32);
    var bodyMaterial = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});
    var headMaterial = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniformsHead});

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 1 + i; j++) {
            for (k = -3; k < 6; k += 3) {
                this.body = new THREE.Mesh(bodyGeometry, bodyMaterial);
                this.body.position.x = k + 0.85 * j + i;
                this.body.position.y = 4 - 1.5 * i;
                this.body.position.z = 1;
                scene.add(this.body);
                turtles.push(this.body);

                this.head = new THREE.Mesh(headGeometry, headMaterial);
                this.head.position.x = this.body.position.x - 0.3;
                this.head.position.y = this.body.position.y;
                this.head.position.z = this.body.position.z;
                scene.add(this.head);
                turtles.push(this.head);
            }
        }
    }
}

function turtleBoundBox(obj){
 
    var xMin = obj.position.x - 0.175;
    var xMax = obj.position.x + 0.175;
    var yMin = obj.position.y - 0.4;
    var yMax = obj.position.y + 0.4;
    var min = new THREE.Vector2(xMin, yMin);
    var max = new THREE.Vector2(xMax, yMax);
    
    return new THREE.Box2(min, max);
}

function updateTurtles() {
    for (i = 0; i < turtles.length; i++) {
        if (turtles[i].position.x > -5.5)
            turtles[i].position.x -= 0.01;
        else
            turtles[i].position.x = 5.5;
    }
}

function allTurtles(){
    return turtles;
}

