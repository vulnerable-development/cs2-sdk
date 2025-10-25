#ifndef CS2_SDK_CTX_HPP
#define CS2_SDK_CTX_HPP

class c_ctx {
private:
	using modules_t = std::unordered_map< sdk::hash_t, sdk::pe_image_t* >;

    using interfaces_t = std::unordered_map< sdk::hash_t, sdk::address_t >;

	bool wait_for_all_modules( modules_t& modules ) const;

    void init_imgui( const modules_t& modules ) const;

    void parse_interfaces( sdk::pe_image_t* const image, interfaces_t& interfaces ) const;

    void init_interfaces( const modules_t& modules ) const;

    void init_hooks( const modules_t& modules ) const;
public:
	void init( ) const;
};

inline const auto g_ctx = std::make_unique< c_ctx >( );

#endif