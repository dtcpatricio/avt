/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var bus = [];

function Bus() {
    var uniforms = {
        ambient: {type: "c", value: new THREE.Color(0x543808)},
        diffuse: {type: "c", value: new THREE.Color(0xC7991C)},
        spec: {type: "c", value: new THREE.Color(0xFCF0CC)},
        shininess: {type: "f", value: 20},
        billboard: {type: "f", value: 0.0},
        spotDir: {type: "v3", value: sDir},
        spotPos: {type: "v3", value: sPos},
        night: {type: "v3", value: nightMode},
        texMode: {type: "f", value: 0.0}
    };

    var uniformsTorus = {
        ambient: {type: "c", value: new THREE.Color(0x050505)},
        diffuse: {type: "c", value: new THREE.Color(0x030303)},
        spec: {type: "c", value: new THREE.Color(0x666666)},
        shininess: {type: "f", value: 20}
    };

    var geometry = new THREE.BoxGeometry(0.9, 0.7, 0.25);
    var TorusGeometry = new THREE.TorusGeometry(0.1, 0.05, 32, 32);

    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});
    var materialTorus = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniformsTorus});

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            this.bus = new THREE.Mesh(geometry, material);
            this.bus.position.x = -3 + 2 * j;
            this.bus.position.y = -1 - 2 * i;
            this.bus.position.z = 1.3;
            scene.add(this.bus);
            bus.push(this.bus);
            for (k = 0; k < 2; k++) {
                for (l = 0; l < 2; l++) {
                    this.wheel = new THREE.Mesh(TorusGeometry, materialTorus);
                    this.wheel.rotation.x += 45;
                    this.wheel.position.x = this.bus.position.x - 0.4 + 0.8 * k;
                    this.wheel.position.y = this.bus.position.y + 0.35 - 0.7* l;
                    this.wheel.position.z = 1.25;
                    scene.add(this.wheel);
                    bus.push(this.wheel);
                }
            }
        }
    }
    
}

function updateBus() {
    for(i = 0; i < bus.length; i++){
        if(bus[i].position.x > -6.0)
            bus[i].position.x -= 0.01;
        else
            bus[i].position.x = 6.0;
    }
}

function busBoundBox(obj){
 
    var xMin = obj.position.x - 0.45;
    var xMax = obj.position.x + 0.45;
    var yMin = obj.position.y - 0.35;
    var yMax = obj.position.y + 0.35;
    var min = new THREE.Vector2(xMin, yMin);
    var max = new THREE.Vector2(xMax, yMax);
    
    return new THREE.Box2(min, max);
}

function allBus(){
    return bus;
}

