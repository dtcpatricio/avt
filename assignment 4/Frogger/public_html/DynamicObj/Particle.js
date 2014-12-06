/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


var fireworks = [];
var speed = [];
var acel = new THREE.Vector3(0.1, 0.0, -0.15);
var lifeP = 1.0;
var fade = 0.01;

function Particle() {

    var particleImage = THREE.ImageUtils.loadTexture('tree.png');
    particleImage.magFilter = THREE.NearestFilter;
    var speedP = 0.0;
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
        texture1: {type: "t", value: particleImage},
        life: {type: "f", value: life}
    };
    
    var rectGeom = new THREE.BoxGeometry(1, 1, 0.00000000000000001);
    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});
    material.depthTest = false;
    for (i = 0; i < 100; i++) {

        this.particle = new THREE.Mesh(rectGeom, material);
        //this.particle.position.x = 0.0;
        //this.particle.position.y = 0.0;
        //this.particle.position.z = 7.0;
        this.particle.rotation.x += 89;
        fireworks.add(this.particle);
        speed.add(speedP);
        scene.add(this.particle);
    }

    material.depthTest = true;
}

function setPosition(obj, x, y, z, speed) {
    obj.position.x = x + speed;
    obj.position.y = y + speed;
    obj.position.z = z + speed;
}

function initParticles() {
    var v, theta, phi;
    for (i = 0; i < fireworks.length; i++)
    {
        v = 0.8 * frand() + 0.2;
        phi = frand() * M_PI;
        theta = 2.0 * frand() * M_PI;
        fireworks[i].position.x = 0.0;
        fireworks[i].position.y = 0.0;
        fireworks[i].position.z = 10.0;
        speed[i] = v * cos(theta) * sin(phi), v * cos(phi), v * sin(theta) * sin(phi);
        /* tom amarelado que vai ser multiplicado pela textura que varia entre branco e preto */
        //fireworks[i].setColors(new THREE.Vector3(0.882, 0.552, 0.211));
    }

}

function iterate() {

    /* Método de Euler de integração de eq. diferenciais ordinárias
     h representa o step de tempo; dv/dt = a; dx/dt = v; e conhecem-se os valores iniciais de x e v */

    var h = 0.125;

    for (i = 0; i < fireworks.length; i++) {
        var x = fireworks[i].position.x;
        var y = fireworks[i].position.y;
        var z = fireworks[i].position.z;
        var speedAux = speed[i];
        var newSpeed = acel * h;
        newSpeed += speedAux;
        setPosition(fireworks[i], x, y, z, speedAux);
        speed[i] = newSpeed;
        lifeP -= fade;
    }


//glutTimerFunc(33, MyWrapper::iterate, 1);
}
