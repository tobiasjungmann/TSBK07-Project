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
     * @brief Returns z coordinate of the terrain at a given point on the generated terrain.
     * identifies corresponding nodes and then computes the vector on the plane to the given x and y coordinates.
     *
     * @param x - Coordinate
     * @param z - Coordiante
     * @return float
     */
    /* integrate anywhere in the project once terrain exists
    float computeHeight(float x, float z)
    {
        float x_difference = x - int(x);
        float z_difference = z - int(z);
        printf("x_difference: %f, ydifference: %f   sum: %f ", x_difference, z_difference, x_difference + z_difference);

        vec3 p_bottom_left = tm->vertexArray[(int(x) + (int(z) + 1) * terrain_width)];
        vec3 p_top_right = tm->vertexArray[((int(x) + 1) + int(z) * terrain_width)];

        
        //Third point of the triangle differs depending on which triangle is used in the square -> select teh corresponding starting point.
        //if it's the bottom right one of the square -> x and y distances must be switched
        
        vec3 startingPoint;
        if (x_difference + z_difference < 1.0)
        {
            startingPoint = tm->vertexArray[(int(x) + int(z) * terrain_width)]; // p_top_left;
        }
        else
        {
            startingPoint = tm->vertexArray[((int(x) + 1) + (int(z) + 1) * terrain_width)]; // p_bottom_right;
            int helper = x_difference;                                                      // swapp since axis have changed
            x_difference = z_difference;
            z_difference = helper;
        }

        vec3 v1 = (p_bottom_left - startingPoint) * x_difference; // movement on the x axis (top left to bottom left)
        vec3 v2 = (p_top_right - startingPoint) * z_difference;   // movement on the z axis (top left to top right)

        return (startingPoint + v1 + v2).y;
    }*/

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