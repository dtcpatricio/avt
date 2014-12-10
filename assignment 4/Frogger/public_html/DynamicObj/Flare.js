/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

var elements = [];
var _fMaxSize = 0.0;
var _nPieces = 0.0;
var WinX = window.innerWidth;
var WinY = window.innerHeight;

var cam = new THREE.OrthographicCamera(0, WinX, 0, WinY, -500, 500);
var proj = cam.projectionMatrix;

// Could be a problem
function flareRange(a, b) {
    return ((Math.random() & 0xffffff) / 0xfffffe) * (b - a) + a;
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

        fEnvelopeSize = Math.abs(1.0 - 2.0 * el.fDistance);
        el.size = flareRange(0.6, 1.0) * fEnvelopeSize;
        elements.push(el);
    }
}

function Flare(_lx, _ly) {

    var flareImage = THREE.ImageUtils.loadTexture('particle.png');
    flareImage.magFilter = THREE.NearestFilter;

    var uniforms = {
        ambient: {type: "c", value: new THREE.Color(0x000000)},
        diffuse: {type: "c", value: new THREE.Color(0x000000)},
        spec: {type: "c", value: new THREE.Color(0x000000)},
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

    var material = new THREE.ShaderMaterial({vertexShader: vertexShader, fragmentShader: fragmentShader, uniforms: uniforms});

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
    flaredist = maxflaredist - flaredist;
    flaremaxsize = WinX * _fMaxSize;
    flarescale = WinX * _fMaxSize;

    // Destination is opposite side of centre from source
    dx = _cx + (_cx - _lx);
    dy = _cy + (_cy - _ly);

    for (i = 0; i < _nPieces; i++) {
        var el = elements[i];

        material.depthTest = false;
        material.transparent = true;
        material.blending = THREE[ "AdditiveBlending" ];

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

        var ratio = WinX / WinY;

        height = width / ratio;

        if (width > 1)
        {
            var geometry = new THREE.BoxGeometry(height, height, 0.0);
            this.flare = new THREE.Mesh(geometry, material);
            this.flare.frustumCulled = false;
            this.flare.position.x = px;
            this.flare.position.y = py;
            this.flare.position.z = 200.0;
            this.flare.renderDepth = 0.0;
            scene.add(this.flare);
        }
        
        material.depthTest = true;

    }
}





