#ifndef CS2_SDK_MOVE_HPP
#define CS2_SDK_MOVE_HPP

namespace hacks {
    class c_move {
    public:
        void run( valve::user_cmd_t* const cmd ) const;
    };

    inline const auto g_move = std::make_unique< c_move >( );
}

#endif