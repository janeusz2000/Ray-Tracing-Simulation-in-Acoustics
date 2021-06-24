'use strict';

export function deleteTracking(scene, arrowList) {
  arrowList.forEach(arrow => { scene.remove(arrow); });
  arrowList = [];
}
