#include <gtest/gtest.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glpp/extra/Transform.hpp>
#include <iostream>
#include <stdexcept>

#include "glTest.hpp"

namespace {
    using glm::mat4;
    using glm::quat;
    using glm::vec3;
    using glpp::extra::Transform;

    /**
     * Empty constructor should have default position and rotation of 0 with
     * default scale of 1.
     */
    TEST(TransformTest, EmptyConstructor) {
        Transform t;
        EXPECT_GLM_EQUAL(t.getPosition(), vec3(0, 0, 0));
        EXPECT_GLM_EQUAL(t.getRotationEuler(), vec3(0, 0, 0));
        EXPECT_GLM_EQUAL(t.getScale(), vec3(1, 1, 1));
    }

    /**
     * Component constructor should not modify components.
     */
    TEST(TransformTest, ComponentsConstructor) {
        vec3 pos(1, 2, 3);
        vec3 rot(0.4, 0.5, 0.6);
        quat qRot(rot);
        vec3 scale(7, 8, 9);
        Transform t(pos, qRot, scale);
        EXPECT_GLM_EQUAL(t.getPosition(), pos);
        EXPECT_GLM_EQUAL(t.getRotation(), qRot);
        // EXPECT_GLM_EQUAL(t.getRotationEuler(), rot);
        EXPECT_GLM_EQUAL(t.getScale(), scale);
    }

    static void printQuat(const quat & q) {
        std::cout << q.w << " " << q.x << " " << q.y << " " << q.z << std::endl;
    }

    /**
     * Matrix constructor should decompose into components without further
     * modification.
     */
    TEST(TransformTest, MatrixConstructor) {
        vec3 pos(1, 2, 3);
        vec3 rot(0.4, 0.5, 0.6);
        quat qRot(rot);
        vec3 scale(7, 8, 9);
        auto mTranslate = glm::translate(glm::mat4(1), pos);
        auto mRotate = glm::mat4(qRot);
        auto mScale = glm::scale(glm::mat4(1), scale);
        mat4 matrix = mTranslate * mRotate * mScale;

        Transform t(matrix);
        EXPECT_GLM_EQUAL(t.getPosition(), pos);
        EXPECT_GLM_EQUAL(t.getRotation(), qRot);
        EXPECT_GLM_EQUAL(t.getRotationEuler(), rot);
        EXPECT_GLM_EQUAL(t.getScale(), scale);
        EXPECT_GLM_EQUAL(t.toMatrix(), matrix);
    }

    /**
     * Matrix constructor should decompose into components without further
     * modification. Calculated matrix should match the input matrix.
     */
    TEST(TransformTest, MatrixConstructor2) {
        vec3 pos(1, 2, 3);
        vec3 rot(0.4, 0.5, 0.6);
        quat qRot(rot);
        vec3 scale(7, 8, 9);
        Transform t1(pos, qRot, scale);
        Transform t2(t1.toMatrix());

        EXPECT_GLM_EQUAL(t2.getPosition(), pos);
        EXPECT_GLM_EQUAL(t2.getRotation(), qRot);
        EXPECT_GLM_EQUAL(t2.getRotationEuler(), rot);
        EXPECT_GLM_EQUAL(t2.getScale(), scale);
        EXPECT_GLM_EQUAL(t2.toMatrix(), t1.toMatrix());
    }

    /**
     * Getter method should return the value without modifying it.
     */
    TEST(TransformTest, Getters) {
        vec3 pos(1, 2, 3);
        vec3 rot(0.4, 0.5, 0.6);
        quat qRot(rot);
        vec3 scale(7, 8, 9);
        Transform t(pos, qRot, scale);

        EXPECT_GLM_EQUAL(t.getPosition(), pos);
        EXPECT_GLM_EQUAL(t.getRotation(), qRot);
        EXPECT_GLM_EQUAL(t.getRotationEuler(), rot);
        EXPECT_GLM_EQUAL(t.getScale(), scale);
    }

    /**
     * Setter method should store the value without modifying it.
     */
    TEST(TransformTest, Setters) {
        Transform t;

        vec3 pos(1, 2, 3);
        t.setPosition(pos);
        EXPECT_GLM_EQUAL(t.getPosition(), pos);

        vec3 rot(0.4, 0.5, 0.6);
        quat qRot(rot);
        t.setRotation(qRot);
        EXPECT_GLM_EQUAL(t.getRotation(), qRot);
        EXPECT_GLM_EQUAL(t.getRotationEuler(), rot);

        vec3 scale(7, 8, 9);
        t.setScale(scale);
        EXPECT_GLM_EQUAL(t.getScale(), scale);
    }

    /**
     * Modifier method should update the current value instead of replacing it.
     */
    TEST(TransformTest, Modifiers) {
        vec3 pos(1, 2, 3);
        vec3 rot(0.4, 0.5, 0.6);
        quat qRot(rot);
        vec3 scale(7, 8, 9);
        Transform t(pos, qRot, scale);

        vec3 delta(0.1, 0.2, 0.3);
        quat qDelta(delta);

        t.move(delta);
        EXPECT_GLM_EQUAL(t.getPosition(), pos + delta);

        t.rotate(qDelta);
        EXPECT_GLM_EQUAL(t.getRotation(), qDelta * qRot);
        EXPECT_GLM_EQUAL(t.getRotationEuler(), glm::eulerAngles(qDelta * qRot));

        t.setRotation(qRot);
        t.rotateEuler(delta);
        EXPECT_GLM_EQUAL(t.getRotation(), qDelta * qRot);
        EXPECT_GLM_EQUAL(t.getRotationEuler(), glm::eulerAngles(qDelta * qRot));

        t.scale(delta);
        EXPECT_GLM_EQUAL(t.getScale(), scale * delta);
    }

    /**
     * Generated matrix should correctly include all components.
     */
    TEST(TransformTest, ToMatrix) {
        vec3 pos(1, 2, 3);
        vec3 rot(0.4, 0.5, 0.6);
        quat qRot(rot);
        vec3 scale(7, 8, 9);
        Transform t(pos, qRot, scale);

        auto mTranslate = glm::translate(glm::mat4(1), pos);
        auto mRotate = glm::mat4(qRot);
        auto mScale = glm::scale(glm::mat4(1), scale);
        auto matrix = mTranslate * mRotate * mScale;
        EXPECT_GLM_EQUAL(t.toMatrix(), matrix);
    }
}
