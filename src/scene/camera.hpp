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
    /*
        class CameraEventSubscriber : public evt::Subscriber
        {
        public:
            constexpr CameraEventSubscriber(Camera *camera) : camera{camera} {}
            void updateCameraPosition();
            //bool dispatch(long delta_time); //, unsigned char keymap[26]) override;
            // bool dispatch (long delta_time, evt::Mouse const& prev, evt::Mouse const& curr) override;
        private:
            Camera *camera = nullptr;
        };
    */
    struct Camera
    {
        union
        {
            vec3 pos;
            vec3 position;
        };
        GLfloat &x{pos.x};
        GLfloat &y{pos.y};
        GLfloat &z{pos.z};
        /*union
        {
            vec3 lookat;
            vec3 at;
        };*/
        union
        {
            vec3 upVector;
            vec3 up;
        };
        vec3 viewingDirection;
        float yaw = 0;   // yaw of the camera
        float pitch = 0; // pitch of the camera

        //    const CameraEventSubscriber subscriber{this};

        // float &x {pos.x};
        // float &y {pos.y};
        // float &z {pos.z};
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
         * @brief If any of the inputs have changed (either the mouse moved or one button was clicked) it recomputes all 3 vectors used by the lookat function
         *
         */
        void updateCameraPosition();
        /**
         * @brief Set the Mouse position based on the given location
         *
         * @param x position of the mouse
         * @param y position of the mouse
         */
        void setMousePosition(int x, int y);
        /**
         * @brief Indicates, if one or more of the keys for forward, left, down and right are currently pressed. e.g.: (w,a,s,d)
         *
         * @param input (w,a,s,d)
         */
        void forwardPressedKeys(vec4 input);

        mat4 matrix() const;
    };
    std::ostream &operator<<(std::ostream &os, Camera const &camera);
}