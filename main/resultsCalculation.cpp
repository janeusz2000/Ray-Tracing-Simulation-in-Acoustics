#include "resultsCalculation.h"

float convertPressureToDecibels(float pressure) {
  return pressure > 0 ? (120 + 10 * std::log10(pressure)) : 0.0f;
}

float WaveObject::getTotalPressure() const {
  // Trapezoid Integral calculation:
  // https://en.wikipedia.org/wiki/Trapezoidal_rule
  const float kDt = 1.0f / sampleRate_;
  float total = 0;
  for (size_t sampleIndex = 1; sampleIndex < length(); ++sampleIndex) {
    float t0 = static_cast<float>(sampleIndex) / sampleRate_;
    float t1 = static_cast<float>(sampleIndex - 1) / sampleRate_;
    total += kDt * (getEnergyAtTime(t0) + getEnergyAtTime(t1)) / 2.0f;
  }

  return (total > 0) ? (120 + 10 * std::log10(total)) : 0;
}

std::vector<float>
calculateSoundPressureLevels(const std::vector<WaveObject> &waveObjects) {

  std::vector<float> soundPressureLevels;
  soundPressureLevels.reserve(waveObjects.size());
  for (const WaveObject &wave : waveObjects) {
    soundPressureLevels.push_back(wave.getTotalPressure());
  }
  return soundPressureLevels;
}

// private
// TODO: add this to .h file
int64 WaveObject::getTimeIndex(float time) const {
  return std::floor(time * sampleRate_);
}

float WaveObject::getEnergyAtTime(float time) const {
  int64 timeIndex = getTimeIndex(time);
  if (timeIndex >= data_.size() || timeIndex < 0) {
    return 0;
  }
  return data_[timeIndex];
}

size_t WaveObject::length() const { return data_.size(); }

const std::vector<float> &WaveObject::getData() const { return data_; }

void WaveObject::addEnergyAtTime(float time, float energy) {
  if (time < 0) {
    std::stringstream errorStream;
    errorStream << "given time at addEnergyAtTime() in: \n"
                << *this << "cannot be less then 0! Given Time: " << time
                << "s.";
    throw std::invalid_argument(errorStream.str());
  }
  int64 timeIndex = getTimeIndex(time);
  if (timeIndex >= data_.size()) {
    data_.resize(timeIndex + 1, 0);
  }

  // TODO: create approximation of the energy if sample is between two samples

  data_[timeIndex] += energy;
}

int WaveObject::getSampleRate() const { return sampleRate_; }

void WaveObject::printItself(std::ostream &os) const noexcept {
  os << "Wave Object\n"
     << "Sample rate: " << sampleRate_ << " Hz\n"
     << "Data size: " << length();
}

std::vector<WaveObject> createWaveObjects(const Collectors &collectors,
                                          int sampleRate = 96e3) {
  std::vector<WaveObject> output;
  output.reserve(collectors.size());

  for (objects::EnergyCollector *collector : collectors) {
    WaveObject wave(sampleRate);
    for (auto energyPerTimeIt = collector->getEnergy().cbegin();
         energyPerTimeIt != collector->getEnergy().cend(); ++energyPerTimeIt) {
      wave.addEnergyAtTime(energyPerTimeIt->first, energyPerTimeIt->second);
    }

    output.push_back(wave);
  }
  return output;
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

float DiffusionCoefficient::calculateDiffusionCoefficient(
    const std::vector<float> &soundPressureLevels) const {

  float alpha = std::pow(std::accumulate(soundPressureLevels.cbegin(),
                                         soundPressureLevels.cend(), 0),
                         2);

  float beta = 0;
  for (const float pressure : soundPressureLevels) {
    beta += std::pow(pressure, 2);
  }

  float gamma = static_cast<float>(soundPressureLevels.size() - 1) * beta;

  return (alpha - beta) / gamma;
}
