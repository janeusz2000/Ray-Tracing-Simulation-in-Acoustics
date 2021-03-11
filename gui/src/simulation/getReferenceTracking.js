'use strict'
import{Tracking} from './Tracking'

export function getReferenceTracking() {
  return referenceTrackingData.map(tracking => new Tracking(tracking));
}