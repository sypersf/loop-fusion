#include "meta_container.h"
#include <vector>
#include <tuple>
#include <iostream>
#include <typeinfo>
int main()
{

    // auto&& dict = MetaContainer::VarTypeDict<struct A, struct B, struct C>::Create().Set<A>(0.1f).Set<B>(false).Set<C>(888);

    // std::cout << dict.template Get<A>() << std::endl;
    // std::cout << dict.template Get<B>() << std::endl;
    // std::cout << dict.template Get<C>() << std::endl;


    MetaContainer::Tensor<float, 2, 2, 2> tensor0(
        {3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f}
        );
    MetaContainer::Tensor<float, 2, 2, 2> tensor1(
        {4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f}
        );
    MetaContainer::Tensor<float, 2, 2, 2> tensor2(
        {5.f, 6.f, 7.f, 8.f, 9.f, 10.f, 11.f, 12.f}
        );
    auto ops = tensor0 / tensor1 + tensor1 * tensor2;
    decltype(tensor0) result;
    result.Evaluate(ops);
    for (size_t i = 0; i < result.elem_num; ++i)
    {
        std::cout << result.data[i] << std::endl;
    }
    return 0;
}