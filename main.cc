#include "ops.h"
#include "meta_container.h"
#include <vector>
#include <tuple>
#include <iostream>
#include <typeinfo>
int main()
{

    auto&& dict = MetaContainer::VarTypeDict<struct A, struct B, struct C>::Create().Set<A>(0.1f).Set<B>(false).Set<C>(888);

    std::cout << dict.template Get<A>() << std::endl;
    std::cout << dict.template Get<B>() << std::endl;
    std::cout << dict.template Get<C>() << std::endl;


    MetaContainer::Tensor<float, 2, 3, 6> tensor0(3.f);
    MetaContainer::Tensor<float, 2, 3, 6> tensor1(4.f);
    MetaContainer::Tensor<float, 2, 3, 6> tensor2(5.f);
    auto ops = tensor0 / tensor1 + tensor1 * tensor2;
    auto fn = OPs::Build(ops);
    MetaContainer::Tensor<float, 2, 3, 6> result(0.f);
    for(size_t i = 0; i < result.elem_num; ++i)
    {
        result.data[i] = fn(i);
    }
    return 0;
}