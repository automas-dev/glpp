#include <gtest/gtest.h>

#include <glm/glm.hpp>
#include <glpp/extra/Transform.hpp>

#include "glTest.hpp"

namespace {
    using glm::mat4;
    using glm::quat;
    using glm::vec3;

    TEST(GLMTest, VecEqIsExpected) {
        vec3 a(1, 2, 3);
        vec3 b(1, 2, 3);
        EXPECT_GLM_EQUAL(a, b);
    }

    TEST(GLMTest, VecEqIsNotExpected) {
        vec3 a(1, 2, 3);
        EXPECT_GLM_NEQUAL(a, vec3(0, 2, 3));
        EXPECT_GLM_NEQUAL(a, vec3(1, 0, 3));
        EXPECT_GLM_NEQUAL(a, vec3(1, 2, 0));
    }

    TEST(GLMTest, QuatEqIsExpected) {
        quat a(1, 2, 3, 4);
        quat b(1, 2, 3, 4);
        EXPECT_GLM_EQUAL(a, b);
    }

    TEST(GLMTest, QuatEqIsNotExpected) {
        quat a(1, 2, 3, 4);
        EXPECT_GLM_NEQUAL(a, quat(0, 2, 3, 4));
        EXPECT_GLM_NEQUAL(a, quat(1, 0, 3, 4));
        EXPECT_GLM_NEQUAL(a, quat(1, 2, 0, 4));
        EXPECT_GLM_NEQUAL(a, quat(1, 2, 3, 0));
    }

    TEST(GLMTest, MatEqIsExpected) {
        mat4 a({1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16});
        mat4 b({1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16});
        EXPECT_GLM_EQUAL(a, b);
    }

    TEST(GLMTest, MatEqIsNotExpected) {
        mat4 a({1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16});
        EXPECT_GLM_NEQUAL(a, mat4({0, 0, 0, 0}, {5, 6, 7, 8}, {9, 10, 11, 12},
                                  {13, 14, 15, 16}));
        EXPECT_GLM_NEQUAL(a, mat4({1, 2, 3, 4}, {0, 0, 0, 0}, {9, 10, 11, 12},
                                  {13, 14, 15, 16}));
        EXPECT_GLM_NEQUAL(
            a, mat4({1, 2, 3, 4}, {5, 6, 7, 8}, {0, 0, 0, 0}, {13, 14, 15, 16}));
        EXPECT_GLM_NEQUAL(
            a, mat4({1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {0, 0, 0, 0}));
    }
}
