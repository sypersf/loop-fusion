#include "meta_container.h"
#include <vector>
#include <tuple>
#include <iostream>
int main()
{

    auto &&dict = MetaContainer::VarTypeDict<struct A, struct B, struct C>::Create().Set<A>(0.1f).Set<B>(false).Set<C>(888);

    std::cout << dict.template Get<A>() << std::endl;
    std::cout << dict.template Get<B>() << std::endl;
    std::cout << dict.template Get<C>() << std::endl;

    MetaContainer::Tensor<float, 2,3,6> tensor0(3.f);
    return 0;
}