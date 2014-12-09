/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var car = [];

function Cars() {
    var uniforms = {
        ambient: {type: "c", value: new THREE.Color(0x2E0303)},
        diffuse: {type: "c", value: new THREE.Color(0x990A0A)},
        spec: {type: "c", value: new THREE.Color(0xBAA0A0)},
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

    var geometry = new THREE.BoxGeometry(0.7, 0.5, 0.25);
    var TorusGeometry = new THREE.TorusGeometry(0.1, 0.05, 32, 32);

    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});
    var materialTorus = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniformsTorus});

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            this.car = new THREE.Mesh(geometry, material);
            this.car.position.x = -2 + 2 * j;
            this.car.position.y = -2 - 2 * i;
            this.car.position.z = 1.3;
            scene.add(this.car);
            car.push(this.car);
            
            for (k = 0; k < 2; k++) {
                for (l = 0; l < 2; l++) {
                    this.wheel = new THREE.Mesh(TorusGeometry, materialTorus);
                    this.wheel.rotation.x += 45;
                    this.wheel.position.x = this.car.position.x - 0.3 + 0.55 * k;
                    this.wheel.position.y = this.car.position.y + 0.25 - 0.5* l;
                    this.wheel.position.z = 1.25;
                    scene.add(this.wheel);
                    car.push(this.wheel);
                }
            }
        }
    }
}

function updateCars() {
    for(i = 0; i < car.length; i++){
        if(car[i].position.x > -6.0)
            car[i].position.x -= 0.02;
        else
            car[i].position.x = 6.0;
    }
    
}

function carBoundBox(obj){
 
    var xMin = obj.position.x - 0.35;
    var xMax = obj.position.x + 0.35;
    var yMin = obj.position.y - 0.25;
    var yMax = obj.position.y + 0.25;
    var min = new THREE.Vector2(xMin, yMin);
    var max = new THREE.Vector2(xMax, yMax);
    
    return new THREE.Box2(min, max);
}

function allCars(){
    
    return car;
    
}
