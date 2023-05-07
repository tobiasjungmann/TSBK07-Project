#include "camera.hpp"

#include <cmath>
#include "constants.hpp"

#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) < (b) ? (b) : (a))

namespace scn
{

    evt::MovementKeys currentPressedKeys;
    evt::Mouse currentMousePosition;

    void Camera::rotateAround(Axis axis, float angle, float radius) noexcept
    {
        switch (axis)
        {
        case Axis::X:
            y = z = 0;
            break;
        case Axis::Y:
            x = z = 0;
            break;
        case Axis::Z:
            x = y = 0;
            break;
        default:
            break;
        }
        rotateRelAround(axis, angle, radius);
    }

    void Camera::rotateRelAround(Axis axis, float relativeAng, float radius) noexcept
    {
        switch (axis)
        {
        case Axis::X:
            y += cos(relativeAng) * radius;
            z += sin(relativeAng) * radius;
            break;
        case Axis::Y:
            x += sin(relativeAng) * radius;
            z += cos(relativeAng) * radius;
            break;
        case Axis::Z:
            x += cos(relativeAng) * radius;
            y += sin(relativeAng) * radius;
            break;
        default:
            break;
        }
    }

    void Camera::translate(vec3 offset) noexcept
    {
        pos += offset;
    }

    void Camera::setMousePosition(int x, int y)noexcept
    {
        currentMousePosition.update(x, y);
    }

    void Camera::setPressedKeys(bool forward,bool left,bool back,bool right)noexcept
    {
        currentPressedKeys.w = forward;
        currentPressedKeys.a = left;
        currentPressedKeys.s = back;
        currentPressedKeys.d = right;
    }

    mat4 Camera::matrix() const
    {
        return lookAt(pos, pos + viewingDirection, upVector);
    }

    std::ostream &operator<<(std::ostream &os, Camera const &camera)
    {
        os << "pos:" << camera.pos << ' ';
        os << "viewingDirection:" << camera.viewingDirection << ' ';
        os << "upVector:" << camera.up;
        return os;
    }

  float Camera::computeMinMovement(int last, int current)
    {
        int offset = last - current;
        float noMovement = (constants::display_size >> 2);
        float minMovementScaling = 0.05;

        if (last < (constants::display_size >> 1))
        {
            return min((last - (constants::display_size >> 1) + noMovement), 0.0f) * minMovementScaling;
        }
        else
        {
            return max((last - (constants::display_size >> 1) - noMovement), 0.0f) * minMovementScaling;
        }
    }

    // FIXME there should be this function in header <cmath>
    float toRadians(float degrees)
    {
        return degrees * (M_1_PI / 180);
    }

    void Camera::updateCameraPosition(const Terrain & terrain)
    {
       const float cameraSpeed = 0.5f;
        float sensitivity = 0.7f;
       if (currentPressedKeys.w)
        {
            pos += cameraSpeed * viewingDirection;
        }
        if (currentPressedKeys.s)
        {
            pos -= cameraSpeed * viewingDirection;
        }
        if (currentPressedKeys.a)
        {
            pos -= normalize(cross(viewingDirection, up)) * cameraSpeed;
        }
        if (currentPressedKeys.d)
        {
            pos += normalize(cross(viewingDirection, up)) * cameraSpeed;
        }

        float xmovement = computeMinMovement(currentMousePosition.previousPosition.x, currentMousePosition.currentPosition.x) * sensitivity;
        float ymovement = -computeMinMovement(currentMousePosition.currentPosition.y, currentMousePosition.previousPosition.y) * sensitivity;

        yaw += xmovement;
        pitch += ymovement;

        /* Add if loopings should be forbidden
        if (pitch > 179.0f)
            pitch = 179.0f;
        if (pitch < -179.0f)
            pitch = -179.0f;*/

        // TODO only compute when mouse was updated between the last two dispalys
        vec3 direction;
        direction.x = cos(toRadians(yaw)) * cos(toRadians(pitch));
        direction.y = sin(toRadians(pitch));
        direction.z = sin(toRadians(yaw)) * cos(toRadians(pitch));
        viewingDirection = normalize(direction);

        // check for collisions with the ground
        float minHeigth = terrain.computeHeight(pos.x, pos.z)+2.0;// + 0.5;        // TODO take a real hitbox size
            if (pos.y < minHeigth)
            {
                pos.y = minHeigth;
            }
    }
}