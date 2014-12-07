/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


var fireworks = [];
var speed = [];
var acel = new THREE.Vector3(0.1, 0.0, -0.15);
var lifeF = 1.0;
var fade = 0.0005;
var M_PI = 3.14;
var init = 0.0;

function Particle() {

    var particleImage = THREE.ImageUtils.loadTexture('particle.png');
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
        life: {type: "f", value: lifeF}
    };

    var rectGeom = new THREE.BoxGeometry(0.25, 0.25, 0.00000000000000001);
    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});
    material.transparent = true;
    material.depthTest = false;
    for (i = 0; i < 40; i++) {

        this.particle = new THREE.Mesh(rectGeom, material);
        this.particle.position.x = 0.0;
        this.particle.position.y = 0.0;
        this.particle.position.z = -10.0;
        this.particle.rotation.x += 90;

        fireworks.push(this.particle);
        speed.push(speedP);
        scene.add(this.particle);
    }

    material.depthTest = true;
}

function frand() {
    return Math.random();
}

function initParticles() {
    var v, theta, phi, rAux;

    for (i = 0; i < fireworks.length; i++)
    {
        rAux = frand();
        v = 0.8 * rAux;
        phi = rAux * M_PI;
        theta = 2.0 * rAux * M_PI;
        speed[i] = new THREE.Vector3(v * Math.cos(theta) * Math.sin(phi), v * Math.cos(phi), v * Math.sin(theta) * Math.sin(phi));
        fireworks[i].position.x = 0.0;
        fireworks[i].position.y = 0.0;
        fireworks[i].position.z = 7.0;
        lifeF = 1.0;
        /* tom amarelado que vai ser multiplicado pela textura que varia entre branco e preto */
        //fireworks[i].setColors(new THREE.Vector3(0.882, 0.552, 0.211));
    }

}

function iterate() {

    /* Método de Euler de integração de eq. diferenciais ordinárias
     h representa o step de tempo; dv/dt = a; dx/dt = v; e conhecem-se os valores iniciais de x e v */

    var h = 0.125;
    
    if (frog[0].position.y > 4.7) {
        
        if(init === 0.0){
            initParticles();
            init = 1.0;
        }
        
        for (i = 0; i < fireworks.length; i++) {
            if (lifeF > 0.0) {
                fireworks[i].position.x += speed[i].x;
                fireworks[i].position.y += speed[i].y;
                fireworks[i].position.z += speed[i].z;

                speed[i].x += acel.x * h;
                speed[i].y += acel.y * h;
                speed[i].z += acel.z * h;

                lifeF -= fade;

            }
            else{
                fireworks[i].position.x = 50.0;
                fireworks[i].position.z = -10.0;
            }
        }
    }

}
