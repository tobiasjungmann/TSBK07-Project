#include "camera.hpp"

#include <cmath>

#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) < (b) ? (b) : (a))


namespace scn
{
    float yaw = 0;                  // yaw of the camera
    float pitch = 0;                // pitch of the camera

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
    }

    void Camera::updateCameraPosition()
    {
    }

    void Camera::forwardPressedKeys(vec4 input)
    {
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
    static float computeMinMovement(float last, float offset)
    {
        int display_size = 800;
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

    bool CameraEventSubscriber::dispatch(long delta_time, unsigned char keymap[26])
    {
        auto& pos = camera->pos;
        auto const& lookat = camera->lookat;
        auto const& upV = camera->upVector;
        const float cameraSpeed = 0.5f;
        if (keymap['w'])
            pos += cameraSpeed * lookat;
        if (keymap['a'])
            pos -= cameraSpeed * lookat;
        if (keymap['s'])
            pos -= normalize(cross(lookat, upV)) * cameraSpeed;
        if (keymap['d'])
            pos += normalize(cross(lookat, upV)) * cameraSpeed;

        return false;
    }

    bool CameraEventSubscriber::dispatch(long delta_time, evt::Mouse const& prev, evt::Mouse const& curr)
    {
        auto dx = curr.rel.x - prev.rel.x;
        auto dy = curr.rel.y - prev.rel.y;
        float sensitivity = 0.7f;
        float xmovement = computeMinMovement(prev.rel.x, dx) * sensitivity;
        float ymovement = -computeMinMovement(prev.rel.y, dy) * sensitivity;

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
        auto lookat = normalize(direction);

        // check for collisions with the ground
        /* TODO add once height is computed
        float minHeigth = computeHeight(camera.x, camera.z) + 0.5;
            if (camera.y < minHeigth)
            {
                camera.y = minHeigth;
            }*/
        camera->lookat = lookat + camera->pos;
        return false;
    }
}