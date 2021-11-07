#include "resultsCalculation.h"

float convertPressureToDecibels(float pressure) {
  return pressure > 0 ? (120 + 10 * std::log10(pressure)) : 0.0f;
}

u_int64_t WaveObject::getTimeIndex(float time) const {
  return std::floor(time * sampleRate_);
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

float WaveObject::getEnergyAtTime(float time) const {
  u_int64_t timeIndex = getTimeIndex(time);
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
  u_int64_t timeIndex = getTimeIndex(time);
  if (timeIndex >= data_.size()) {
    data_.resize(timeIndex + 1, 0);
  }

  // FIXME: create approximation of the energy if sample is between two samples

  data_[timeIndex] += energy;
}

int WaveObject::getSampleRate() const { return sampleRate_; }

void WaveObject::printItself(std::ostream &os) const noexcept {
  os << "Wave Object\n"
     << "Sample rate: " << sampleRate_ << " Hz\n"
     << "Data size: " << length();
}

std::vector<WaveObject> WaveObjectFactory::createWaveObjectsFromCollectors(
    const Collectors &collectors) {
  std::vector<WaveObject> output;
  output.reserve(collectors.size());

  for (auto &collector : collectors) {
    WaveObject wave(sampleRate_);
    for (auto energyPerTimeIt = collector->getEnergy().cbegin();
         energyPerTimeIt != collector->getEnergy().cend(); ++energyPerTimeIt) {
      wave.addEnergyAtTime(energyPerTimeIt->first, energyPerTimeIt->second);
    }

    output.push_back(wave);
  }
  return output;
}

parameterPerFrequency ResultInterface::getResults(
    const std::unordered_map<float, Collectors> &energyCollectorsPerFrequency)
    const {
  parameterPerFrequency calculatedParameterVectorInTime;
  for (auto it = energyCollectorsPerFrequency.cbegin();
       it != energyCollectorsPerFrequency.cend(); ++it) {

    float frequency = it->first;
    calculatedParameterVectorInTime.insert(
        std::make_pair(frequency, calculateParameter(it->second)));
  }
  return calculatedParameterVectorInTime;
}

void ResultInterface::printItself(std::ostream &os) const noexcept {
  os << getName();
}

std::string_view ResultInterface::getName() const noexcept {
  return "Acoustic Parameter Interface Class";
}

float DiffusionCoefficient::calculateParameter(
    const Collectors &collectors) const {

  std::vector<WaveObject> wavePerEnergyCollector =
      waveFactory_->createWaveObjectsFromCollectors(collectors);

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

std::string_view DiffusionCoefficient::getName() const noexcept {
  return "Acoustic Diffusion Coefficient";
}

void DiffusionCoefficient::printItself(std::ostream &os) const noexcept {
  os << getName();
}

std::string_view NormalizedDiffusionCoefficient::getName() const noexcept {
  return "Normalized Acoustic Diffusion Coefficient";
}

NormalizedDiffusionCoefficient::NormalizedDiffusionCoefficient(
    WaveObjectFactory *waveFactory, const std::unordered_map<float, Collectors>
                                        &referenceResultCollectorsPerFrequency)
    : ResultInterface(waveFactory), waveFactory_(waveFactory) {

  DiffusionCoefficient referenceDiffusion(waveFactory_);
  referenceResult_ =
      referenceDiffusion.getResults(referenceResultCollectorsPerFrequency);
}

parameterPerFrequency NormalizedDiffusionCoefficient::getResults(
    const std::unordered_map<float, Collectors> &energyCollectors) const {

  DiffusionCoefficient actualDiffusionCoefficient(waveFactory_);
  parameterPerFrequency result =
      actualDiffusionCoefficient.getResults(energyCollectors);

  if (referenceResult_.size() != result.size()) {
    std::stringstream ss;
    ss << "Error in NormalizedDiffusionCoefficient::getResults()\n"
       << "Reference ParameterPerFrequency::size(): " << referenceResult_.size()
       << " should be the same as actual ParameterPerFrequency::size(): "
       << result.size();
    throw std::invalid_argument(ss.str());
  }

  parameterPerFrequency output;
  for (auto [itReference, itActual] =
           std::tuple{referenceResult_.cbegin(), result.cbegin()};
       itReference != referenceResult_.cend() || itActual != result.cend();
       itReference++, itActual++) {

    if (itActual->first != itReference->first) {
      std::stringstream ss;
      ss << "Error in NormalizedDiffusionCoefficient::getResults()\n"
         << "Frequencies from reference: " << itReference->first
         << " and from sample: " << itActual->first << "\n"
         << "are not the same!";
      throw std::invalid_argument(ss.str());
    }

    float tempValue =
        (itActual->second - itReference->second) / (1 - itReference->second);
    float parameterValue = tempValue > 0 ? tempValue : 0;
    float frequency = itActual->first;
    

    output.insert(std::make_pair(frequency, parameterValue));
  }
  return output;
}

float NormalizedDiffusionCoefficient::calculateParameter(
    const Collectors &collectors) const {
  // TODO: Delete this becasue its silly :P
  return 0;
}
