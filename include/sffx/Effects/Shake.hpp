#pragma once
#include "Effect.hpp"
#include "RegionBased.hpp"

namespace sffx
{
    class Shake : virtual public Effect, virtual public RegionBased
    {
        sf::RenderTexture m_buffer;
        float m_intensity;
        float m_speed;
        float m_timer;
        public:

        Shake
        ( 
            unsigned width, 
            unsigned height, 
            float intensity = 0.005f,
            float speed = 0.01f
        );

        void setIntensity( float intensity );
        float getIntensity() const;

        void setSpeed( float speed );
        float getSpeed() const;


        virtual sf::Sprite operator()( const sf::Texture& t ) override;
        virtual ~Shake() = default;
    };
} // namespace sffx
