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


namespace generators
{
    struct RandomGen {
        virtual ~RandomGen() {};
        virtual double getNext()  = 0;
    };

    struct UniformRandomGen : public RandomGen {
        UniformRandomGen(double min, double max) : _engine(std::random_device()()), _dist(min, max) {};
        double getNext() override {
            return _dist(_engine);
        }

        std::random_device _rd;
        std::mt19937_64 _engine;
        std::uniform_real_distribution<double> _dist;
    };

    struct FakeRandomGen : public RandomGen {
        double getNext() override 
        {
            return 0;
        }
    };

    class PointSource final // This object is similar to Camera in graphics ray-tracer, that why name convension is simillar to camera
    {
    public:
        PointSource() = delete;
        PointSource::PointSource(const double &freq, const size_t &rayNumPerRow, const double &diffusorSize, RandomGen* randomGen) : _frequency(freq), _rayNumPerRow(rayNumPerRow), _diffusorSize(diffusorSize), _origin(core::Vec3(0, 0, 4)), _randomGen(randomGen)
        {
        updateDiffusorSize();
        }
        PointSource(const PointSource &) = default;
        ~PointSource() = default;

        // OPERATORS
        PointSource &operator=(const PointSource &) = default;
        bool operator==(const PointSource &other) const;
        friend std::ostream &operator<<(std::ostream &os, const PointSource &pointSource);

        // METHODS
        void updateDiffusorSize();
        // TODO: Parameter names are not self explanatory. Please update.
        core::Ray GenerateRay(const size_t &xIter, const size_t &yIter);

        // GETTERS AND SETTERS
        double getFrequency() const;
        void setFrequency(const double &freq);

        double getDiffusorSize() const;
        void setDiffusorSize(const double &diffusorSize);

        size_t getRayNumPerRow() const;
        void setRayNumPerRow(const size_t &raynum);

        core::Vec3 getOrigin() const;
        void setOrigin(const core::Vec3 &point);

        core::Vec3 PointSource::getLeftCorner() const;
        void PointSource::setLeftCorner(const core::Vec3 &point);

    private:
        core::Vec3 _origin, _leftCorner;
        double _frequency, _diffusorSize;
        size_t _rayNumPerRow;
        
        RandomGen* _randomGen;

    };

} // namespace generators

#endif
