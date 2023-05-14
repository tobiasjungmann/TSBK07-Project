#pragma once

#include "VectorUtils4.h"
#include <iostream>
#include <vector>

#include "event.hpp"
#include "gameobj/gameobj.hpp"

namespace obj {
  class GameObj;
}

namespace evt {
  struct Context;
  class Terrain;
}

namespace scn
{
  enum class Axis
  {
    X,
    Y,
    Z
  };

  class Camera;


  class Camera
  {
  public:
    Camera(vec3 pos, vec3 viewingDirection, vec3 upVector) : position{pos}, viewingDirection{viewingDirection}, upVector{upVector} {}

    Camera(Camera const &other) : pos{other.pos}, viewingDirection{other.viewingDirection}, up{other.up} {}

    Camera &operator=(Camera const &other) noexcept
    {
      viewingDirection = other.viewingDirection;
      pos = other.pos;
      up = other.up;
      return *this;
    }

    /**
     * @brief updates all vectors used by the lookat function according to the change in the mouse position and the currently pressed keys
     * setMousePosition and setPressedKeys must be called before calling this function.
     *
     */
    void updateCameraPosition(evt::Context const& ctxt, Terrain const& terrain);

    // Requires that obj has a setPos() method
    // And that obj is a GameObj
    inline void attachPosition(obj::prop::Position &pos) {
      pos.attach(m_attachedPos);
    }
    inline void attachDirection(obj::AnchoredProp<vec3> &dir) {
      dir.attach(m_attachedDir);
    }

    mat4 matrix() const;

  public:
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

  private:
    obj::AnchoredProp<vec3>::Anchor m_attachedPos;
    obj::AnchoredProp<vec3>::Anchor m_attachedDir;
  };

  std::ostream &operator<<(std::ostream &os, Camera const &camera);
}