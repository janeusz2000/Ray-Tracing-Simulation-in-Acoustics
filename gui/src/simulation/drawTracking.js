
'use strict';
import {getRandomColorAttribute} from './getRandomColorAttribute'

const THREE = require('three');

export function drawTracking(scene, frequency, arrowList, allTracking) {

  const currentTracking =
      allTracking.find(tracking => tracking.frequency === frequency);

  if (currentTracking == null) {
    throw Error("Desired Frequency of Tracking not Found! Desired Frequency: " +
                frequency.toString())
  }

  const trackingData = currentTracking.trackings;
  trackingData.forEach(tracking => {
    const color = getRandomColorAttribute();
    tracking.forEach(currentRay => {
      const direction =
          new THREE.Vector3(currentRay.direction.x, currentRay.direction.z,
                            currentRay.direction.y);
      const origin = new THREE.Vector3(currentRay.origin.x, currentRay.origin.z,
                                       currentRay.origin.y);
      const length = currentRay.length;
      const energy = currentRay.energy;
      const arrowSize = energy * trackingData.length / 30000;
      const arrow = new THREE.ArrowHelper(direction, origin, length, color,
                                          arrowSize, arrowSize / 2);
      scene.add(arrow);
      arrowList.push(arrow);
    })
  })
}