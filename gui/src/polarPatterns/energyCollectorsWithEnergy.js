export class EnergyCollectorWithEnergy {
  constructor(EnergyCollector, energy) {
    this.x = EnergyCollector.x;
    this.y = EnergyCollector.y;
    this.z = EnergyCollector.z;
    this.energy = energy;
  }
}