namespace obj {

  template <class> class Property;

  template <class ArgType>
  Property<ArgType>::Property(Property<ArgType> const &other) : m_prop{other.m_prop}
  {
    if (other.hasAnchor())
      attach(*other.m_anchor);
  }

  template <class ArgType>
  Property<ArgType> &Property<ArgType>::operator=(Property<ArgType> const &other)
  {
    if (&other != this)
    {
      m_prop = other.m_prop;
      if (other.hasAnchor())
        attach(*other.m_anchor);
    }
    return *this;
  }

  template <class ArgType>
  void Property<ArgType>::attach(Anchor<ArgType> &anchor)
  {
    if (hasAnchor())
    {
      detach();
    }
    m_anchor = &anchor;
    m_anchor->attach(*this);
  }

  template <class ArgType>
  void Property<ArgType>::detach()
  {
    if (not hasAnchor())
      return;
    m_anchor->detach(*this);
  }
}