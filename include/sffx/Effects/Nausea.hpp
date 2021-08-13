#pragma once
#include "Effect.hpp"
#include "RegionBased.hpp"

namespace sffx
{
    class Nausea : virtual public Effect, virtual public RegionBased
    {
        sf::RenderTexture m_buffer;
        float m_intensity;
        public:
        Nausea(unsigned width, unsigned height, float intensity);

        void setIntensity(float intensity);
        float getIntensity( ) const;

        virtual sf::Sprite operator()( const sf::Texture& t ) override;

        virtual ~Nausea()=default;
    };
}
