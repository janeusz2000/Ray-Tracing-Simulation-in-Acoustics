#include "constants.h"
#include "core/exceptions.h"
#include "core/ray.h"
#include "core/vec3.h"
#include "obj/objects.h"
#include "gtest/gtest.h"

#include <cmath>
#include <sstream>
#include <random>

// TODO: Change all doubles to float
// TODO: change underscore to the end

using core::Ray;
using core::RayHitData;
using core::Vec3;

namespace objects
{
    const double kSkipFreq = 1000;

    struct RandomEngine
    {
        virtual ~RandomEngine(){};
        virtual double getNext() = 0;
    };

    struct GaussianRandom : public RandomEngine
    {
        GaussianRandom() : _engine(std::random_device()()), _dist(0, 1){};
        double getNext() override
        {
            return _dist(_engine);
        }

        std::random_device _rd;
        std::mt19937_64 _engine;
        std::normal_distribution<double> _dist;
    };

    struct FakeRandom : public RandomEngine
    {
        double getNext() override
        {
            return 0;
        }
    };

    GaussianRandom gaussRand;
    FakeRandom fakeRand;

    TEST(SPHERE_OPERATORS, Operator_ostream)
    {
        std::stringstream ss1;
        std::stringstream ss2;
        std::stringstream ss3;
        std::stringstream ss4;
        std::stringstream ss5;
        std::stringstream ss6;

        Sphere temp1(core::Vec3(0, 0, 0), 4);
        Sphere temp2(core::Vec3(1, 0, 1), 2);
        Sphere temp3(core::Vec3(6, 5, 4), 3);
        Sphere temp4(core::Vec3(7, 8, 9), 10);
        Sphere temp5(core::Vec3(14, 13, 12), 11);
        Sphere temp6(core::Vec3(15, 16, 17), 18);

        ss1 << temp1;
        ss2 << temp2;
        ss3 << temp3;
        ss4 << temp4;
        ss5 << temp5;
        ss6 << temp6;

        ASSERT_EQ(ss1.str(), "Sphere origin: Vec3(0, 0, 0), radius: 4 [m]");
        ASSERT_EQ(ss2.str(), "Sphere origin: Vec3(1, 0, 1), radius: 2 [m]");
        ASSERT_EQ(ss3.str(), "Sphere origin: Vec3(6, 5, 4), radius: 3 [m]");
        ASSERT_EQ(ss4.str(), "Sphere origin: Vec3(7, 8, 9), radius: 10 [m]");
        ASSERT_EQ(ss5.str(), "Sphere origin: Vec3(14, 13, 12), radius: 11 [m]");
        ASSERT_EQ(ss6.str(), "Sphere origin: Vec3(15, 16, 17), radius: 18 [m]");
    }

    TEST(SPHERE_METHOD, Normal)
    {
        Sphere tempSphere(core::Vec3(0, 0, 0), 1);

        core::Vec3 normal1 = tempSphere.normal(core::Vec3(0, 0, -1));
        core::Vec3 normal2 = tempSphere.normal(core::Vec3(0, 0, 1));
        core::Vec3 normal3 = tempSphere.normal(core::Vec3(0, 1, 0));
        core::Vec3 normal4 = tempSphere.normal(core::Vec3(0, -1, 0));

        ASSERT_EQ(normal1, core::Vec3(0, 0, -1));
        ASSERT_EQ(normal2, core::Vec3(0, 0, 1));
        ASSERT_EQ(normal3, core::Vec3(0, 1, 0));
        ASSERT_EQ(normal4, core::Vec3(0, -1, 0));
    }

    TEST(SPHERE_METHOD, Getters_and_Setters)
    {
        Sphere object1(core::Vec3(0, 0, 0), 0.5);
        Sphere object2(core::Vec3(1, 2, 1), 5);
        Sphere object3(core::Vec3(1, 3, 2), 123);
        Sphere object4(core::Vec3(6, -1, 2), 8);
        Sphere object5(core::Vec3(1, 43, 3), 4);
        Sphere object6(core::Vec3(12, -12, 1), 0.3);

        ASSERT_EQ(core::Vec3(0, 0, 0), object1.getOrigin());
        ASSERT_EQ(core::Vec3(1, 2, 1), object2.getOrigin());
        ASSERT_EQ(core::Vec3(1, 3, 2), object3.getOrigin());
        ASSERT_EQ(core::Vec3(6, -1, 2), object4.getOrigin());
        ASSERT_EQ(core::Vec3(1, 43, 3), object5.getOrigin());
        ASSERT_EQ(core::Vec3(12, -12, 1), object6.getOrigin());

        ASSERT_EQ(0.5, object1.getRadius());
        ASSERT_EQ(5, object2.getRadius());
        ASSERT_EQ(123, object3.getRadius());
        ASSERT_EQ(8, object4.getRadius());
        ASSERT_EQ(4, object5.getRadius());
        ASSERT_EQ(0.3, object6.getRadius());
    }

    TEST(SPHERE_METHOD, HitObject_VEC3_0_0_1) // MonteCarlo test: https://en.wikipedia.org/wiki/Monte_Carlo_method
    {
        std::random_device rd;
        std::mt19937_64 e2(rd());
        std::uniform_real_distribution<double> dist(0, 2);

        double radius1 = 0.2;
        Sphere object1(core::Vec3(1, 1, 20), radius1);

        double radius2 = 0.3;
        Sphere object2(core::Vec3(1, 1, 20), radius2);

        double radius3 = 0.4;
        Sphere object3(core::Vec3(1, 1, 20), radius3);

        const double rayNum = 1000000;

        double hits1 = 0;
        double hits2 = 0;
        double hits3 = 0;

        const double reference1 = constants::kPi * radius1 * radius1 / 4;
        const double reference2 = constants::kPi * radius2 * radius2 / 4;
        const double reference3 = constants::kPi * radius3 * radius3 / 4;

        for (size_t n = 0; n < rayNum; n++)
        {
            core::Vec3 rayPosition(dist(e2), dist(e2), -50);
            core::Ray tempRay(rayPosition, core::Vec3(0, 0, 1));
            core::RayHitData hitData1, hitData2, hitData3;
            if (object1.hitObject(tempRay, kSkipFreq, &hitData1))
            {
                hits1++;
            }
            if (object2.hitObject(tempRay, kSkipFreq, &hitData2))
            {
                hits2++;
            }
            if (object3.hitObject(tempRay, kSkipFreq, &hitData3))
            {
                hits3++;
            }
        }

        ASSERT_NEAR(reference1, hits1 / rayNum, constants::kHitAccuracy * 10);
        ASSERT_NEAR(reference2, hits2 / rayNum, constants::kHitAccuracy * 10);
        ASSERT_NEAR(reference3, hits3 / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(SPHERE_METHOD, HitObject_VEC3_0_0_minus1) // MonteCarlo test: https://en.wikipedia.org/wiki/Monte_Carlo_method
    {
        std::random_device rd;
        std::mt19937_64 e2(rd());
        std::uniform_real_distribution<double> dist(0, 2);

        double radius1 = 0.2;
        Sphere object1(core::Vec3(1, 1, -20), radius1);

        double radius2 = 0.3;
        Sphere object2(core::Vec3(1, 1, -20), radius2);

        double radius3 = 0.4;
        Sphere object3(core::Vec3(1, 1, -20), radius3);

        const double rayNum = 1000000;

        double hits1 = 0;
        double hits2 = 0;
        double hits3 = 0;

        const double reference1 = constants::kPi * radius1 * radius1 / 4;
        const double reference2 = constants::kPi * radius2 * radius2 / 4;
        const double reference3 = constants::kPi * radius3 * radius3 / 4;
        const double tempFreq = 1000;

        for (size_t n = 0; n < rayNum; n++)
        {
            core::Vec3 rayPosition(dist(e2), dist(e2), 50);
            core::Ray tempRay(rayPosition, core::Vec3(0, 0, -1));
            core::RayHitData hitData1, hitData2, hitData3;
            if (object1.hitObject(tempRay, kSkipFreq, &hitData1))
            {
                hits1++;
            }
            if (object2.hitObject(tempRay, kSkipFreq, &hitData2))
            {
                hits2++;
            }
            if (object3.hitObject(tempRay, kSkipFreq, &hitData3))
            {
                hits3++;
            }
        }

        ASSERT_NEAR(reference1, hits1 / rayNum, constants::kHitAccuracy * 10);
        ASSERT_NEAR(reference2, hits2 / rayNum, constants::kHitAccuracy * 10);
        ASSERT_NEAR(reference3, hits3 / rayNum, constants::kHitAccuracy * 10);
    }
    TEST(SPHERE_METHOD, HitObject_VEC3_0_1_0) // MonteCarlo test: https://en.wikipedia.org/wiki/Monte_Carlo_method
    {
        std::random_device rd;
        std::mt19937_64 e2(rd());
        std::uniform_real_distribution<double> dist(0, 2);

        double radius1 = 0.2;
        Sphere object1(core::Vec3(1, 20, 1), radius1);

        double radius2 = 0.3;
        Sphere object2(core::Vec3(1, 20, 1), radius2);

        double radius3 = 0.4;
        Sphere object3(core::Vec3(1, 20, 1), radius3);

        const double rayNum = 1000000;

        double hits1 = 0;
        double hits2 = 0;
        double hits3 = 0;

        const double reference1 = constants::kPi * radius1 * radius1 / 4;
        const double reference2 = constants::kPi * radius2 * radius2 / 4;
        const double reference3 = constants::kPi * radius3 * radius3 / 4;

        for (size_t n = 0; n < rayNum; n++)
        {
            core::Vec3 rayPosition(dist(e2), -50, dist(e2));
            core::Ray tempRay(rayPosition, core::Vec3(0, 1, 0));
            core::RayHitData hitData1, hitData2, hitData3;
            if (object1.hitObject(tempRay, kSkipFreq, &hitData1))
            {
                hits1++;
            }
            if (object2.hitObject(tempRay, kSkipFreq, &hitData2))
            {
                hits2++;
            }
            if (object3.hitObject(tempRay, kSkipFreq, &hitData3))
            {
                hits3++;
            }
        }

        ASSERT_NEAR(reference1, hits1 / rayNum, constants::kHitAccuracy * 10);
        ASSERT_NEAR(reference2, hits2 / rayNum, constants::kHitAccuracy * 10);
        ASSERT_NEAR(reference3, hits3 / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(SPHERE_METHOD, HitObject_VEC3_0_minus1_0) // MonteCarlo test: https://en.wikipedia.org/wiki/Monte_Carlo_method
    {
        std::random_device rd;
        std::mt19937_64 e2(rd());
        std::uniform_real_distribution<double> dist(0, 2);

        double radius1 = 0.2;
        Sphere object1(core::Vec3(1, -20, 1), radius1);

        double radius2 = 0.3;
        Sphere object2(core::Vec3(1, -20, 1), radius2);

        double radius3 = 0.4;
        Sphere object3(core::Vec3(1, -20, 1), radius3);

        const double rayNum = 1000000;

        double hits1 = 0;
        double hits2 = 0;
        double hits3 = 0;

        const double reference1 = constants::kPi * radius1 * radius1 / 4;
        const double reference2 = constants::kPi * radius2 * radius2 / 4;
        const double reference3 = constants::kPi * radius3 * radius3 / 4;

        for (size_t n = 0; n < rayNum; n++)
        {
            core::Vec3 rayPosition(dist(e2), 50, dist(e2));
            core::Ray tempRay(rayPosition, core::Vec3(0, -1, 0));
            core::RayHitData hitData1, hitData2, hitData3;
            if (object1.hitObject(tempRay, kSkipFreq, &hitData1))
            {
                hits1++;
            }
            if (object2.hitObject(tempRay, kSkipFreq, &hitData2))
            {
                hits2++;
            }
            if (object3.hitObject(tempRay, kSkipFreq, &hitData3))
            {
                hits3++;
            }
        }

        ASSERT_NEAR(reference1, hits1 / rayNum, constants::kHitAccuracy * 10);
        ASSERT_NEAR(reference2, hits2 / rayNum, constants::kHitAccuracy * 10);
        ASSERT_NEAR(reference3, hits3 / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(SPHERE_METHOD, HitObject_VEC3_1_0_0) // MonteCarlo test: https://en.wikipedia.org/wiki/Monte_Carlo_method
    {
        std::random_device rd;
        std::mt19937_64 e2(rd());
        std::uniform_real_distribution<double> dist(0, 2);

        double radius1 = 0.2;
        Sphere object1(core::Vec3(20, 1, 1), radius1);

        double radius2 = 0.3;
        Sphere object2(core::Vec3(20, 1, 1), radius2);

        double radius3 = 0.4;
        Sphere object3(core::Vec3(20, 1, 1), radius3);

        const double rayNum = 1000000;

        double hits1 = 0;
        double hits2 = 0;
        double hits3 = 0;

        const double reference1 = constants::kPi * radius1 * radius1 / 4;
        const double reference2 = constants::kPi * radius2 * radius2 / 4;
        const double reference3 = constants::kPi * radius3 * radius3 / 4;
        for (size_t n = 0; n < rayNum; n++)
        {
            core::Vec3 rayPosition(-50, dist(e2), dist(e2));
            core::Ray tempRay(rayPosition, core::Vec3(1, 0, 0));
            core::RayHitData hitData1, hitData2, hitData3;
            if (object1.hitObject(tempRay, kSkipFreq, &hitData1))
            {
                hits1++;
            }
            if (object2.hitObject(tempRay, kSkipFreq, &hitData2))
            {
                hits2++;
            }
            if (object3.hitObject(tempRay, kSkipFreq, &hitData3))
            {
                hits3++;
            }
        }

        ASSERT_NEAR(reference1, hits1 / rayNum, constants::kHitAccuracy * 10);
        ASSERT_NEAR(reference2, hits2 / rayNum, constants::kHitAccuracy * 10);
        ASSERT_NEAR(reference3, hits3 / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(SPHERE_METHOD, HitObject_VEC3_minus1_0_0) // MonteCarlo test: https://en.wikipedia.org/wiki/Monte_Carlo_method
    {
        std::random_device rd;
        std::mt19937_64 e2(rd());
        std::uniform_real_distribution<double> dist(0, 2);

        double radius1 = 0.2;
        Sphere object1(core::Vec3(-20, 1, 1), radius1);

        double radius2 = 0.3;
        Sphere object2(core::Vec3(-20, 1, 1), radius2);

        double radius3 = 0.4;
        Sphere object3(core::Vec3(-20, 1, 1), radius3);

        const double rayNum = 1000000;

        double hits1 = 0;
        double hits2 = 0;
        double hits3 = 0;

        const double reference1 = constants::kPi * radius1 * radius1 / 4;
        const double reference2 = constants::kPi * radius2 * radius2 / 4;
        const double reference3 = constants::kPi * radius3 * radius3 / 4;
        for (size_t n = 0; n < rayNum; n++)
        {
            core::Vec3 rayPosition(50, dist(e2), dist(e2));
            core::Ray tempRay(rayPosition, core::Vec3(-1, 0, 0));
            core::RayHitData hitData1, hitData2, hitData3;
            if (object1.hitObject(tempRay, kSkipFreq, &hitData1))
            {
                hits1++;
            }
            if (object2.hitObject(tempRay, kSkipFreq, &hitData2))
            {
                hits2++;
            }
            if (object3.hitObject(tempRay, kSkipFreq, &hitData3))
            {
                hits3++;
            }
        }

        ASSERT_NEAR(reference1, hits1 / rayNum, constants::kHitAccuracy * 10);
        ASSERT_NEAR(reference2, hits2 / rayNum, constants::kHitAccuracy * 10);
        ASSERT_NEAR(reference3, hits3 / rayNum, constants::kHitAccuracy * 10);
    }

    TEST(SPHERE_METHODS, Hit_Object_RayHitData_Vec3_0_0_1)
    {
        using namespace core;

        std::vector<Sphere> objectList = {Sphere(Vec3(0, 0, 0), 1), Sphere(Vec3(0, 0, 4), 1)};
        Ray tempRay(Vec3(0, 0, -5), Vec3(0, 0, 1));
        RayHitData hitData;
        for (auto &obj : objectList)
        {
            ASSERT_TRUE(obj.hitObject(tempRay, kSkipFreq, &hitData));
            ASSERT_EQ(hitData.time, std::abs(obj.getOrigin().z() - obj.getRadius() - tempRay.getOrigin().z()));
            ASSERT_EQ(hitData.collisionPoint, tempRay.at(std::abs(obj.getOrigin().z() - obj.getRadius() - tempRay.getOrigin().z())));
        }
    }

    TEST(SPHERE_METHODS, Ray_hit_Data_Properties)
    {
        core::Ray tempRay(core::Vec3(0, 0, 5), core::Vec3(0, 0, -1));
        Sphere object1(core::Vec3(0, 0, 0), 1);
        core::RayHitData hitData;

        ASSERT_TRUE(object1.hitObject(tempRay, kSkipFreq, &hitData));
        ASSERT_EQ(core::Vec3(0, 0, -1), hitData.direction);
        ASSERT_EQ(core::Vec3(0, 0, 1), hitData.collisionPoint);
        ASSERT_EQ(core::Vec3(0, 0, 5), hitData.origin);
        ASSERT_EQ(4, hitData.time);
    }

    TEST(SPHERE_CONSTRUCTOR, All_Possible_Constructors)
    {
        Sphere object1;

        ASSERT_EQ(1, object1.getRadius());
        ASSERT_EQ(core::Vec3(0, 0, 0), object1.getOrigin());

        Sphere object2(core::Vec3(0, 0, 1), 4);

        ASSERT_EQ(4, object2.getRadius());
        ASSERT_EQ(core::Vec3(0, 0, 1), object2.getOrigin());
    }

    TEST(SPHEREWALL_CONSTRUCTOR, All_Possible_Constructors)
    {
        SphereWall object1;

        ASSERT_EQ(4.0, object1.getRadius());
        ASSERT_EQ(core::Vec3(0, 0, 0), object1.getOrigin());
    }

    TEST(SPHEREWALL_METHOD, Normal)
    {
        SphereWall tempSphere;

        core::Vec3 normal1 = tempSphere.normal(core::Vec3(0, 0, -constants::kSimulationRadius));
        core::Vec3 normal2 = tempSphere.normal(core::Vec3(0, 0, constants::kSimulationRadius));
        core::Vec3 normal3 = tempSphere.normal(core::Vec3(0, constants::kSimulationRadius, 0));
        core::Vec3 normal4 = tempSphere.normal(core::Vec3(0, -constants::kSimulationRadius, 0));

        ASSERT_EQ(normal1, core::Vec3(0, 0, -1));
        ASSERT_EQ(normal2, core::Vec3(0, 0, 1));
        ASSERT_EQ(normal3, core::Vec3(0, 1, 0));
        ASSERT_EQ(normal4, core::Vec3(0, -1, 0));
    }

    TEST(SPHEREWALL_METHOD, Getters_and_Setters)
    {
        SphereWall object1;

        ASSERT_EQ(core::Vec3(0, 0, 0), object1.getOrigin());
        ASSERT_EQ(4, object1.getRadius());
    }

    TEST(SPHEREWALL_OPERATORS, Operator_ostream)
    {
        std::stringstream ss1;
        SphereWall temp1;
        ss1 << temp1;
        ASSERT_EQ(ss1.str(), "SphereWall origin: Vec3(0, 0, 0), radius: 4 [m]");
    }

    TEST(SphereWallObjectTest, RayInsideSphereHit)
    {
        Vec3 kO(123, 1, 2);
        const double kR = 3;
        Sphere sphere(kO, kR);

        RayHitData ignore;

        Vec3 dir(1, 2, 3);
        Ray rayInsideSphere(sphere.getOrigin(), dir);
        ASSERT_TRUE(sphere.hitObject(rayInsideSphere, kSkipFreq, &ignore))
            << "Ray Hit didn't occur.";

        Ray rayOutsideSphere(sphere.getOrigin() + Vec3(2 * kR, 0, 0), dir);
        ASSERT_FALSE(sphere.hitObject(rayOutsideSphere, kSkipFreq, &ignore))
            << "Ray hit when it shouldn't";
    }
} // namespace objects
