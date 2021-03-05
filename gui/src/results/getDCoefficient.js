export function getDCoefficient(data) {
  // TODO: Time Factor

  const energy = [];
  for (var energyIndex = 0; energyIndex < data.length; energyIndex++) {
    if (data[energyIndex] == 0) {
      energy.push(0);
    } else {
      energy.push(120 + 10 * Math.log10(Math.abs(data[energyIndex])));
    }
  }

  const alpha = Math.pow(energy.reduce((acc, curr) => acc + curr), 2);
  const beta = energy.reduce((acc, curr) => Math.pow(curr, 2) + acc);

  const gamma = (energy.length - 1) * beta;
  return (alpha - beta) / gamma
}