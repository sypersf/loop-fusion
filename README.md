## Problem: Loop Fusion Failure

When performing vector calculations using Tensors by operator overloading, the compiler fails to perform loop fusion. For example, for an operation like `result = tensor0 + tensor1 * tensor2`, the generated code for disassembly looks like this:

```c++
for(int i = 0; i < L; ++i)
{
    tmp[i] = tensor1[i] * tensor2[i];
}
for(int i = 0; i < L; ++i)
{
    result[i] = tensor0[i] + tmp[i];
}
```

However, we expect the generated code to be:

```c++
for(int i = 0; i < L; ++i)
{
    result[i] = tensor0[i] + tensor1[i] * tensor2[i];
}
```

## Solution: Computational Graph Construction

We can solve this problem by recording a series of operations to construct a computational graph using operator overloading.

```c++
auto ops = tensor0 + tensor1 * tensor2;
```

Then, we use the constructed computational graph to build the anonymous function. Finally, the result is given:
```c++
decltype(tensor0) result(OPs::Build(ops));
```

The details of the construction are as follows:
```c++
Tensor(const std::function<T(size_t)>& fn)
{
    for (size_t i = 0; i < elem_num; ++i)
    {
        data[i] = fn(i);
    }
}
```

This approach allows us to express the desired calculations in a more concise and efficient way, while also avoiding the compiler's inability to perform loop fusion to optimize the code.
