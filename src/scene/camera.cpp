#include "camera.hpp"

#include <cmath>

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

    void Camera::setMousePosition(int x, int y)
    {
        currentMousePosition.update(x, y);
    }

    void Camera::forwardPressedKeys(vec4 input)
    {
        currentPressedKeys.w = input.w;
        currentPressedKeys.a = input.x;
        currentPressedKeys.s = input.y;
        currentPressedKeys.d = input.z;
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

    /**
     * @brief Computes the minimum amount of camera rotation dependeing on the placement of the of teh mouse on the scree
     * If the current moevement is too smallcomapred to the camera position, the new minimum will be returned. Otherwise, the current
     * movement offest is returned.
     *
     * @param last last mouse cordinate (either x or y direction)
     * @param current current mouse coordniate
     * @return float
     */
    static float computeMinMovement(int last, int current)
    {
        int offset = last - current;
        int display_size = 900; // TODO load from main - resizable
        float noMovement = (display_size >> 2);
        float minMovementScaling = 0.05;

        if (last < (display_size >> 1))
        {
            float minMovement = min((last - (display_size >> 1) + noMovement), 0.0f) * minMovementScaling;
            if (minMovement < offset && offset >= 0)
            {
                return minMovement;
            }
        }
        else
        {
            float minMovement = max((last - (display_size >> 1) - noMovement), 0.0f) * minMovementScaling;
            if (minMovement > offset && offset >= 0)
            {
                return minMovement;
            }
        }
        return offset;
    }

    // FIXME there should be this function in header <cmath>
    float toRadians(float degrees)
    {
        return degrees * (M_1_PI / 180);
    }

    void Camera::updateCameraPosition() // const& prev, evt::Mouse const& curr)
    {
        //  std::cout << "mouse: x" << currentMousePosition.previousPosition.x << "  y: " << currentMousePosition.previousPosition.y;
        //  std::cout << "    curent: x" << currentMousePosition.currentPosition.x << "  y: " << currentMousePosition.currentPosition.y << "\n";
        const float cameraSpeed = 0.5f;
        float sensitivity = 0.7f;
        std::cout << "wasd " << currentPressedKeys.w << currentPressedKeys.a << currentPressedKeys.s << currentPressedKeys.d<<"\n";
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

        // std::cout << this <<"\n";
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
        /* TODO add once height is computed
        float minHeigth = computeHeight(camera.x, camera.z) + 0.5;
            if (camera.y < minHeigth)
            {
                camera.y = minHeigth;
            }*/
        // camera->lookat = lookat + camera->pos;
    }
}