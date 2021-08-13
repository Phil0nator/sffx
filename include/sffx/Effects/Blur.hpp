#pragma once

#include "Effect.hpp"
#include "RegionBased.hpp"

namespace sffx
{

    class Blur : virtual public Effect, virtual public RegionBased
    {
        sf::RenderTexture m_buffers[2];
        sf::Vector2<bool> m_axis;
        float m_scale;
        float m_intensity;

        public:
        Blur( unsigned width, unsigned height, float scale = 2.00f, float intensity = 3.f, sf::Vector2<bool> axis = {true,true} );
        virtual sf::Sprite operator()( const sf::Texture& t ) override;

        sf::Vector2<bool> getAxis() const;
        void setAxis(sf::Vector2<bool> axis);

        void setIntensity( float intensity );
        float getIntensity() const;

        void setScale( float scale );
        float getScale() const;

        virtual ~Blur() = default;

    };

}