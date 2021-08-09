#pragma once
#include "Effect.hpp"


namespace sffx
{
    class Shake : virtual public Effect
    {
        sf::RenderTexture m_buffer;
        float m_intensity;
        float m_speed;
        sf::IntRect m_region;
        float m_timer;
        public:

        Shake
        ( 
            unsigned width, 
            unsigned height, 
            float intensity = 0.005f,
            float speed = 0.01f
        );

        void setSize( unsigned width, unsigned height );
        sf::Vector2u getSize() const;

        void setIntensity( float intensity );
        float getIntensity() const;

        void setSpeed( float speed );
        float getSpeed() const;

        sf::IntRect getRegion() const;
        void setRegion( const sf::IntRect& rect );


        virtual sf::Sprite operator()( const sf::Texture& t ) override;
        virtual ~Shake() = default;
    };
} // namespace sffx
