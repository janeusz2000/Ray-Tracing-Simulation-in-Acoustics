load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
    name = "utils",
    srcs = [
        "core/ray.cpp",
        "core/vec3.cpp",
    ],
    hdrs = [
        "constants.h",
        "core/exceptions.h",
        "core/ray.h",
        "core/vec3.h",
    ],
    copts = ["/std:c++17"],
)

cc_test(
    name = "vec3_test",
    srcs = [
        "tests/vec3_test.cpp",
    ],
    copts = [
        "-Iexternal/gtest/include",
        "/std:c++17",
    ],
    deps = [
        ":utils",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "ray_test",
    srcs = [
        "tests/ray_test.cpp",
    ],
    copts = [
        "-Iexternal/gtest/include",
        "/std:c++17",
    ],
    deps = [
        ":utils",
        "@com_google_googletest//:gtest_main",
    ],
)
