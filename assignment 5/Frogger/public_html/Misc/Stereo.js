/**
 * @author alteredq / http://alteredqualia.com/
 * @authod mrdoob / http://mrdoob.com/
 * @authod arodic / http://aleksandarrodic.com/
 */

THREE.StereoEffect = function (renderer, aspect) {

    var translation = true;

    // API

    this.separation = 3;

    // internals

    var _width, _height;

    var _position = new THREE.Vector3();
    var _quaternion = new THREE.Quaternion();
    var _scale = new THREE.Vector3();

    var _aspect = aspect;

    // initialization

    renderer.autoClear = false;

    this.setSize = function (width, height) {

        _width = width / 2;
        _height = height;

        renderer.setSize(width, height);

    };

    function createCamera(target, target_separation) {

        if (camera instanceof THREE.OrthographicCamera) {
            target.left   = camera.left / 2; 
            target.right  = camera.right / 2;
            target.top    = camera.top;
            target.bottom = camera.bottom;
        } else {
            target.fov    = camera.fov;
            target.aspect = 0.5 * _aspect;
        }
        target.near = camera.near;
        target.far  = camera.far;
        target.updateProjectionMatrix();

        target.position.copy(_position);
        target.quaternion.copy(_quaternion);
        if (translation)
            target.translateX(target_separation);
    }

    this.render = function (scene, camera) {

        var _cameraL, _cameraR;
        if (camera instanceof THREE.OrthographicCamera) {
            _cameraL = new THREE.OrthographicCamera();
            _cameraR = new THREE.OrthographicCamera();
        } else {
            _cameraL = new THREE.PerspectiveCamera();
            _cameraR = new THREE.PerspectiveCamera();
        }

        scene.updateMatrixWorld();

        if (camera.parent === undefined)
            camera.updateMatrixWorld();

        camera.matrixWorld.decompose(_position, _quaternion, _scale);

        createCamera(_cameraL, - this.separation);
        createCamera(_cameraR, this.separation);

        renderer.setViewport(0, 0, _width * 2, _height);
        renderer.clear();

        renderer.setViewport(0, 0, _width, _height);
        renderer.render(scene, _cameraL);

        renderer.setViewport(_width, 0, _width, _height);
        renderer.render(scene, _cameraR);

        translation = false;
    };
};
