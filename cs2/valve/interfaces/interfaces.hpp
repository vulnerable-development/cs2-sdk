#ifndef CS2_SDK_INTERFACES_HPP
#define CS2_SDK_INTERFACES_HPP

namespace valve {
    struct input_system_t {} inline* g_input_system{};

    struct input_t {
        std::byte       pad0[ 593u ]{};
        bool            m_camera_in_third_person{};
        std::byte       pad1[ 6u ]{};
        sdk::qang_t     m_third_person_angle{};
    } inline* g_input{};
}

#endif