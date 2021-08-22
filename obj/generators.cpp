#include "generators.h"

namespace generators {

void RandomRayOffseter::printItself(std::ostream &os) const noexcept {
  os << "Random Ray Offseter Interface Class";
}

void FakeOffseter::printItself(std::ostream &os) const noexcept {
  os << "Fake Ray Offseter";
}

void RayFactory::printItself(std::ostream &os) const noexcept {
  os << "Ray Factory Abstract Class";
}

PointSpeakerRayFactory::PointSpeakerRayFactory(int numOfRaysAlongEachAxis,
                                               float sourcePower,
                                               ModelInterface *model)
    : model_(model), numOfRaysAlongEachAxis_(numOfRaysAlongEachAxis),
      currentRayIndex_(0),
      energyPerRay_(sourcePower /
                    (numOfRaysAlongEachAxis * numOfRaysAlongEachAxis)) {

  if (numOfRaysAlongEachAxis_ <= 0) {
    std::stringstream ss;
    ss << "|numOfRaysAlongEachAxis| given to: \n"
       << *this
       << "cannot be equal or less than zero! "
          "\n|numOfRaysAlongEachAxis|: "
       << numOfRaysAlongEachAxis_;
    throw std::invalid_argument(ss.str());
  }
  if (sourcePower < 0) {
    std::stringstream ss;
    ss << "|sourcePower| power cannot be less than zero! \n|sourcePower|: "
       << sourcePower;
    throw std::invalid_argument(ss.str());
  }
  if (model_->empty()) {
    throw std::invalid_argument("Model cannot be Empty!");
  }

  // Comes from the requirements of the ISO 17497-2:2012, which says that point
  // source must be placed at least twice as high as the microphone radius
  // array. Because microphone radius array is equal to:
  // |kSimulationHeight| / 2 * model.height(), we can assume:
  origin_ =
      core::Vec3(0, 0,
                 std::max(static_cast<float>(constants::kSimulationHeight),
                          constants::kSimulationHeight * model_->height()));

  float sizeFactor = -model_->sideSize();
  targetReferenceDirection_ =
      core::Vec3(sizeFactor, sizeFactor, model->height()) - origin_;
};

bool PointSpeakerRayFactory::genRay(core::Ray *ray) {
  if (!isRayAvailable()) {
    return false;
  }
  *ray = core::Ray(origin_, getDirection(currentRayIndex_), energyPerRay_);
  ++currentRayIndex_;
  return true;
}
core::Vec3 PointSpeakerRayFactory::getDirection(int currentRayIndex) const {
  if (numOfRaysAlongEachAxis_ == 1) {
    return -core::Vec3::kZ;
  }

  int xIndex = currentRayIndex % numOfRaysAlongEachAxis_;
  int yIndex = currentRayIndex / numOfRaysAlongEachAxis_;

  float u = 2 * static_cast<float>(xIndex) / (numOfRaysAlongEachAxis_ - 1) *
            model_->sideSize();
  float v = 2 * static_cast<float>(yIndex) / (numOfRaysAlongEachAxis_ - 1) *
            model_->sideSize();

  return targetReferenceDirection_ + core::Vec3(u, v, 0);
}

bool PointSpeakerRayFactory::isRayAvailable() const {
  return (currentRayIndex_ < numOfRaysAlongEachAxis_ * numOfRaysAlongEachAxis_);
}

void PointSpeakerRayFactory::printItself(std::ostream &os) const noexcept {
  os << "POINT SPEAKER RAY FACTORY\n"
     << "\tOrigin: " << origin_ << "\n"
     << "\tNum Of Rays Along Each Axis: " << numOfRaysAlongEachAxis_ << "\n"
     << "\tCurrent Ray Index: " << currentRayIndex_ << "\n"
     << "\tEnergy Per Ray: " << energyPerRay_ << "\n"
     << "\tTarget Reference Direction: " << targetReferenceDirection_;
}

bool CustomPointRayFactory::genRay(core::Ray *ray) {
  if (!wasUsed_) {
    *ray = core::Ray(origin_, direction_, energy_);
    wasUsed_ = true;
    return true;
  }
  return false;
}

core::Vec3 CustomPointRayFactory::origin() const { return origin_; }

void CustomPointRayFactory::printItself(std::ostream &os) const noexcept {
  os << "TEST RAY FACTORY\n\trendering rays with: \n\torigin: " << origin_
     << '\n'
     << "\tdirection: " << direction_ << '\n'
     << "\tenergy: " << energy_;
}
} // namespace generators
