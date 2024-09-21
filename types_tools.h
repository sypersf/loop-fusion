#ifndef _TYPE_TOOLS_
#define _TYPE_TOOLS_
#include <stdint.h>
#include <stddef.h>
#include <memory>
#include <stdexcept>
namespace TypeTools
{
    struct NullType{};
    struct Sequential
    {
        
            template<typename T, size_t N, template <typename...> class TBase>
            struct CreateDuplicate
            {
                template<typename ...TTypes>
                using NextType = TBase<T, TTypes...>;
                using type = typename CreateDuplicate<T, N-1, NextType>::type;
            };

        
            template<typename T, template <typename...> class TBase>
            struct CreateDuplicate<T, 1, TBase>
            {
                using type = TBase<T>;
            };

        
            template<template <typename...> class TBase, typename ...TTypes>
            struct CreateSquential
            {
                using type = TBase<TTypes...>;
            };

        
            template<typename T> struct GetLen;
            template<template<typename ...> class TBase, typename ...TTypes>
            struct GetLen<TBase<TTypes...>>
            {
                constexpr static size_t value = sizeof...(TTypes);
            };

    
        
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
                using type = typename Sequential::CreateSquential<TBase, NT, TTypes...>::type;
            };
        
        
            template<typename T, size_t Slots, typename NT> struct Modify;

            template<template<typename ...> class TBase, size_t Slots, typename NT, typename ...TTypes>
            struct Modify<TBase<TTypes...>, Slots, NT>
            {
                using type = typename CreateModify<TBase, 0, Slots, sizeof...(TTypes) - 1, NT, TTypes...>::type;
            };

        
            template<typename T, typename TSearch>
            struct GetPosition;

        
            template<template<typename...> class TBase, typename TSearch, typename TCur, typename ...TTypes>
            struct GetPosition<TBase<TCur, TTypes...>, TSearch>{
                constexpr static size_t value = 1 + GetPosition<TBase<TTypes...>, TSearch>::value;  
            };

            template<template<typename...> class TBase, typename TSearch, typename ...TTypes>
            struct GetPosition<TBase<TSearch, TTypes...>, TSearch>{
                constexpr static size_t value = 0;  
            };


        
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


        
            template<typename T, size_t Slots>
            struct GetType
            {
                using type = typename GetType_<T, 0, Slots>::type;
            };
    
    };
}
#endif