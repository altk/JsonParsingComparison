#pragma once
#include <string>

namespace AndroidCpp
{
    class Data final
    {
    public:
        static const std::wstring& GetSmallJson() noexcept;

        static const std::wstring& GetBigJson() noexcept;            
    };    
}
