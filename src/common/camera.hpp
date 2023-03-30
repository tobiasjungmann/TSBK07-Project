#pragma once

#include "VectorUtils4.h"
#include <iostream>

namespace scn {
enum class Axis {
    X,
    Y,
    Z
};

struct Camera {
    union {
        vec3 pos;
        vec3 position;
    };
    GLfloat &x {pos.x};
    GLfloat &y {pos.y};
    GLfloat &z {pos.z};
    union {
        vec3 lookat;
        vec3 at;
    };
    union {
        vec3 upVector;
        vec3 up;
    };

    
    // float &x {pos.x};
    // float &y {pos.y};
    // float &z {pos.z};

public:
    constexpr Camera(vec3 pos, vec3 lookAt, vec3 upVector) :
        position {pos}, lookat{lookAt}, upVector{upVector} {}

    constexpr Camera(Camera const& other) : pos{other.pos}, at{other.at}, up{other.up} {}

    constexpr Camera& operator=(Camera const& other) noexcept
    {
        at = other.at;
        pos = other.pos;
        up = other.up;
        return *this;
    }

    void rotateAround(Axis axis, float angle, float radius) noexcept;
    void rotateRelAround(Axis axis, float relativeAng, float radius) noexcept;
    void translate(vec3 offset) noexcept;

    mat4 matrix() const;
};
    std::ostream& operator<<(std::ostream& os, Camera const& camera);
}