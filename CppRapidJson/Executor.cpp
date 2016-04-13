#include "pch.h"
#include "Executor.h"

using namespace CppRapidJson;

void Executor::Execute() noexcept
{
    if (IsDebuggerPresent())
    {
        ShowResult();
    }
    else
    {
        PerformComputations();
    }
}

void Executor::PerformComputations() noexcept
{
}

void Executor::ShowResult() noexcept
{
}
