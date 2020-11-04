#include "constants.h"
#include "core/ray.h"
#include "core/vec3.h"
#include "obj/objects.h"
#include "gtest/gtest.h" // https://google.github.io/styleguide/cppguide.html#Namespaces

#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

using core::Ray;
using core::RayHitData;
using core::Vec3;

namespace objects {

const float kSkipFreq = 1000;
// This test is kind of useless anyway, but hey...
TEST(TriangleObjTest, Printing) {
  {
    std::stringstream ss;
    ss << TriangleObj(core::Vec3(std::sqrt(2.0f), 0, 0), core::Vec3(1, 2, 3),
                      core::Vec3(-4, -5, -6));
    ASSERT_EQ(ss.str(), "Triangle Object, vertex: Vec3(1.41421, 0, 0), Vec3(1, "
                        "2, 3), Vec3(-4, -5, -6)");
  }

  // This test is useless, it is here just for demonstration purposes that with
  // braces I could reuse "ss".
  {
    std::stringstream ss;
    ss << TriangleObj();
    ASSERT_EQ(
        ss.str(),
        "Triangle Object, vertex: Vec3(1, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1)");
  }
}

// If ray has an origin inside the triangle then it never hits, as we try to
// avoid hitting the same object twice.
TEST(TriangleObjectTest, RayWithOriginInsideDoNotHit) {
  // 1. Define some constant data points and name them nicely, so that you can
  // just reuse them but not rewrite them every time. What if you make a mistake
  // in the numbers and then you spend hours trying to figure out where is the
  // error?
  //
  // 2. Try to define variables just before you use them.
  // https://stackoverflow.com/questions/17386326/what-downsides-is-declaring-variables-just-before-first-time-use-it
  // https://wiki.c2.com/?DeclareVariablesAtFirstUse
  Vec3 kA(1, 2, 3);
  Vec3 kB(4, 5, 2);
  Vec3 kC(-2, -2, 1);
  TriangleObj triangle(kA, kB, kC);

  // Naming is key, this way the asserts below read nicely.
  // Alternatively, allow to pass nullptr in hitObject.
  RayHitData ignore;

  // Group the code so that it has:
  // 1. Data preparation
  // 2. Call method under test
  // 3. Asserts (all you need for this specific test).
  //
  // This way you can focus on this one specific test case, as opposed to first
  // read all the data you prepare for all other tests, and then see the methods
  // under test and then asserts you, and then you need to jump back and fourth
  // between data and checks to make sure it works... Nightmare.
  //
  // The worst example of this is the first test with ostream operator.
  //
  // References:
  // https://blogs.agilefaqs.com/2010/11/14/single-assert-per-unit-test-myth/
  // https://www.agilealliance.org/glossary/gwt/
  //
  // Side note: Thanks to my clever initialization, I am sure this is inside the
  // triangle regardless of what is A, B, C. I am not limited to simple cases!
  //
  // This defines just the center of the triangle, but more general you have
  // Barycentric coordinates [1], so you could just generate A, B, C, and
  // generate random point on a triangle and always check if this is true.
  //
  // [1]
  // https://en.wikipedia.org/wiki/Barycentric_coordinate_system#Barycentric_coordinates_on_triangles
  Ray rayOppositeNormal((kA + kB + kC) / 3.0, -triangle.normal());
  ASSERT_FALSE(triangle.hitObject(rayOppositeNormal, kSkipFreq, &ignore));

  // Here I am constructing this object in a non trivial way so I need to also
  // check if my assumption that this ray has a perperpendicular normal checks
  // out. Even if the constrution is trivial, just putting numbers in place
  // still check if you assumptions hold. You made this mistake in the past,
  // multiple times, e.g when triangle was really a line, so assumptions were
  // wrong, and you spend coutless hours debugging.
  {
    Ray rayPerpendicularNormal((kA + kB + kC) / 3.0,
                               triangle.normal().crossProduct(kA - kB));
    ASSERT_NEAR(
        0,
        rayPerpendicularNormal.getDirection().scalarProduct(triangle.normal()),
        0.001f)
        << "Ray is not perpendicular to the triangle normal, but it should be";
    ASSERT_FALSE(
        triangle.hitObject(rayPerpendicularNormal, kSkipFreq, &ignore));
  }

  // Sometimes it is a good idea to wrap test cases with braces like this. This
  // way:
  // 1. you have some visual separation of the test cases. You can also add
  // comments explaining each case instead, saying, why you want this to happen.
  // 2. if you want to reuse the same variables you used earlier (see Printing
  // test).
  {
    Ray raySamDirectionNormal((kA + kB + kC) / 3.0, triangle.normal());
    ASSERT_FALSE(triangle.hitObject(raySamDirectionNormal, kSkipFreq, &ignore));
  }
}

// This is how it looks without comments.
TEST(TriangleObjectTest, RayWithOriginInsideDoNotHit2) {
  Vec3 kA(1, 2, 3);
  Vec3 kB(4, 5, 2);
  Vec3 kC(-2, -2, 1);
  TriangleObj triangle(kA, kB, kC);

  RayHitData ignore;

  Ray rayOppositeNormal((kA + kB + kC) / 3.0, -triangle.normal());
  ASSERT_FALSE(triangle.hitObject(rayOppositeNormal, kSkipFreq, &ignore));

  Ray raySamDirectionNormal((kA + kB + kC) / 3.0, triangle.normal());
  ASSERT_FALSE(triangle.hitObject(raySamDirectionNormal, kSkipFreq, &ignore));

  Ray rayPerpendicularNormal((kA + kB + kC) / 3.0,
                             triangle.normal().crossProduct(kA - kB));
  ASSERT_NEAR(
      0, rayPerpendicularNormal.getDirection().scalarProduct(triangle.normal()),
      0.001f)
      << "Ray is not perpendicular to the triangle normal, but it should be";
  ASSERT_FALSE(triangle.hitObject(rayPerpendicularNormal, kSkipFreq, &ignore));
}

}; // namespace objects
