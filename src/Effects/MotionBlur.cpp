#include <SFML/Graphics.hpp>
#include "Effects/MotionBlur.hpp"

namespace sffx
{

    MotionBlur::MotionBlur( unsigned width, unsigned height, size_t frames, sf::Time delay ): 
        Effect(),
        m_frames( frames ),
        m_delay( delay )
    {
        for (auto& f : m_frames) f.create( width, height );
        m_composite.create(width,height);
    }

    
    size_t MotionBlur::getFrames() const
    { return m_frames.size(); }

    void MotionBlur::setFrames( size_t frames )
    { m_frames.resize( frames ); } 
    
    sf::Sprite MotionBlur::operator()( const sf::Texture& t )
    {
        if ( m_timer.getElapsedTime() >= m_delay )
        {
            auto prev = m_frames.begin();
            for ( auto iter = m_frames.begin()++; iter != m_frames.end(); iter++ )
            {
                prev->clear();
                prev->draw( sf::Sprite( iter->getTexture() ) );
                prev->display();
                prev = iter;
            }
            m_frames.back().clear();
            m_frames.back().draw( sf::Sprite( t ), sf::BlendAlpha );
            m_frames.back().display();

            m_composite.clear();
            int alpha = 255;
            int delta = 255 / getFrames();

            for ( const auto& frame : m_frames )
            {
                sf::Sprite sp(frame.getTexture());
                sf::Color color = sp.getColor();
                color.a = alpha;
                sp.setColor( color );
                m_composite.draw( sp );
                alpha -= delta;
            }

            m_composite.display();
            m_timer.restart();
        }

        return sf::Sprite( m_composite.getTexture() );
    }

    

} // namespace sffx
