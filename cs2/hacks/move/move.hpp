#pragma once

namespace hacks {
    class c_move {
    private:
        bool m_should_fake_jump{};
    public:
        void run( valve::user_cmd_t* const cmd );
    };

    inline const auto g_move = std::make_unique< c_move >( );
}