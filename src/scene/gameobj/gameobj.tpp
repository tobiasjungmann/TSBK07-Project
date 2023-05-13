#include <ostream>

namespace obj
{

  template <class>
  class Property;
  template <class>
  class AnchoredProp;
  using namespace traits;

  // -------------- NON-MEMBER OPERATORS
  template <class ArgType, class T>
  auto operator+(Property<ArgType> prop, cpy_or_cref_t<T> rhs) -> decltype(prop() + rhs)
  {
    return prop() += rhs;
  }

  template <class ArgType, class T>
  auto operator-(Property<ArgType> prop, cpy_or_cref_t<T> rhs) -> decltype(prop() - rhs)
  {
    return prop() -= rhs;
  }

  template <class ArgType, class T>
  auto operator*(Property<ArgType> prop, cpy_or_cref_t<T> rhs) -> decltype(prop() * rhs)
  {
    return prop() -= rhs;
  }

  template <class ArgType, class T>
  auto operator/(Property<ArgType> prop, cpy_or_cref_t<T> rhs) -> decltype(prop() / rhs)
  {
    return prop() /= rhs;
  }

  template <class ArgType>
  std::ostream &operator<<(std::ostream &os, Property<ArgType> const &prop)
  {
    return os << prop();
  }

  // ---------- AnchoredProp

  template <class ArgType>
  AnchoredProp<ArgType>::AnchoredProp(AnchoredProp<ArgType> const &other)
  {
    Property<ArgType>::m_prop = other.m_prop;
    if (other.hasAnchor())
      attach(*other.m_anchor);
  }

  template <class ArgType>
  AnchoredProp<ArgType> &AnchoredProp<ArgType>::operator=(AnchoredProp<ArgType> const &other)
  {
    if (&other != this)
    {
      Property<ArgType>::m_prop = other.m_prop;
      if (other.hasAnchor())
        attach(*other.m_anchor);
    }
    return *this;
  }

  template <class ArgType>
  void AnchoredProp<ArgType>::attach(Anchor &anchor)
  {
    if (hasAnchor())
    {
      detach();
    }
    m_anchor = &anchor;
    m_anchor->attach(*this);
  }

  template <class ArgType>
  void AnchoredProp<ArgType>::detach()
  {
    if (not hasAnchor())
      return;
    m_anchor->detach(*this);
  }
}