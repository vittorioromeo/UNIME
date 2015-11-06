#include <iostream>

template <typename T0, typename T1>
__attribute__((always_inline)) 
auto mod(T0 a, T1 b)
{
    auto division_result(a / b);
    auto multiplication_result(division_result * b);
    return a - multiplication_result;
}

template <typename T0, typename T1>
__attribute__((always_inline)) 
auto is_multiple_of(T0 a, T1 b)
{
    return mod(a, b) == 0;
}

template <typename T0, typename T1>
__attribute__((always_inline)) 
auto is_multiple_of_raw(T0 a, T1 b)
{
    return (a / b) * b == a;
}

void t0()
{
    int a, b;
    scanf("%d %d", &a, &b);
    volatile auto x = is_multiple_of(a, b);
}

void t1()
{
    int a, b;
    scanf("%d %d", &a, &b);
    volatile auto x = is_multiple_of_raw(a, b);
}

int main() {}
