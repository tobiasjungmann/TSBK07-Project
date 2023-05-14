#include <cmath>
#include "camera.hpp"
#include "../constants.hpp"
#include "../event.hpp"
#include "scene/terrain.hpp"
#include "gameobj/gameobj.hpp"

#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) < (b) ? (b) : (a))

namespace scn
{
  namespace helpers
  {
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
      float noMovement = (constants::display_size >> 2);
      float minMovementScaling = 0.05;

      if (last < (constants::display_size >> 1))
      {
        return min((last - (constants::display_size >> 1) + noMovement), 0.0f) * minMovementScaling;
      }
      else
      {
        return max((last - (constants::display_size >> 1) - noMovement), 0.0f) * minMovementScaling;
      }
    }

    float toRadians(float degrees)
    {
      return degrees * (M_PI / 180);
    }
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

  void Camera::updateCameraPosition(evt::Context const &ctxt, Terrain const &terrain)
  {
    const float cameraSpeed = 0.5f;
    float sensitivity = 0.7f;
    if (ctxt.keys.w)
    {
      pos += cameraSpeed * viewingDirection;
    }
    if (ctxt.keys.s)
    {
      pos -= cameraSpeed * viewingDirection;
    }
    if (ctxt.keys.a)
    {
      pos -= normalize(cross(viewingDirection, up)) * cameraSpeed;
    }
    if (ctxt.keys.d)
    {
      pos += normalize(cross(viewingDirection, up)) * cameraSpeed;
    }
    pos.x = terrain.nextInsideFieldWidth(pos.x, 10);
    pos.z = terrain.nextInsideFieldWidth(pos.z, 10);

    const float maxAltitude = 50.f;
    if (pos.y > maxAltitude)
      pos.y = maxAltitude;

    // check for collisions with the ground
    float minHeigth = terrain.computeHeight(pos.x, pos.z) + 2.0; // + 0.5;        // TODO take a real hitbox size
    if (pos.y < minHeigth)
    {
      pos.y = minHeigth;
    }

    float xmovement = helpers::computeMinMovement(ctxt.mouse.prevPos.x, ctxt.mouse.currPos.x) * sensitivity;
    float ymovement = -helpers::computeMinMovement(ctxt.mouse.currPos.y, ctxt.mouse.prevPos.y) * sensitivity;

    yaw += xmovement;
    pitch += ymovement;

    /* Add if loopings should be forbidden
    if (pitch > 179.0f)
        pitch = 179.0f;
    if (pitch < -179.0f)
        pitch = -179.0f;*/

    using helpers::toRadians;
    // TODO only compute when mouse was updated between the last two dispalys
    vec3 direction;
    direction.x = cos(0.1 * toRadians(yaw)) * cos(0.1 * toRadians(pitch));
    direction.y = sin(0.1 * toRadians(pitch));
    direction.z = sin(0.1 * toRadians(yaw)) * cos(0.1 * toRadians(pitch));
    viewingDirection = normalize(direction);

    // FIXME probably updateProps requirements of attached to have a relative position setter, instead of setPos
    m_attachedDir.updateProps(viewingDirection);
    m_attachedPos.updateProps(0);

    // check for collisions with the ground
    /* TODO probably move into the collision detection
    float minHeigth = computeHeight(camera.x, camera.z) + 0.5;
        if (camera.y < minHeigth)
        {
            camera.y = minHeigth;
        }*/
    // camera->lookat = lookat + camera->pos;
  }
}