#pragma once
#include <SFML/Graphics.hpp>
class RegionBased
{
    protected:
    sf::IntRect m_region;
    public:
    RegionBased( const sf::IntRect& rect ): m_region(rect) {}

    inline sf::IntRect getRegion() const { return m_region; }
    inline void setRegion( const sf::IntRect& rect ) { m_region = rect; }
    inline void setSize( unsigned width, unsigned height ) 
        { m_region.width = width, m_region.height = height; }
    inline sf::Vector2i getSize() const 
        { return sf::Vector2i(m_region.width,m_region.height); }
    inline void setPosition( float x, float y )
        { m_region.left = x, m_region.top = y; }
    inline sf::Vector2i getPosition() const 
        { return sf::Vector2i(m_region.left,m_region.top); }
};