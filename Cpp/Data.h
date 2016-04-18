#pragma once
#include <string>

namespace Cpp
{
    class Data final
    {
    public:
        static const std::wstring& GetSmallJson() noexcept;

        static const std::wstring& GetBigJson() noexcept;            
    };    
}
