#ifndef RESULTS_CALCULATION_H
#define RESULTS_CALCULATION_H

#include "core/classUtlilities.h"
#include "obj/objects.h"

#include <cmath>

using Collectors = std::vector<std::unique_ptr<objects::EnergyCollector>>;

// Represents acquired energy in time function. This class try to imitate
// ".wav" file recording acquired in real research of acoustic recording.
class WaveObject : public Printable {
public:
  explicit WaveObject(int sampleRate) : sampleRate_(sampleRate){};

  const std::vector<float> &getData() const;

  float getEnergyAtTimeIndex(int timeIndex) const;
  void setEnergyAtTime(float time, float energy);
  int getSampleRate() const;

  void printItself(std::ostream &os) const noexcept override;

private:
  // Returns true if given time index is within range of |data|.
  bool isTimeIndexValid(size_t timeIndex) const;

  int sampleRate_;
  std::vector<float> data_;
};

std::vector<WaveObject> createWaveObjects(const Collectors &collectors);
std::vector<float>
calculateSoundPressureLevels(const std::vector<WaveObject> &waveObjectVector);

// Interface struct for acoustic parameter calculation from function sequence of
// sound level pressure acquired in simulation.
struct ResultInterface {
  virtual ~ResultInterface(){};

  // Calculates desired acoustic parameter in frequency function. Key of the
  // std::unordered_map represents frequency and element value represent
  // value of the calculated acoustic parameter.
  std::unordered_map<float, float> getResults(
      const std::unordered_map<float, Collectors> &energyCollectorsPerFrequency)
      const;

protected:
  virtual float calculateParameter(const Collectors &collectors) const = 0;
};

struct DiffusionCoefficient : public ResultInterface {
private:
  float calculateParameter(const Collectors &collectors) const override;
};

// Diffusion Coefficient is a measure of the uniformity of diffusion for a
// representative sample of sources over a complete semicircle for a single
// plane diffuser, or a complete hemisphere for a hemispherical diffuser.
//
// "Mean or a weighting of the directional diffusion coefficients for
// the difference source ositions is used to calculate the diffusion
// coefficient, as specified in 8.4 (source). A guideline to achieve
// a representative sample of sources is given in 6.2.2 (source). The lack of a
// subscript for d indicates random incidence." ~Comes from (source): ISO
// 17497-2:2012
float calculateDiffusionCoefficient(
    const std::vector<float> &soundPressureLevels);

#endif