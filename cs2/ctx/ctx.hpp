#ifndef CS2_SDK_CTX_HPP
#define CS2_SDK_CTX_HPP

class c_ctx {
private:
	using modules_t = std::unordered_map< sdk::hash_t, sdk::pe_image_t* >;

	bool wait_for_all_modules( modules_t& modules ) const;
public:
	void init( ) const;
};

inline const auto g_ctx = std::make_unique< c_ctx >( );

#endif