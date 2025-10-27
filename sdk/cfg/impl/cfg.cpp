#include "../../sdk.hpp"

namespace sdk {
    void c_cfg::save( const std::string_view name ) const {
        const auto path = std::filesystem::path{ PROJECT_NAME } /= name;

        std::filesystem::create_directory( PROJECT_NAME );

        nlohmann::json json{};

        auto& object = json[ PROJECT_NAME ];

        for ( const auto var : m_vars )
            var->save( object );

        if ( std::ofstream file{ path, std::ios::out | std::ios::trunc } )
            file << json.dump( );
    }

    void c_cfg::load( const std::string_view name ) {
        const auto path = std::filesystem::path{ PROJECT_NAME } /= name;

        std::filesystem::create_directory( PROJECT_NAME );

        std::string str{};
        if ( std::ifstream file{ path, std::ios::in } )
            file >> str;

        if ( str.empty( ) )
            return;

        const auto json = nlohmann::json::parse( str );
        if ( !json.is_object( ) )
            return;

        const auto object = json.find( PROJECT_NAME );
        if ( object == json.end( ) )
            return;

        const auto& value = object.value( );

        for ( const auto var : m_vars )
            var->load( value );
    }
}