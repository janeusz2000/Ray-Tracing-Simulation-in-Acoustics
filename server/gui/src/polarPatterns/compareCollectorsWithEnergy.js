'use strict'

export function compareCollectorsWithEnergy(collectorA, collectorB) {
  if (collectorA.x < collectorB.x) {
    return -1;
  } else if (collectorA.x > collectorB.x) {
    return 1;
  } else {
    if (collectorA.y < collectorB.y) {
      return -1;
    } else if (collectorA.y > collectorB.y) {
      return 1;
    }
  }
  return 0;
}
