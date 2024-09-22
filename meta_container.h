#ifndef _META_CONTAINTER_
#define _META_CONTAINTER_

#include "types_tools.h"
#include "numeric_tools.h"
#include <array>
#include <functional>
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


namespace MetaContainer
{

    template<typename ...TTags>
    struct VarTypeDict
    {

        template<typename ...TParameters>
        struct Values
        {

            Values() = default;
            Values(std::array<std::shared_ptr<void>, sizeof...(TTags)>&& tuple)
            {
                for (size_t i = 0; i < sizeof...(TTags); ++i)
                {
                    mTuple[i] = std::move(tuple[i]);
                }
            }

            template<typename TType, typename TVal>
            auto Set(TVal&& val)
            {
                constexpr size_t pos = TypeTools::Sequential::GetPosition<VarTypeDict<TTags...>, TType>::value;
                using new_type = typename TypeTools::Sequential::Modify<Values<TParameters...>, pos, TVal>::type;
                using rawTVal = typename std::decay<TVal>::type;
                rawTVal* tmp = new rawTVal(std::forward<TVal>(val));
                mTuple[pos] = std::shared_ptr<void>(tmp,
                    [](void* ptr) {
                        rawTVal* nptr = static_cast<rawTVal*>(ptr);
                        delete nptr;
                    });
                return new_type(std::move(mTuple));
            }

            template<typename TTag>
            auto& Get()
            {
                constexpr size_t pos = TypeTools::Sequential::GetPosition<VarTypeDict<TTags...>, TTag>::value;
                using AimType = typename TypeTools::Sequential::GetType<Values<TParameters...>, pos>::type;
                void* tmp = mTuple[pos].get();
                if (!tmp)
                    throw std::runtime_error("Empty Value.");

                AimType* res = static_cast<AimType*>(tmp);
                return *res;
            }


            std::array<std::shared_ptr<void>, sizeof...(TTags)> mTuple;
        };


        static auto Create()
        {
            using type = typename TypeTools::Sequential::CreateDuplicate<TypeTools::NullType, sizeof...(TTags), Values>::type;
            return type();
        }
    };

    template <typename T, size_t ...NDims>
    struct Tensor
    {
        using TensorType = Tensor<T, NDims...>;
        constexpr static size_t elem_num = NumericTools::Multiplies<NDims...>::value;
        using DataType = std::array<T, elem_num>;
        using ElemType = T;

        std::array<T, elem_num> data;
        Tensor(const T& v)
        {
            for (size_t i = 0; i < elem_num; ++i)
            {
                data[i] = v;
            }
        }

        Tensor(const T*& v)
        {
            for (size_t i = 0; i < elem_num; ++i)
            {
                data[i] = v[i];
            }
        }

        Tensor(const std::function<T(size_t)>& fn)
        {
            for (size_t i = 0; i < elem_num; ++i)
            {
                data[i] = fn(i);
            }
        }

        Tensor(const std::initializer_list<T>& l)
        {
            for (size_t i = 0; i < elem_num; ++i)
            {
                data[i] = *(l.begin() + i);
            }
        }


        template<typename Ty_>
        constexpr auto operator + (const Ty_& rhs)
        {
            return OPs::PlusOP<TensorType, Ty_>(*this, rhs);
        }
        template<typename Ty_>
        constexpr auto operator - (const Ty_& rhs)
        {
            return  OPs::MinusOP<TensorType, Ty_>(*this, rhs);
        }
        template<typename Ty_>
        constexpr auto operator * (const Ty_& rhs)
        {
            return  OPs::MultipliesOP<TensorType, Ty_>(*this, rhs);
        }
        template<typename Ty_>
        constexpr auto operator / (const Ty_& rhs)
        {
            return OPs::DividesOP<TensorType, Ty_>(*this, rhs);
        }
    };
}
#endif