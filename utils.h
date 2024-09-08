#ifndef __UTILS__
#define __UTILS__

#include <stdint.h>
#include <stddef.h>
#include <memory>
namespace AudioMath
{
    struct NullType{};
    
    class Sequential
    {
        public:
            template<typename T, size_t N, template <typename...> class TBase>
            struct CreateDuplicate
            {
                template<typename ...TTypes>
                using NextType = TBase<T, TTypes...>;
                using type = typename CreateDuplicate<T, N-1, NextType>::type;
            };

        private:
            template<typename T, template <typename...> class TBase>
            struct CreateDuplicate<T, 1, TBase>
            {
                using type = TBase<T>;
            };

        public:
            template<template <typename...> class TBase, typename ...TTypes>
            struct CreateSquential
            {
                using type = TBase<TTypes...>;
            };

        public:
            template<typename T> struct GetLen;
            template<template<typename ...> class TBase, typename TType, typename ...TTypes>
            struct GetLen<TBase<TType, TTypes...>>
            {
                constexpr static size_t value = 1 + GetLen<TBase<TTypes...>>::value;
            };

        private:
            template<template<typename ...> class TBase, typename TType>
            struct GetLen<TBase<TType>>
            {
                constexpr static size_t value = 1;

            };

        private:
            template<template<typename ...> class TBase, size_t CurSlots, size_t Slots, size_t LastSlots, typename NT, typename ...TTypes>
            struct CreateModify;

            template<template<typename ...> class TBase, size_t CurSlots, size_t Slots, size_t LastSlots, typename NT, typename TCur, typename ...TTypes>
            struct CreateModify<TBase, CurSlots, Slots, LastSlots, NT, TCur, TTypes...>
            {
                template<typename ...RemainTypes>
                using NextTBase = TBase<TCur, RemainTypes...>;

                using type = typename CreateModify<NextTBase, CurSlots + 1, Slots, LastSlots, NT, TTypes...>::type;
            };

            template<template<typename ...> class TBase, size_t CurSlots, size_t LastSlots, typename NT, typename TCur, typename ...TTypes>
            struct CreateModify<TBase, CurSlots, CurSlots, LastSlots, NT, TCur, TTypes...>
            {
                template<typename ...RemainTypes>
                using NextTBase = TBase<NT, RemainTypes...>;

                using type = typename CreateModify<NextTBase, CurSlots + 1, CurSlots, LastSlots, NT, TTypes...>::type;
            };

            template<template<typename ...> class TBase, size_t Slots, size_t LastSlots, typename NT, typename TCur, typename ...TTypes>
            struct CreateModify<TBase, Slots + 1, Slots, LastSlots, NT, TCur, TTypes...>
            {
                using type = TBase<TCur, TTypes...>;
            };
        
        public:
            template<typename T, size_t Slots, typename NT> struct Modify;

            template<template<typename ...> class TBase, size_t Slots, typename NT, typename ...TTypes>
            struct Modify<TBase<TTypes...>, Slots, NT>
            {
                using type = typename CreateModify<TBase, 0, Slots, sizeof...(TTypes) - 1, NT, TTypes...>::type;
            };

        public:
            template<typename T, typename TSearch>
            struct GetPosition;

        private:
            template<template<typename...> class TBase, typename TSearch, typename TCur, typename ...TTypes>
            struct GetPosition<TBase<TCur, TTypes...>, TSearch>{
                constexpr static size_t value = 1 + GetPosition<TBase<TTypes...>, TSearch>::value;  
            };

            template<template<typename...> class TBase, typename TSearch, typename ...TTypes>
            struct GetPosition<TBase<TSearch, TTypes...>, TSearch>{
                constexpr static size_t value = 0;  
            };


        private:
            template<typename T, size_t CurSlots, size_t Slots>
            struct GetType_;
            template<template<typename...> class TBase,  size_t CurSlots, size_t Slots, typename TCur, typename ...TTypes>
            struct GetType_<TBase<TCur, TTypes...>, CurSlots, Slots>
            {
                using type = typename GetType_<TBase<TTypes...>, CurSlots + 1, Slots>::type;
            };


            template<typename T, size_t CurSlots, size_t Slots>
            struct GetType_;
            template<template<typename...> class TBase,  size_t CurSlots, typename TCur, typename ...TTypes>
            struct GetType_<TBase<TCur, TTypes...>, CurSlots, CurSlots>
            {
                using type = TCur;
            };


        public:
            template<typename T, size_t Slots>
            struct GetType
            {
                using type = typename GetType_<T, 0, Slots>::type;
            };
    
    };
    
    template<typename ...TTags>
    struct VarTypeDict
    {
        public:
            template<typename ...TParameters>
            struct Values
            {
                public:
                    Values() = default;
                    Values(std::array<std::shared_ptr<void> , sizeof...(TTags)>&& tuple)
                    {
                        for (size_t i = 0; i < sizeof...(TTags); ++i)
                        {
                            mTuple[i] = std::move(tuple[i]);
                        }
                    }

                    template<typename TType, typename TVal> 
                    auto Set(TVal&& val) -> typename Sequential::Modify<Values<TParameters...>, Sequential::GetPosition<VarTypeDict<TTags...>, TType>::value, TVal>::type
                    {
                        constexpr size_t pos = Sequential::GetPosition<VarTypeDict<TTags...>, TType>::value;
                        using new_type = typename Sequential::Modify<Values<TParameters...>, pos, TVal>::type;
                        using rawTVal = typename std::decay<TVal>::type;
                        rawTVal* tmp = new rawTVal(std::forward<TVal>(val));
                        mTuple[pos] = std::shared_ptr<void>(tmp,
                                                [](void* ptr){
                                                    rawTVal* nptr = static_cast<rawTVal*>(ptr);
                                                    delete nptr;
                                                });
                        return new_type(std::move(mTuple));
                    }

                    template<typename TTag>
                    auto Get() -> typename Sequential::GetType<Values<TParameters...>, Sequential::GetPosition<VarTypeDict<TTags...>, TTag>::value>::type&
                    {
                        constexpr size_t pos = Sequential::GetPosition<VarTypeDict<TTags...>, TTag>::value;
                        using AimType = typename Sequential::GetType<Values<TParameters...>, pos>::type;
                        void* tmp = mTuple[pos].get();
                        if (!tmp)
                            throw std::runtime_error("Empty Value.");

                        AimType* res = static_cast<AimType*>(tmp);
                        return *res;
                    }

                private:
                    std::array<std::shared_ptr<void> , sizeof...(TTags)> mTuple;
            };


            static auto Create() -> typename Sequential::CreateDuplicate<NullType, sizeof...(TTags), Values>::type
            {
                using type = typename Sequential::CreateDuplicate<NullType, sizeof...(TTags), Values>::type;
                return type();
            }
    };
}
#endif