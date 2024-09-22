# Problem
Build a Tensor, overload operators to complete vector calculations, but the compiler cannot implement loop fusion.
For an operation result = tensor0 + tensor1 * tensor2
The generated code is
```
for(int i = 0; i < L; ++i)
{
    tmp[i] = tensor1[i] * tensor2[i];
}
for(int i = 0; i < L; ++i)
{
    result[i] = tensor0[i] + tmp[i];
}
```
However we expect to generate
```
for(int i = 0; i < L; ++i)
{
    result[i] = tensor0[i] + tensor1[i] * tensor2[i];
}
```
# Solution
We record a series of operations to construct a calculation graph (anonymous function) by overloading operators.
```
auto ops = tensor0 + tensor1 * tensor2;
```
Then, Use the constructed computational graph to construct the final result. The details of the construction are as follows:
```
Tensor(const std::function<T(size_t)>& fn)
{
    for (size_t i = 0; i < elem_num; ++i)
    {
        data[i] = fn(i);
    }
}
```