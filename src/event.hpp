#pragma once

#include <memory>
#include <unordered_set> 

#include "VectorUtils4.h"
#include "MicroGlut.h"

namespace evt {

struct Mouse {
    vec2 abs;
    vec2 rel;
};

class Subscriber {
    public:
    /**
     * @brief Manage key input;
     * 
     * @return true if dispatch chaining should stop here.
     * @return false keep going with following subscribers
     */
    virtual bool dispatch(long delta_time, unsigned char keymap[26]) = 0;
    virtual bool dispatch(long delta_time, Mouse const& prev, Mouse const& curr) = 0;

    // delete copy functions for polymorphic class to avoid slicing derived classes.
    // here abstract class should prevent this anyway 
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
        curr.abs = vec2(x,y);
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