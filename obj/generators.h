#ifndef GENERATORS_H
#define GENERATORS_H

#include "constants.h"
#include "core/exceptions.h"
#include "core/ray.h"
#include "core/vec3.h"

#include <cmath>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <random>

// TODO: Change all doubles to float

namespace generators
{
    struct RandomGen
    {
        virtual ~RandomGen(){};
        virtual double getNext() = 0;
    };

    struct UniformRandomGen : public RandomGen
    {
        UniformRandomGen(double min, double max) : engine_(std::random_device()()), dist_(min, max){};
        double getNext() override
        {
            return dist_(engine_);
        }

        std::mt19937_64 engine_;
        std::uniform_real_distribution<double> dist_;
    };

    struct FakeRandomGen : public RandomGen
    {
        double getNext() override
        {
            return 0;
        }
    };

    class PointSource final // This object is generating rays with the origin
                            // of the point source position and direction calculated
                            // from leftCorner position + x * Vec3(1, 0, 0) + y * Vec3(0, 1, 0)
                            // and origin of the source position
    {
    public:
        PointSource() = delete;
        explicit PointSource::PointSource(const double &freq, const size_t &numOfRaysPerRow,
                                          const double &SampleSize, RandomGen *randomGen) : frequency_(freq),
                                                                                            numOfRaysPerRow_(numOfRaysPerRow), sampleSize_(SampleSize),
                                                                                            origin_(core::Vec3(0, 0, 4)), randomGen_(randomGen) { updateSampleSize(); }

        // OPERATORS
        bool operator==(const PointSource &other) const;
        friend std::ostream &operator<<(std::ostream &os, const PointSource &pointSource);

        // METHODS
        void updateSampleSize();
        core::Ray generateRay(const size_t &xIter, const size_t &yIter);

        // GETTERS AND SETTERS
        double frequency() const;
        void setFrequency(const double &freq);

        double sampleSize() const;
        void setDiffusorSize(const double &diffusorSize);

        size_t numOfRaysPerRow() const;
        void setNumOfRaysPerRow(const size_t &raynum);

        core::Vec3 origin() const;
        void setOrigin(const core::Vec3 &point);

        core::Vec3 PointSource::getLeftCorner() const;
        void PointSource::setLeftCorner(const core::Vec3 &point);

    private:
        core::Vec3 origin_, leftCorner_;
        double frequency_, sampleSize_;
        size_t numOfRaysPerRow_;

        RandomGen *randomGen_;
    };

} // namespace generators

#endif
