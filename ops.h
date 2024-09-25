#ifndef _OPS_
#define _OPS_
#include <stddef.h>
#include <memory>
namespace OPs
{
    template<typename Tx_, typename Ty_>
    struct PlusOP;

    template<typename Tx_, typename Ty_>
    struct MinusOP;

    template<typename Tx_, typename Ty_>
    struct MultipliesOP;

    template<typename Tx_, typename Ty_>
    struct DividesOP;


    template<template <typename ...> class OPName, typename Tx_, typename Ty_>
    struct BinaryOP {
        using ChildType = OPName<Tx_, Ty_>;
       	std::shared_ptr<const Tx_> left;
		std::shared_ptr<const Ty_> right;
        BinaryOP(const Tx_& left, const Ty_& right) : left(new Tx_(left)), right(new Ty_(right)) {}

        template<typename Ty__>
        constexpr auto operator +(const Ty__& right)
        {
            using RetType = PlusOP<ChildType, Ty__>;
            return RetType(*reinterpret_cast<ChildType*>(this), right);
        }
        template<typename Ty__>
        constexpr auto operator -(const Ty__& right)
        {
            using RetType = MinusOP<ChildType, Ty__>;
            return RetType(*reinterpret_cast<ChildType*>(this), right);
        }
        template<typename Ty__>
        constexpr auto operator *(const Ty__& right)
        {
            using RetType = MultipliesOP<ChildType, Ty__>;
            return RetType(*reinterpret_cast<ChildType*>(this), right);
        }
        template<typename Ty__>
        constexpr auto operator /(const Ty__& right)
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

    template<typename T>
	constexpr static auto Build(const T& ops)
	{
		return [&ops](size_t i) {return ops.data[i]; };
	}

	template<typename ...Args>
	constexpr static auto Build(const PlusOP<Args...>& ops)
	{
		return [&ops](size_t i) {return Build(*ops.left.get())(i) + Build(*ops.right.get())(i); };
	}

	template<typename ...Args>
	constexpr static auto Build(const MinusOP<Args...>& ops)
	{
		return [&ops](size_t i) {return Build(*ops.left.get())(i) - Build(*ops.right.get())(i); };
	}

	template<typename ...Args>
	constexpr static auto Build(const MultipliesOP<Args...>& ops)
	{
		return [&ops](size_t i) {return Build(*ops.left.get())(i) * Build(*ops.right.get())(i); };
	}

	template<typename ...Args>
	constexpr static auto Build(const DividesOP<Args...>& ops)
	{
		return [&ops](size_t i) {return Build(*ops.left.get())(i) / Build(*ops.right.get())(i); };
	}

}
#endif
