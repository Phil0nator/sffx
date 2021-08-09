#include <assert.h>
#include "Effects/Shake.hpp"
#include "base_vt.hpp"


namespace sffx {


    static sf::Shader shake_shader;
    static const char* shake_fg = 
R"(
uniform sampler2D texture;
uniform float time;
uniform float intensity;
void main()
{

    vec2 pos = gl_TexCoord[0].xy;
    pos.x += cos( time*3.0 )*intensity;
    pos.y += sin( time*2.0 )*intensity;
    gl_FragColor = texture2D( texture, pos );
}

)";

    Shake::Shake
    (
        unsigned width, 
        unsigned height, 
        float intensity,
        float speed
    ):
        m_region( {0,0}, {width, height} ),
        m_intensity(intensity),
        m_timer(0),
        m_speed(speed)
            {
                m_buffer.create( width,height );
                if( shake_shader.getNativeHandle() == 0 )
                {
                    assert(shake_shader.loadFromMemory( base_vt, shake_fg ));
                }
            }

    void Shake::setSize( unsigned width, unsigned height )
    {
        m_buffer.create(width,height);
    }
    sf::Vector2u Shake::getSize() const
    {
        return m_buffer.getSize();
    }

    void Shake::setSpeed( float speed )
    {
        m_speed = speed;
    }
    float Shake::getSpeed() const
    {
        return m_speed;
    }

    void Shake::setIntensity( float intensity )
    {
        m_intensity=intensity;
    }
    float Shake::getIntensity() const
    {
        return m_intensity;
    }

    sf::IntRect Shake::getRegion() const
    {
        return m_region;
    }
    void Shake::setRegion( const sf::IntRect& rect )
    {
        m_region = rect;
    }


    sf::Sprite Shake::operator()( const sf::Texture& t )
    {

        m_buffer.clear();
        sf::Sprite drawer(t);
        if ( sf::Vector2u{m_region.width, m_region.height} != m_buffer.getSize() )
        {
            m_buffer.draw(drawer);
        }
        drawer.setTextureRect(m_region);
        drawer.setPosition(m_region.left, m_region.top);
        shake_shader.setUniform("texture", sf::Shader::CurrentTexture);
        shake_shader.setUniform("time", m_timer );
        shake_shader.setUniform("intensity", m_intensity );
        m_buffer.draw( drawer, &shake_shader );
        m_buffer.display();
        m_timer+=m_speed;
        return sf::Sprite( m_buffer.getTexture() );

    }

}