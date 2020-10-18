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

    class PointSource final // This object is generating rays with the origin 
                            // of the point source position and direction calculated 
                            // from leftCorner position + x * Vec3(1, 0, 0) + y * Vec3(0, 1, 0) 
                            // and origin of the source position 
    {
    public:
        PointSource() = delete;
        explicit PointSource::PointSource(const double &freq, const size_t &numOfRaysPerRow, \
                                        const double &SampleSize, RandomGen* randomGen) : _frequency(freq),\
                                        _numOfRaysPerRow(numOfRaysPerRow), _sampleSize(SampleSize), \
                                        _origin(core::Vec3(0, 0, 4)), _randomGen(randomGen) {updateSampleSize();}

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
        core::Vec3 _origin, _leftCorner;
        double _frequency, _sampleSize;
        size_t _numOfRaysPerRow;
        
        RandomGen* _randomGen;

    };

} // namespace generators

#endif
