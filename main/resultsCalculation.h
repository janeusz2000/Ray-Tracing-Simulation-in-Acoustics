#ifndef RESULTS_CALCULATION_H
#define RESULTS_CALCULATION_H

#include "core/classUtlilities.h"
#include "obj/objects.h"

#include <cmath>
#include <map>
#include <numeric>
#include <sstream>
#include <unordered_map>


using Collectors = std::vector<std::unique_ptr<objects::EnergyCollector>>;

// Converts given pressure defined in [Pa] into [dB].
// NOTE: given |pressure| must be greater then zero, otherwise it will return 0.
float convertPressureToDecibels(float pressure);

// Represents acquired energy in time function. This class try to imitate
// ".wav" file recording acquired in real research of acoustic recording.
class WaveObject : public Printable {
public:
  explicit WaveObject(int sampleRate) : sampleRate_(sampleRate){};
  const std::vector<float> &getData() const;
  // return pressure defined in [Pa]
  float getTotalPressure() const;

  float getEnergyAtTime(float time) const;
  void addEnergyAtTime(float time, float energy);

  int getSampleRate() const;
  size_t length() const;

  void printItself(std::ostream &os) const noexcept override;

private:
  u_int64_t getTimeIndex(float time) const;
  // Returns true if given time index is within range of |data|.
  int sampleRate_;
  std::vector<float> data_;
};

class WaveObjectFactory {
public:
  explicit WaveObjectFactory(int sampleRate) : sampleRate_(sampleRate){};
  std::vector<WaveObject>
  createWaveObjectsFromCollectors(const Collectors &collectors);

private:
  int sampleRate_;
};

std::vector<float>
calculateSoundPressureLevels(const std::vector<WaveObject> &waveObjectVector);

using parameterPerFrequency = std::map<float, float>;
// Interface struct for acoustic parameter calculation from function sequence of
// sound level pressure acquired in simulation.
class ResultInterface : public Printable {
public:
  explicit ResultInterface(WaveObjectFactory *waveObjectFactory)
      : waveFactory_(waveObjectFactory){};
  virtual ~ResultInterface(){};

  virtual parameterPerFrequency getResults(
      const std::unordered_map<float, Collectors> &energyCollectorsPerFrequency)
      const;

  virtual std::string_view getName() const noexcept;

  void printItself(std::ostream &os) const noexcept override;

protected:
  virtual float calculateParameter(const Collectors &collectors) const = 0;
  WaveObjectFactory *waveFactory_;
};
// Diffusion Coefficient is a measure of the uniformity of diffusion for a
// representative sample of sources over a complete semicircle for a single
// plane diffuser, or a complete hemisphere for a hemispherical diffuser.
//
// "Mean or a weighting of the directional diffusion coefficients for
// the difference source ositions is used to calculate the diffusion
// coefficient, as specified in 8.4 (source). A guideline to achieve
// a representative sample of sources is given in 6.2.2 (source). The lack of
// a subscript for d indicates random incidence." ~Comes from (source): ISO
// 17497-2:2012
class DiffusionCoefficient : public ResultInterface {
public:
  explicit DiffusionCoefficient(WaveObjectFactory *waveFactory)
      : ResultInterface(waveFactory){};

  std::string_view getName() const noexcept override;

  void printItself(std::ostream &os) const noexcept override;

protected:
  float calculateParameter(const Collectors &collectors) const override;

private:
  // Calculates parameter from given vector of pressures defined in [dB]
  float calculateDiffusionCoefficient(
      const std::vector<float> &soundPressureLevels) const;
};

class NormalizedDiffusionCoefficient : public ResultInterface {
public:
  explicit NormalizedDiffusionCoefficient(
      WaveObjectFactory *waveFactory,
      const std::unordered_map<float, Collectors>
          &referenceResultCollectorsPerFrequency);

  std::string_view getName() const noexcept override;

  parameterPerFrequency getResults(
      const std::unordered_map<float, Collectors> &energyCollectorsPerFrequency)
      const override;

protected:
  // This is not used in this class [only God can judge me].
  float calculateParameter(const Collectors &collectors) const override;

private:
  WaveObjectFactory *waveFactory_;
  parameterPerFrequency referenceResult_;
};

#endif