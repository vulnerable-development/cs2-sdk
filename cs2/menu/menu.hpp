#ifndef CS2_SDK_MENU_HPP
#define CS2_SDK_MENU_HPP

class c_menu {
private:
    struct {
        bool m_hidden{}, m_active{};
    } m_main{};
public:
    void run( ) const;

    ALWAYS_INLINE auto& main( );
};

inline const auto g_menu = std::make_unique< c_menu >( );

#include "impl/menu.inl"

#endif