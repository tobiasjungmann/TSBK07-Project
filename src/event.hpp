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
  struct Context;
  struct Mouse;
  struct MovementKeys;
  class Subscriber;
  // class Dispatcher;


  struct Mouse
  {
    vec2 currPos = vec2(450, 450); // FIXME here you should make use of your values in constants::
    vec2 prevPos = vec2(450, 450);

    void update(int x, int y)
    {
      if (prevPos.x != -INFINITY) // BUG Tobias please explain this part.
      {
        prevPos = currPos;
        currPos.x = x;
        currPos.y = y;
      }
      else
      {
        currPos.x = x;
        currPos.y = y;
        prevPos = currPos;
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
          curr.currPos = vec2(x,y);
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

  
  struct Context
  {
    evt::Mouse mouse;
    evt::MovementKeys keys;

    /**
     * @brief Set the Mouse position based on the given location
     *
     * @param x position of the mouse
     * @param y position of the mouse
     */
    void setMousePosition(int x, int y) noexcept;

    /**
     * @brief Indicates, if one or more of the keys for forward, left, down and right are currently pressed.
     *
     * @param forward - true if the key responsible for the movement is pressed.
     * @param left - true if the key responsible for the movement is pressed.
     * @param back - true if the key responsible for the movement is pressed.
     * @param right - true if the key responsible for the movement is pressed.
     */
    void setPressedKeys(bool forward, bool left, bool back, bool right) noexcept;
  };
}