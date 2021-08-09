#pragma once
#include <SFML/Graphics.hpp>

namespace sffx
{

    class Effect
    {
        public:
        Effect() = default;
        virtual sf::Sprite operator()( const sf::Texture& input ) = 0;
        virtual ~Effect() = default;
    };

}