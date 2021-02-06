
export function animate(scene, renderer, camera, controls) {
  // Hold the current object's scope,
  // for accessing properties from within the callback method.
  var $this = this;
  function renderLoop() {
    requestAnimationFrame(renderLoop);
    controls.update(); // only required if controls.enableDamping = true, or if
                       // controls.autoRotate = true
    renderer.render(scene, camera);
  }
  renderLoop();
}