#pragma once

#include "VectorUtils4.h"
#include <iostream>

#include "event.hpp"

namespace scn
{
    enum class Axis
    {
        X,
        Y,
        Z
    };

    struct Camera;

    struct Camera
    {
        union
        {
            vec3 pos;
            vec3 position;
        };
        vec3 viewingDirection;
        union
        {
            vec3 upVector;
            vec3 up;
        };
        float yaw = 0;   // yaw of the camera
        float pitch = 0; // pitch of the camera

        constexpr Camera(vec3 pos, vec3 viewingDirection, vec3 upVector) : position{pos}, viewingDirection{viewingDirection}, upVector{upVector} {}

        constexpr Camera(Camera const &other) : pos{other.pos}, viewingDirection{other.viewingDirection}, up{other.up} {}

        constexpr Camera &operator=(Camera const &other) noexcept
        {
            viewingDirection = other.viewingDirection;
            pos = other.pos;
            up = other.up;
            return *this;
        }

    public:
        void rotateAround(Axis axis, float angle, float radius) noexcept;
        void rotateRelAround(Axis axis, float relativeAng, float radius) noexcept;
        void translate(vec3 offset) noexcept;

/**
         * @brief Set the Mouse position based on the given location
         *
         * @param x position of the mouse
         * @param y position of the mouse
         */
        void setMousePosition(int x, int y)noexcept;

        /**
         * @brief Indicates, if one or more of the keys for forward, left, down and right are currently pressed.
         *
         * @param forward - true if the key responsible for the movement is pressed.
         * @param left - true if the key responsible for the movement is pressed.
         * @param back - true if the key responsible for the movement is pressed.
         * @param right - true if the key responsible for the movement is pressed.
         */
        void setPressedKeys(bool forward,bool left,bool back,bool right) noexcept;

        /**
         * @brief updates all vectors used by the lookat function according to the change in the mouse position and the currently pressed keys
         * setMousePosition and setPressedKeys must be called before calling this function.
         *
         */
        void updateCameraPosition();


        mat4 matrix() const;

    private:
        /**
         * @brief Computes the minimum amount of camera rotation dependeing on the placement of the of teh mouse on the scree
         * If the current moevement is too smallcomapred to the camera position, the new minimum will be returned. Otherwise, the current
         * movement offest is returned.
         *
         * @param last last mouse cordinate (either x or y direction)
         * @param current current mouse coordniate
         * @return float
         */
        float computeMinMovement(int last, int current);
    };
    std::ostream &operator<<(std::ostream &os, Camera const &camera);
}