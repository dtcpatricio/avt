/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var elements = [];
var _fMaxSize = 0.0;
var _nPieces = 0.0;
var WinX = window.innerWidth;
var WinY = window.outerHeight;

var proj = new THREE.Matrix4();
proj.identity();
proj.makeOrthographic(0, WinX, 0, WinY, -1, 1);

// Could be a problem
function flareRange(a, b) {
    return (Math.random() % (b - a + 1)) + a;
}

function elementsFlare() {
    this.fDistance = 0.0;
    this.size = 0.0;
}

function randomize(nPieces, fMaxSize) {

    var fFracDist;
    var fEnvelopeSize;
    var el;

    _fMaxSize = fMaxSize;
    _nPieces = nPieces;

    fFracDist = 1.0 / (_nPieces - 1);

    for (i = 0; i < nPieces; ++i)
    {
        el = new elementsFlare();
        el.fDistance = fFracDist * i + flareRange(0, fFracDist);

        fEnvelopeSize = Math.abs(1.0 - 2 * el.fDistance);
        el.size = flareRange(0.6, 1.0) * fEnvelopeSize;
        elements.push(el);
    }
}

function Flare(_lx, _ly) {

    var flareImage = THREE.ImageUtils.loadTexture('particle.png');
    flareImage.magFilter = THREE.NearestFilter;

    var uniforms = {
        ambient: {type: "c", value: new THREE.Color(0xFFFFFF)},
        diffuse: {type: "c", value: new THREE.Color(0xFFFFFF)},
        spec: {type: "c", value: new THREE.Color(0xFFFFFF)},
        shininess: {type: "f", value: 32},
        billboard: {type: "f", value: 1.0},
        spotDir: {type: "v3", value: sDir},
        spotPos: {type: "v3", value: sPos},
        night: {type: "v3", value: nightMode},
        texMode: {type: "f", value: 1.0},
        texture1: {type: "t", value: flareImage},
        lens: {type: "f", value: 1.0},
        lensProj: {type: "m4", value: proj}
    };

    //var rectGeom = new THREE.BoxGeometry(0.25, 0.25, 0.00000000000000001);
    var geometry;
    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});
    material.depthTest = false;
    //material.transparent = true;
    //material.blending = THREE[ "AdditiveBlending" ];

    var dx, dy;          // Screen coordinates of "destination"
    var px, py;          // Screen coordinates of flare element
    var maxflaredist, flaredist, flaremaxsize, flarescale;
    var width, height;
    var _cx = WinX / 2;
    var _cy = WinY / 2;

    // Compute how far off-center the flare source is.
    maxflaredist = _cx * _cx + _cy * _cy;
    flaredist = (_lx - _cx) * (_lx - _cx) +
            (_ly - _cy) * (_ly - _cy);
    flaredist = Math.abs(maxflaredist - flaredist);
    flaremaxsize = WinX * _fMaxSize;
    flarescale = WinX * _fMaxSize;

    // Destination is opposite side of centre from source
    dx = _cx + (_cx - _lx);
    dy = _cy + (_cy - _ly);

    for (i = 0; i < _nPieces; ++i) {
        var el = elements[i];

        // Position is interpolated along line between start and destination.
        px = 1.0 - el.fDistance * _lx + el.fDistance * dx;
        py = 1.0 - el.fDistance * _ly + el.fDistance * dy;

        // Piece size are 0 to 1; flare size is proportion of
        // screen width; scale by flaredist/maxflaredist.
        width = (flaredist * flarescale * el.size) / maxflaredist;

        // Width gets clamped, to allows the off-axis flares
        // to keep a good size without letting the elements get
        // too big when centered.
        if (width > flaremaxsize)
        {
            width = flaremaxsize;
        }

        height = (320 * width * WinX) / (240 * WinY);

        if (width > 1)
        {
            
            geometry = new THREE.BoxGeometry(100, 100, 0.00000000000000001);
            this.flare = new THREE.Mesh(geometry, material);
            this.flare.position.x = px - width/2;
            this.flare.position.y = py - height/2;
            this.flare.position.z = 5.0;
            this.flare.rotation.x += 90;
            scene.add(this.flare);
        }

    }


    material.depthTest = true;
}

function createFlare() {
    randomize(5, 0.05);
    var flare = new Flare(10, 10);
    return flare;
}

