#pragma once

#include <string>
#include <unordered_set>
#include "modelv2.hpp"
#include "../types_properties.hpp"
#include <vector>
#include <memory>

/*******
 * ****************
 * // FIXME create templated operators for property, to pass through to the ArgType
 * ****************
 */

namespace scn
{
  class Terrain;
  class Scene;
}

namespace obj
{
  // Forward Decl
  struct GameObj;
  struct MutableObj;
  class ModelledObject;
  class CollidingObject;

  template <typename>
  class Property;
  template <typename>
  class AnchoredProp;
  template <typename>
  class Anchor;

  namespace prop
  {
    struct Position;
    struct ModelProp;
  }

  struct GameObj
  {
  };

  class MutableObj : public GameObj
  {
  public:
    virtual void update(scn::Scene &) = 0;
    virtual void update(scn::Scene &, size_t) = 0;
  };

}

namespace obj
{
  namespace traits
  {
    template <class T, typename Threshold = double>
    using cpy_or_cref_t = std::conditional_t<(sizeof(T) < sizeof(Threshold)),
                                             std::decay_t<T>,
                                             std::add_const_t<std::add_lvalue_reference_t<T>>>;

    namespace _impl
    {
      template <class Obj>
      constexpr auto IsMutableT(int i) -> decltype(std::declval<Obj>().update, true) { return true; }
      template <class Obj>
      constexpr bool IsMutableT(double i) { return false; }
      template <class Obj>
      constexpr auto IsMutableV(Obj *obj, int i) -> decltype(obj->update, true) { return true; }
      template <class Obj>
      constexpr bool IsMutableV(Obj *obj, double i) { return false; }
    }

    template <class Obj>
    struct Mutable
    {
      static constexpr bool value{_impl::IsMutableT<Obj>(1)};
    };

    template <class Obj> 
    constexpr bool IsMutable(Obj *obj){ return _impl::IsMutableV<Obj>(obj, 1);}
    template <class Obj>
    constexpr bool Mutable_v{Mutable<Obj>::value};
  }

  template <class ArgType>
  class Property
  {
  public:
    using arg_t = ArgType;

    Property() = default;
    Property(ArgType arg) : m_prop{arg} {}
    Property(Property<ArgType> const &other) = default;
    Property<ArgType> &operator=(Property<ArgType> const &other) = default;

    // ---------- Cast
    inline operator ArgType() const
    {
      return m_prop;
    }

    // ---------- Accessors
    inline ArgType &operator()()
    {
      return m_prop;
    }

    inline ArgType const &operator()() const
    {
      return m_prop;
    }

    inline void operator()(ArgType arg)
    {
      m_prop = arg;
    }

    virtual inline void update(ArgType arg)
    {
      m_prop = arg;
    }

    // ------------- Members operators
    // ----------- Assigment
    template <class T>
    inline Property<ArgType> &operator=(T arg)
    {
      m_prop = static_cast<ArgType>(arg);
      return *this;
    }

    // Optimize lvalue arguments
    // If type is smaller than double, get a copy, otherwise get a reference
    template <class T, class = decltype(std::declval<ArgType>() + std::declval<T>())>
    Property<ArgType> &operator+=(traits::cpy_or_cref_t<T> rhs)
    {
      m_prop += rhs;
      return *this;
    }
    template <class T, class = decltype(std::declval<ArgType>() - std::declval<T>())>
    Property<ArgType> &operator-=(traits::cpy_or_cref_t<T> rhs)
    {
      m_prop -= rhs;
      return *this;
    }
    template <class T, class = decltype(std::declval<ArgType>() * std::declval<T>())>
    Property<ArgType> &operator*=(traits::cpy_or_cref_t<T> rhs)
    {
      m_prop *= rhs;
      return *this;
    }
    template <class T, class = decltype(std::declval<ArgType>() / std::declval<T>())>
    Property<ArgType> &operator/=(traits::cpy_or_cref_t<T> rhs)
    {
      m_prop /= rhs;
      return *this;
    }
    template <class T, class = decltype(std::declval<ArgType>() + std::declval<T>())>
    Property<ArgType> &operator+=(const T &&rhs)
    {
      m_prop += rhs;
      return *this;
    }
    template <class T, class = decltype(std::declval<ArgType>() - std::declval<T>())>
    Property<ArgType> &operator-=(const T &&rhs)
    {
      m_prop -= rhs;
      return *this;
    }
    template <class T, class = decltype(std::declval<ArgType>() * std::declval<T>())>
    Property<ArgType> &operator*=(const T &&rhs)
    {
      m_prop *= rhs;
      return *this;
    }
    template <class T, class = decltype(std::declval<ArgType>() / std::declval<T>())>
    Property<ArgType> &operator/=(const T &&rhs)
    {
      m_prop /= rhs;
      return *this;
    }

    // ------------- Operators

  protected:
    ArgType m_prop{};
  };

  template <class ArgType>
  class AnchoredProp : public Property<ArgType>
  {
  public:
    class Anchor
    {
      friend class AnchoredProp<ArgType>;

    public:
      Anchor() = default; // delete all copy and move operations
      ~Anchor()
      {
        for (auto prop : m_properties)
          prop->detach();
      }

      void updateProps(ArgType arg)
      {
        for (auto prop : m_properties)
          prop->update(arg);
      }

    protected:
      // TODO implement that for relative construction
      ArgType diff(ArgType const &arg)
      {
        // return arg - m_selfarg;
        return arg;
      }

    private:
      inline void attach(AnchoredProp<ArgType> &prop)
      {
        m_properties.insert(&prop);
      }

      inline void detach(AnchoredProp<ArgType> &prop)
      {
        m_properties.erase(&prop);
        prop.m_anchor = nullptr;
      }

    private:
      std::unordered_set<AnchoredProp<ArgType> *> m_properties;
    };

  public:
    using Property<ArgType>::Property;
    using Property<ArgType>::operator();
    AnchoredProp(AnchoredProp<ArgType> const &other);
    AnchoredProp<ArgType> &operator=(AnchoredProp<ArgType> const &other);

    inline virtual ~AnchoredProp()
    {
      detach();
    }

    inline bool hasAnchor() const noexcept
    {
      return m_anchor != nullptr;
    }

    inline void update(ArgType arg) override
    {
      Property<ArgType>::m_prop = hasAnchor() ? m_anchor->diff(arg) : arg;
    }

    virtual inline void attach(Anchor &anchor);
    virtual void detach();

  private:
    Anchor *m_anchor = nullptr;
  };

}

namespace obj
{
  namespace prop
  {
    struct Position : public AnchoredProp<vec3>
    {
      using AnchoredProp<vec3>::AnchoredProp;
      using AnchoredProp<vec3>::operator();
      bool inView = false;
    };
  }
}

namespace obj
{

  class ModelledObject : public GameObj
  {
  protected:
    void updateModelToWorldRotation()
    {
      updateModelToWorldRotation(m_direction, m_upVector);
    }

    /**
     * @brief Alligns the x axis of the model with the movement vector and teh up vector
     * @param newMovement
     * @param newUp
     */
    void updateModelToWorldRotation(vec3 newDirection, vec3 newUp);

  public:
    ModelledObject(Modelv2 model,
                   prop::Position pos,
                   vec3 dir,
                   vec3 up = vec3(0, 1, 0),
                   vec3 size = vec3(1, 1, 1)) : m_model{model},
                                                m_position{pos},
                                                m_direction{normalize(dir)},
                                                m_upVector{normalize(up)},
                                                m_size{size}

    {
    }
    virtual ~ModelledObject() = default;

    virtual inline mat3 orientationMtx() const noexcept
    {
      return IdentityMatrix();
    }

    inline Modelv2& model()
    {
      return m_model;
    }

  protected:
    Modelv2 m_model;
    prop::Position m_position;
    AnchoredProp<vec3> m_direction;
    vec3 m_upVector = vec3(0, 1, 0);
    // Size of the object in all three directions will be rotated accordingly and can be used as the hitbox object
    const vec3 m_size;
  };

  class CollidingObject : public ModelledObject
  {
  protected:
    void rotateHitbox(mat3 rotation);

  public:
    CollidingObject(Modelv2 mdl, vec3 pos, vec3 dir, vec3 up = vec3(0, 1, 0), vec3 size = vec3(1, 1, 1))
        : ModelledObject(mdl, pos, dir, up, size),
          m_longestDistanceFromCenter{Norm(size)} {};

    /**
     * @brief Change the movement of the model after it collided on the point given by the position with the given normal
     *
     * @param position
     * @param normalToCollisionPoint
     */
    virtual void collide(vec3 position, vec3 normalToCollisionPoint) = 0;

    /**
     * @brief Precomputes the next positions of the model based on the movement direction and the
     * speed at which it is moving. If the fish won't make it over one of the next obstacles
     * the direction vector is adapted so that it will go over the obstacle.
     * Only adapts movement in the y direction.
     *
     * @param terrain
     */
    virtual void adaptToTerrain(scn::Terrain &terrain) = 0;
    virtual void handleObjectCollision(CollidingObject *other) = 0;

  protected:
    /**
     * @brief Defined the size of the fish boundary (first line size in x, second size in y, third size in z)
     * // TODO update once the whole fish model is rotated
     */
    mat3 m_hitbox = IdentityMatrix(); // Deafult circle with length 1
    float m_longestDistanceFromCenter;
  };
}

#include "gameobj.tpp"
