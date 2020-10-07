#ifndef GENERATORS_TEST_CPP
#define GENERATORS_TEST_CPP

#include "gtest/gtest.h"
#include "obj/generators.h"
namespace generators
{

    TEST(POINTSOURCE_CONSTRUCTOR, Test_All_Possible_Constructors)
    {
        ASSERT_EQ(PointSource(100, 1000, 1), PointSource(100, 1000, 1));
    }

} // namespace generators
#endif