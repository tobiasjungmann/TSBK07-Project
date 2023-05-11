#pragma once

#include <utility>


namespace obj_traits {
  namespace details {
    template <class T, class = decltype(std::declval<T>().setPos())>
    bool constexpr placeable (int i)
    {
      return true;
    }

    template<class T>
    bool constexpr placeable (float i) {
      return false;
    }
  }

  template <class T>
  struct IsPlaceable {
    static constexpr bool value = details::placeable<T>(4);
  };

  template <class T>
  using Placeable = std::enable_if_t<IsPlaceable<T>::value>;
}
