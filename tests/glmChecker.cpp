#include "glmChecker.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <ostream>

using glm::mat4;
using glm::quat;
using glm::vec3;
using std::ostream;

// static ostream & operator<<(ostream & os, const quat & q) {
//     return os << glm::to_string(q);
//     // return os << q.w << " " << q.x << " " << q.y << " " << q.z;
// }

// static ostream & operator<<(ostream & os, const vec3 & q) {
//     return os << glm::to_string(q);
//     // return os << q.x << " " << q.y << " " << q.z;
// }

// static ostream & operator<<(ostream & os, const mat4 & q) {
//     return os << glm::to_string(q);
// }

static bool glmFloatEqual(float a, float b, float t = 0.00001) {
    float c = a - b;
    return c <= t && c >= -t;
}

static bool glmFloatEqual(double a, double b, double t = 0.00001) {
    double c = a - b;
    return c <= t && c >= -t;
}

testing::AssertionResult GLMEqual(const glm::vec3 & a, const glm::vec3 & b) {
    if (!glmFloatEqual(a.x, b.x))
        return testing::AssertionFailure() << "a.x " << a.x << " != b.x " << b.x;

    if (!glmFloatEqual(a.y, b.y))
        return testing::AssertionFailure() << "a.y " << a.y << " != b.y " << b.y;

    if (!glmFloatEqual(a.z, b.z))
        return testing::AssertionFailure() << "a.z " << a.z << " != b.z " << b.z;

    return testing::AssertionSuccess();
}

testing::AssertionResult GLMEqual(const glm::quat & a, const glm::quat & b) {
    if (!glmFloatEqual(a.w, b.w))
        return testing::AssertionFailure() << "a.w " << a.w << " != b.w " << b.w;

    if (!glmFloatEqual(a.x, b.x))
        return testing::AssertionFailure() << "a.x " << a.x << " != b.x " << b.x;

    if (!glmFloatEqual(a.y, b.y))
        return testing::AssertionFailure() << "a.y " << a.y << " != b.y " << b.y;

    if (!glmFloatEqual(a.z, b.z))
        return testing::AssertionFailure() << "a.z " << a.z << " != b.z " << b.z;

    return testing::AssertionSuccess();
}

static bool glmVec3Equal(const glm::vec3 & a, const glm::vec3 & b) {
    return glmFloatEqual(a[0], b[0]) && glmFloatEqual(a[1], b[1])
           && glmFloatEqual(a[2], b[2]);
}

testing::AssertionResult GLMEqual(const glm::mat4 & a, const glm::mat4 & b) {
    if (!glmVec3Equal(a[0], b[0]))
        return testing::AssertionFailure() << "a[0] " << glm::to_string(a[0])
                                           << " != b[0] " << glm::to_string(b[0]);

    if (!glmVec3Equal(a[1], b[1]))
        return testing::AssertionFailure() << "a[1] " << glm::to_string(a[1])
                                           << " != b[1] " << glm::to_string(b[1]);

    if (!glmVec3Equal(a[2], b[2]))
        return testing::AssertionFailure() << "a[2] " << glm::to_string(a[2])
                                           << " != b[2] " << glm::to_string(b[2]);

    if (!glmVec3Equal(a[3], b[3]))
        return testing::AssertionFailure() << "a[3] " << glm::to_string(a[3])
                                           << " != b[3] " << glm::to_string(b[3]);

    return testing::AssertionSuccess();
}
