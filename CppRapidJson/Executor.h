#pragma once
#include <string>

namespace CppRapidJson
{
    class Executor final
    {
    public:
        static void Execute() noexcept;
    private:
        static void PerformComputations() noexcept;
        static void ShowResult() noexcept;
    };
}