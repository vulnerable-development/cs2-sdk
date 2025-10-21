#ifndef CS2_SDK_CTX_HPP
#define CS2_SDK_CTX_HPP

class c_ctx {
public:
	void init( ) const;
};

inline const auto g_ctx = std::make_unique< c_ctx >( );

#endif