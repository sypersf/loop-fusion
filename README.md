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

Then, we use the computational graph to construct the result:
```c++
decltype(tensor0) result;
result.Evaluate(ops);
```

This approach allows us to express the desired calculations in a more concise and efficient way, while also avoiding the compiler's inability to perform loop fusion to optimize the code.

### Result
We see through disassembly that there is only one loop body, and the compiler has completed instruction parallelism.
```c++
do
{
*(__m128 *)&v12[v3 / 4] = _mm_add_ps(
                            _mm_mul_ps(*(__m128 *)((char *)&unk_2044 + v3), *(__m128 *)((char *)&unk_2024 + v3)),
                            _mm_div_ps(*(__m128 *)((char *)&unk_2004 + v3), *(__m128 *)((char *)&unk_2024 + v3)));
v3 += 16LL;
}
while ( v3 != 32 );
```