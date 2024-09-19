#ifndef _NUMERIC_TOOLS_
#define _NUMERIC_TOOLS_
#include <stdint.h>
#include <stddef.h>
#include <memory>
#include <stdexcept>
namespace NumericTools{
    template<size_t ...Values>
    struct Count{
        constexpr static size_t value = sizeof...(Values);
    };
}
#endif