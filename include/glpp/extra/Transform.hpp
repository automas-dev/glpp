#pragma once

#include <glm/ext/quaternion_float.hpp>
#include <glm/glm.hpp>

namespace glpp::extra {
    /**
     * Single transform in 3d space with position, rotation and scale.
     */
    class Transform {
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;

        mutable glm::mat4 matrix;
        mutable bool changed;

    public:
        /**
         * Create a default Transform with position and rotation set to all
         * zeros and scale set to all ones.
         */
        Transform();

        /**
         * Create a Transform from the component vectors.
         *
         * @param position the position vector
         * @param rotation the rotation quaternion
         * @param scale the scale vector
         */
        Transform(const glm::vec3 & position,
                  const glm::quat & rotation,
                  const glm::vec3 & scale);

        /**
         * Create a Transform from an existing matrix. The matrix will be
         * decomposed into it's components discarding skew and perspective.
         *
         * @param matrix the input transform matrix
         */
        Transform(const glm::mat4 & matrix);

        Transform(const Transform & other) = default;

        Transform(Transform && other) = default;

        Transform & operator=(const Transform & other) = default;

        Transform & operator=(Transform && other) = default;

        ~Transform();

        /**
         * Move the position by adding delta to it.
         *
         * @param delta the delta position
         */
        void move(const glm::vec3 & delta);

        /**
         * Rotate using a Euler rotation vector. This will be converted to
         * a quaternion before being applied.
         *
         * @param delta the delta euler rotation
         */
        void rotateEuler(const glm::vec3 & delta);

        /**
         * Rotate using a rotation quaternion.
         *
         * @param delta the delta rotation
         */
        void rotate(const glm::quat & delta);

        /**
         * Scale by multiplying it with the scale parameter.
         *
         * @param scale the scale to multiply by
         */
        void scale(const glm::vec3 & scale);

        /**
         * Get the current position as a glm::vec3.
         *
         * @return the current position
         */
        const glm::vec3 & getPosition() const;

        /**
         * Set the current position.
         *
         * @param position the new position
         */
        void setPosition(const glm::vec3 & position);

        /**
         * Get the current rotation as a Euler vector.
         *
         * @return the current euler rotation
         */
        glm::vec3 getRotationEuler() const;

        /**
         * Get the current rotation as a quaternion.
         *
         * @return the current rotation
         */
        const glm::quat & getRotation() const;

        /**
         * Set the current rotation.
         *
         * @param rotation the new quaternion rotation
         */
        void setRotation(const glm::quat & rotation);

        /**
         * Get the current scale.
         *
         * @return the current scale
         */
        const glm::vec3 & getScale() const;

        /**
         * Set the current scale.
         *
         * @param scale the new scale
         */
        void setScale(const glm::vec3 & scale);

        /**
         * Calculate the transform matrix. The matrix will be cached for future
         * calls until one of position, rotation, scale is updated.
         *
         * @return the transform matrix
         */
        const glm::mat4 & toMatrix() const;
    };
}
