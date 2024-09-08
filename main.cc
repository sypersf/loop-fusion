#include "utils.h"
#include <vector>
#include <tuple>
#include <iostream>
int main()
{
    using type = AudioMath::Sequential::CreateDuplicate<float, 6, std::tuple>::type;
    static_assert(std::is_same<std::tuple<float,float,float,float,float,float>, type>::value, "no pass");

    constexpr size_t param_num = AudioMath::Sequential::GetLen<type>::value;
    static_assert(param_num==6, "no pass");

    using new_type = AudioMath::Sequential::Modify<type, 3, int>::type; 
    static_assert(std::is_same<std::tuple<float,float,float,int,float,float>, new_type>::value, "no pass");
    
    constexpr size_t pos = AudioMath::Sequential::GetPosition<new_type, int>::value;
    static_assert(pos == 3, "no pass");

    using pos_type = AudioMath::Sequential::GetType<new_type, 3>::type;
    static_assert(std::is_same<pos_type, int>::value, "no pass");

    auto &&dict = AudioMath::VarTypeDict<struct A, struct B, struct C>::Create();
    static_assert(std::is_same<decltype(dict), AudioMath::VarTypeDict<A, B, C>::Values<AudioMath::NullType, AudioMath::NullType, AudioMath::NullType>&&>::value, "no pass");

    auto &&dict1 = dict.template Set<B>(1.6f);
    static_assert(std::is_same<decltype(dict1), AudioMath::VarTypeDict<A, B, C>::Values<AudioMath::NullType, float, AudioMath::NullType>&&>::value, "no pass");
    std::cout << dict1. template Get<B>() << std::endl; //1.6f

    auto &&dict2 = dict1.template Set<A>(888);
    static_assert(std::is_same<decltype(dict2), AudioMath::VarTypeDict<A, B, C>::Values<int, float, AudioMath::NullType>&&>::value, "no pass");
    std::cout << dict2. template Get<A>() << std::endl; //888



    return 0;
}