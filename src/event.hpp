#pragma once

#include <memory>
#include <unordered_set>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <limits>
#include "VectorUtils4.h"
#include "MicroGlut.h"

namespace evt
{

    struct Mouse
    {
        vec2 currentPosition = vec2(450, 450);
        vec2 previousPosition = vec2(450, 450);

        void update(int x, int y)
        {
            if (previousPosition.x != -INFINITY)
            {
                previousPosition = currentPosition;
                currentPosition.x = x;
                currentPosition.y = y;
            }
            else
            {
                currentPosition.x = x;
                currentPosition.y = y;
                previousPosition = currentPosition;
            }
        }

        /*std::ostream &operator<<(std::ostream &os, Mouse const &mouse)
        {
            os << "x:" << mouse << ' ';
            return os;
        }*/
    };

    struct MovementKeys
    {
        union
        {
            bool w;
            bool forward;
        };
        union
        {
            bool a;
            bool left;
        };
        union
        {
            bool s;
            bool back;
        };
        union
        {
            bool d;
            bool right;
        };
    };

    class Subscriber
    {
    public:
        /**
         * @brief Manage key input;
         *
         * @return true if dispatch chaining should stop here.
         * @return false keep going with following subscribers
         */
        //    virtual bool dispatch(long delta_time, unsigned char keymap[26]) = 0;
        //   virtual bool dispatch(long delta_time, Mouse const &prev, Mouse const &curr) = 0;
        // virtual void updateCameraPosition()=0;
        //  delete copy functions for polymorphic class to avoid slicing derived classes.
        //  here abstract class should prevent this anyway
    };

    /* class Dispatcher
    {
    public:
        Dispatcher(Window *win) : win{win} {};
        inline void operator()() {
            unsigned char keymap[26];
            glutAlphabetState(&keymap);
            for (auto& sub: subscribers) {
                sub->dispatch(-1, keymap);
            }
        }
        inline void operator()(int x, int y) {
            Mouse curr;
            curr.currentPosition = vec2(x,y);
            curr.rel = 2*vec2(x/win->width(), y/win->height()) - vec2(1, 1);
            for (auto& sub: subscribers) {
                sub->dispatch(-1, prev, curr);
            }
            prev = curr;
        }

        inline void addSubscriber(std::unique_ptr<Subscriber> sub) {
            subscribers.emplace(sub);
        }
        void removeSubscriber(Subscriber *sub);
    private:
        Mouse prev = {vec2(0,0), vec2(0,0)};
        std::unordered_set<std::unique_ptr<Subscriber>> subscribers;
        Window *win;
    }; */
}