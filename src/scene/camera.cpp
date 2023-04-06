#include "camera.hpp"

#include <math.h>

namespace scn
{
    int display_size = 900;     // TODO move at a better position

    bool firstMouse = true;
    float lastX = display_size / 2; // last position of the mouse
    float lastY = display_size / 2; // last position of the mouse
    float yaw = 0;                  // yaw of the camera
    float pitch = 0;                // pitch of the camera
    float xoffset, yoffset;         // actual movement between the last two mouse position updates.
    bool cameraNeedsRecomputing = true;

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

    mat4 Camera::matrix() const
    {
        
        return lookAt(pos, lookat, upVector);
    }

    std::ostream &operator<<(std::ostream &os, Camera const &camera)
    {
        os << "pos:" << camera.pos << ' ';
        os << "lookAt:" << camera.at << ' ';
        os << "upVector:" << camera.up;
        return os;
    }

   
    /**
     * @brief Computes the minimum amount of camera rotation dependeing on the placement of the of teh mouse on the scree
     * If the current moevement is too smallcomapred to the camera position, the new minimum will be returned. Otherwise, the current
     * movement offest is returned.
     *
     * @param last last mouse cordinate (either x or y direction)
     * @param offset current movement
     * @return float
     */
    float computeMinMovement(float last, float offset)
    {
        float noMovement = (display_size / 4);
        float minMovementScaling = 0.05;

        if (last < (display_size / 2))
        {
            float minMovement = std::min((last - (display_size / 2) + noMovement), 0.0f) * minMovementScaling;
            if (minMovement < offset && offset >= 0)
            {
                return minMovement;
            }
        }
        else
        {
            float minMovement = std::max((last - (display_size / 2) - noMovement), 0.0f) * minMovementScaling;
            if (minMovement > offset && offset >= 0)
            {
                return minMovement;
            }
        }
        return offset;
    }

    float toRadians(float degrees)
    {
        return degrees * (M_1_PI / 180);
    }

    /**
     * @brief Updates camera position according to the keys "wasd" and the mouse movement
     * minimal mouse movement used to rotate the camera without moving it -> move the mouse to a side of a screen
     *
     */
    void Camera::updateCameraPosition()
    {
        if (cameraNeedsRecomputing)
        {
            float sensitivity = 0.7f;
            float xmovement = computeMinMovement(lastX, xoffset) * sensitivity;
            float ymovement = -computeMinMovement(lastY, yoffset) * sensitivity;

            yaw += xmovement;
            pitch += ymovement;

            /* Add if loopings should be forbidden
            if (pitch > 179.0f)
                pitch = 179.0f;
            if (pitch < -179.0f)
                pitch = -179.0f;*/

            vec3 direction;
            direction.x = cos(toRadians(yaw)) * cos(toRadians(pitch));
            direction.y = sin(toRadians(pitch));
            direction.z = sin(toRadians(yaw)) * cos(toRadians(pitch));
            lookat = normalize(direction);

            // check for collisions with the ground
            /* TODO add once height is computed
            float minHeigth = computeHeight(camera.x, camera.z) + 0.5;
              if (camera.y < minHeigth)
              {
                  camera.y = minHeigth;
              }*/
            lookat = pos + lookat;
            cameraNeedsRecomputing=false;
            // lookAt(pos, lookat, upVector);
            // camMatrix = lookAt(pos, lookat, upVector);
        }

        
    }

    void Camera::forwardPressedKeys(vec4 input)
    {
        const float cameraSpeed = 0.5f;
        if (input.a)
        {
            pos += cameraSpeed * lookat;
            cameraNeedsRecomputing = true;
        }
        if (input.b)
        {
            pos -= cameraSpeed * lookat;
            cameraNeedsRecomputing = true;
        }
        if (input.g)
        {
            pos -= normalize(cross(lookat, upVector)) * cameraSpeed;
            cameraNeedsRecomputing = true;
        }
        if (input.h)
        {
            pos += normalize(cross(lookat, upVector)) * cameraSpeed;
            cameraNeedsRecomputing = true;
        }
        
        if (cameraNeedsRecomputing)
        {
            updateCameraPosition();
        }
    }

    void setMousePosition(int x, int y)
    {
        if (firstMouse)
        {
            lastX = x;
            lastY = y;
            firstMouse = false;
        }

        xoffset = x - lastX;
        yoffset = y - lastY;
        lastX = x;
        lastY = y;

        if (xoffset == 0 && yoffset == 0)
        {
            cameraNeedsRecomputing = true;
        }
    }
}