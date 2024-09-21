#ifndef _OPS_
#define _OPS_
#include "types_tools.h"
namespace OPs
{
    template<template <typename ...> class OPName, typename Tx_, typename Ty_>
    struct BinaryOP;

    template<typename Tx_, typename Ty_>
    struct PlusOP;

    template<typename Tx_, typename Ty_>
    struct MinusOP;

    template<typename Tx_, typename Ty_>
    struct MultipliesOP;

    template<typename Tx_, typename Ty_>
    struct DividesOP;
}

namespace OPs
{
    template<template <typename ...> class OPName, typename Tx_, typename Ty_>
    struct BinaryOP {
        using ChildType = OPName<Tx_, Ty_>;
        const Tx_& left;
        const Ty_& right;
        BinaryOP(const Tx_& left, const Ty_& right) : left(left), right(right) {}

        template<typename Ty__>
        auto operator +(const Ty__& right)
        {
            using RetType = PlusOP<ChildType, Ty__>;
            return RetType(*reinterpret_cast<ChildType*>(this), right);
        }
        template<typename Ty__>
        auto operator -(const Ty__& right)
        {
            using RetType = MinusOP<ChildType, Ty__>;
            return RetType(*reinterpret_cast<ChildType*>(this), right);
        }
                template<typename Ty__>
        auto operator *(const Ty__& right)
        {
            using RetType = MultipliesOP<ChildType, Ty__>;
            return RetType(*reinterpret_cast<ChildType*>(this), right);
        }
                template<typename Ty__>
        auto operator /(const Ty__& right)
        {
            using RetType = DividesOP<ChildType, Ty__>;
            return RetType(*reinterpret_cast<ChildType*>(this), right);
        }

    };

    template<typename Tx_, typename Ty_>
    struct PlusOP : BinaryOP<PlusOP, Tx_, Ty_> {
        using FatherType = BinaryOP<PlusOP, Tx_, Ty_>;
        PlusOP(const Tx_& left, const Ty_& right) :FatherType(left, right) {}
    };

        template<typename Tx_, typename Ty_>
    struct MinusOP : BinaryOP<MinusOP, Tx_, Ty_> {
        using FatherType = BinaryOP<MinusOP, Tx_, Ty_>;
        MinusOP(const Tx_& left, const Ty_& right) :FatherType(left, right) {}
    };

        template<typename Tx_, typename Ty_>
    struct MultipliesOP : BinaryOP<MultipliesOP, Tx_, Ty_> {
        using FatherType = BinaryOP<MultipliesOP, Tx_, Ty_>;
        MultipliesOP(const Tx_& left, const Ty_& right) :FatherType(left, right) {}
    };

        template<typename Tx_, typename Ty_>
    struct DividesOP : BinaryOP<DividesOP, Tx_, Ty_> {
        using FatherType = BinaryOP<DividesOP, Tx_, Ty_>;
        DividesOP(const Tx_& left, const Ty_& right) :FatherType(left, right) {}
    };






}
#endif
