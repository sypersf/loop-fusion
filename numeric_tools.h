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

    template<size_t ...Values>
    struct Multiplies;

    template<size_t Value, size_t ...Values>
    struct Multiplies<Value, Values...>
    {
        constexpr static size_t value = Value * Multiplies<Values...>::value;
    };

    template<size_t Value>
    struct Multiplies<Value>
    {
        constexpr static size_t value = Value;
    };

}
#endif