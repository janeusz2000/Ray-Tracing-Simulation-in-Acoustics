#include "resultsCalculation.h"

std::vector<WaveObject> createWaveObjects(const Collectors &collectors) {

  // TODO: create interpolation of the energy in time function

  return {};
}
std::vector<float>
calculateSoundPressureLevels(const std::vector<WaveObject> &waveObjectsVector) {

  std::vector<float> outputSoundPressureLevelVector(waveObjectsVector.size());
  for (const WaveObject &wave : waveObjectsVector) {

    // Trapezoid Integral calculation:
    // https://en.wikipedia.org/wiki/Trapezoidal_rule
    float soundLevelPressure = 0;
    for (size_t index = 1; index < wave.getData().size(); ++index) {
      soundLevelPressure += 1.0f / 2 *
                            (wave.getEnergyAtTimeIndex(index) +
                             wave.getEnergyAtTimeIndex(index - 1)) /
                            wave.getSampleRate();
    }

    auto outputPressure = [](float level) {
      return 120 + 10 * std::log10(level);
    };

    outputSoundPressureLevelVector.push_back(
        soundLevelPressure > 0 ? outputPressure(soundLevelPressure) : 0);
  }
  return outputSoundPressureLevelVector;
}

float WaveObject::getEnergyAtTimeIndex(int timeIndex) const {
  if (!isTimeIndexValid(timeIndex)) {
    std::stringstream errorMessage;
    errorMessage << "Calculated time to getEnergyAtTime(): "
                 << static_cast<float>(timeIndex) / sampleRate_
                 << " is invalid\n"
                 << "Given time index: " << timeIndex
                 << " is out of range in: " << *this;
    throw std::invalid_argument(errorMessage.str());
  }

  return data_[timeIndex];
}

bool WaveObject::isTimeIndexValid(size_t timeIndex) const {
  return timeIndex < data_.size() && timeIndex > 0;
}

const std::vector<float> &WaveObject::getData() const { return data_; }

void WaveObject::setEnergyAtTime(float time, float energy) {

  // TODO: resize data to be valid
  // TODO: Create wave approximation for input data

  int timeIndex = static_cast<int>(time * 1.0f / sampleRate_);

  // TODO: approximate energy across different samples

  data_[timeIndex] = energy;
}

int WaveObject::getSampleRate() const { return sampleRate_; }

void WaveObject::printItself(std::ostream &os) const noexcept {
  os << "Wave Object\n"
     << "Sample rate: " << sampleRate_ << "\n"
     << "Data size: " << data_.size();
}

std::unordered_map<float, float> ResultInterface::getResults(
    const std::unordered_map<float, Collectors> &energyCollectorsPerFrequency)
    const {
  std::unordered_map<float, float> calculatedParameterVectorInTime;
  for (auto it = std::cbegin(energyCollectorsPerFrequency);
       it != std::cend(energyCollectorsPerFrequency); ++it) {

    float frequency = it->first;
    calculatedParameterVectorInTime.insert(std::make_pair<float, float>(
        std::move(frequency), std::move(calculateParameter(it->second))));
  }
  return calculatedParameterVectorInTime;
}

float DiffusionCoefficient::calculateParameter(
    const Collectors &collectors) const {

  std::vector<WaveObject> wavePerEnergyCollector =
      createWaveObjects(collectors);

  std::vector<float> soundPressureLevels =
      calculateSoundPressureLevels(wavePerEnergyCollector);

  return calculateDiffusionCoefficient(soundPressureLevels);
}

float calculateDiffusionCoefficient(
    const std::vector<float> &soundPressureLevels) {

  float alpha = std::pow(std::accumulate(soundPressureLevels.cbegin(),
                                         soundPressureLevels.cend(), 0),
                         2);

  float beta = 0;
  std::for_each(
      soundPressureLevels.cbegin(), soundPressureLevels.cend(),
      [&](float const &soundPressure) { beta += std::pow(soundPressure, 2); });

  float gamma = static_cast<float>(soundPressureLevels.size() - 1) * beta;

  return (alpha - beta) / gamma;
}