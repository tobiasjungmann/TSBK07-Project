#ifndef DEBUGGING_H
#define DEBUGGING_H

#include <functional>

#ifdef __cplusplus
#include "types_properties.hpp"

namespace dbg {
enum class Level : int {
    INFO = 0,
    VERBOSE = 1
};

template <Level level>
constexpr void if_dlevel(std::function<void(void)>&& action_if_sufficient_dlevel)
{
    #ifdef DLEVEL
        if constexpr (DLEVEL >= scn::to_underlying(level)) action_if_sufficient_dlevel();
    #else
    #endif // ENDIF
}
}
#else // __cplusplus not defined
    enum Level {
        DLVL_INFO = 0,
        DLVL_VERBOSE = 1
    };
    #ifdef DLEVEL
    #define if_dlevel(lvl, func) do {if (DLEVEL >= (lvl)) {func}} while(0)
    #else // __cplusplus
    #define if_dlevel(lvl, func) do {} while(0)
    #endif // __cplusplus
#endif // __cplusplus
#endif // DEBUGGING_H