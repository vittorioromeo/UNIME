#pragma once

#include <exception>
#include <string>
#include <sstream>
#include <iostream>

#define DEPARENS(...) __VA_ARGS__

#define SA_SAME(value, type) \
    static_assert(std::is_same<DEPARENS value, DEPARENS type>{}, "")

#define SA_TYPE(value, type) \
    static_assert(std::is_same<decltype value, DEPARENS type>{}, "")

namespace test_impl
{
    template <typename T>
    inline void test_assert(
        int line, bool x, const T& res, const char* expr) noexcept
    {
        using namespace std::literals;

        if(x) return;
        std::ostringstream error;

        error << "line: " << line << "\n" << expr << "\n"
              << "result: " << res << "\n";

        std::cout << error.str() << std::endl;
        std::terminate();
    }

    template <typename T>
    inline void test_assert_expected(int line, bool x, const T& res,
        const char* expr, const char* expected)
    {
        using namespace std::literals;

        if(x) return;
        std::ostringstream error;

        error << "line: " << line << "\n" << expr << "\n"
              << "result: " << res << "\n"
              << "expected: " << expected << "\n";

        std::cout << error.str() << std::endl;
        std::terminate();
    }

    template <typename T>
    inline void test_assert_ns(
        int line, bool x, const T&, const char* expr) noexcept
    {
        using namespace std::literals;

        if(x) return;
        std::ostringstream error;

        error << "line: " << line << "\n" << expr << "\n";

        std::cout << error.str() << std::endl;
        std::terminate();
    }

    template <typename T>
    inline void test_assert_ns_expected(int line, bool x, const T&,
        const char* expr, const char* expected)
    {
        using namespace std::literals;

        if(x) return;
        std::ostringstream error;

        error << "line: " << line << "\n" << expr << "\n"
              << "expected: " << expected << "\n";

        std::cout << error.str() << std::endl;
        std::terminate();
    }
}

#define TEST_ASSERT(expr)                                     \
    do                                                        \
    {                                                         \
        auto _t_x = expr;                                     \
        test_impl::test_assert(__LINE__, _t_x, expr, #expr); \
    } while(false)

#define TEST_ASSERT_OP(lhs, op, rhs)                      \
    do                                                    \
    {                                                     \
        auto _t_xl = lhs;                                 \
        auto _t_x = _t_xl op rhs;                         \
        test_impl::test_assert_expected(                  \
            __LINE__, _t_x, _t_xl, #lhs #op #rhs, #rhs); \
    } while(false)

#define TEST_ASSERT_NS(expr)                                     \
    do                                                           \
    {                                                            \
        auto _t_x = expr;                                        \
        test_impl::test_assert_ns(__LINE__, _t_x, expr, #expr); \
    } while(false)

#define TEST_ASSERT_NS_OP(lhs, op, rhs)                   \
    do                                                    \
    {                                                     \
        auto _t_xl = lhs;                                 \
        auto _t_x = _t_xl op rhs;                         \
        test_impl::test_assert_ns_expected(               \
            __LINE__, _t_x, _t_xl, #lhs #op #rhs, #rhs); \
    } while(false)
