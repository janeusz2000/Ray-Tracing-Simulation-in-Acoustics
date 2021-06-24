'use strict'

export function divideCollectorListIntoTwoPolarPatters(
    collectorWithEnergyList) {
  const outputListEvenIndex = [];
  const outputListOddIndex = [];
  const listSize = collectorWithEnergyList.length;

  var index = listSize % 2;
  if (index == 1) {
    outputListEvenIndex.push(collectorWithEnergyList[0]);
    outputListOddIndex.push(collectorWithEnergyList[0]);
  }
  for (; index < listSize; index++) {
    if (index % 2 == 0) {
      outputListEvenIndex.push(collectorWithEnergyList[index]);
    } else {
      outputListOddIndex.push(collectorWithEnergyList[index]);
    }
  }

  return [ outputListEvenIndex, outputListOddIndex ];
}