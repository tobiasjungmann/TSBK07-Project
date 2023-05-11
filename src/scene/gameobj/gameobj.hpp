#pragma once

#include <string>
#include <unordered_set>

namespace obj
{
  struct GameObj
  {
  };
}

namespace obj
{
  template <class ArgType>
  class Property;

  template <typename Arg>
  class Anchor
  {
  public:
    // BUG Requires Arg to be default constructible
    Anchor() = default; // delete all copy and move operations
    virtual ~Anchor() {
      for (auto prop: m_properties)
        prop->detach();
    }

    void updateProps(Arg arg)
    {
      for (auto prop : m_properties)
        prop->update(arg);
    }

  protected:
    // BUG requires Arg::operator- defined
    virtual Arg diff(Arg const &arg)
    {
      // return arg - m_selfarg;
      return arg;
    }

  private:
    friend class Property<Arg>;
    inline void attach(Property<Arg> &prop)
    {
      m_properties.insert(&prop);
    }

    inline void detach(Property<Arg> &prop)
    {
      m_properties.erase(&prop);
      prop.m_anchor = nullptr;
    }

  private:
    std::unordered_set<Property<Arg> *> m_properties;
  };
}

namespace obj
{
  template <class ArgType>
  class Property
  {
  public:
    using arg_t = ArgType;

    Property(ArgType arg = ArgType()) : m_prop{arg} {}
    Property(Property<ArgType> const &other);
    Property<ArgType> &operator=(Property<ArgType> const &other);
    inline virtual ~Property()
    {
      detach();
    }

    inline Property<ArgType> &operator=(ArgType arg)
    {
      m_prop = arg;
      return *this;
    }

    inline bool hasAnchor() const noexcept
    {
      return m_anchor != nullptr;
    }

    inline operator ArgType() const
    {
      return m_prop;
    }

    inline ArgType &operator()()
    {
      return m_prop;
    }

    inline void operator()(ArgType arg)
    {
      m_prop = arg;
    }

    inline ArgType const &operator()() const
    {
      return m_prop;
    }

    // TODO would be nice to have automation here. Maybe with references in anchor and some diff operation ?
    virtual inline void update(ArgType arg)
    {
      m_prop = hasAnchor() ? m_anchor->diff(arg) : arg;
    }

    inline ArgType const& get() const
    {
      return m_prop;
    }

    virtual inline void attach(Anchor<ArgType> &anchor);
    virtual void detach();

  protected:
    ArgType m_prop;

  private:
    friend class Anchor<ArgType>;
    Anchor<ArgType> *m_anchor = nullptr;
  };
}

namespace obj
{
  namespace prop
  {
    struct Position : public Property<vec3>
    {
      inline Position &operator=(vec3 pos)
      {
        Property<vec3>::operator=(pos);
        return *this;
      }

      bool inView = false;
    };
  }
}


#include "gameobj.tpp"