#include "Effects/Blur.hpp"
#include "base_vt.hpp"

#include "assert.h"

namespace sffx
{

    static sf::Shader blur_shader;
    static const char* blur_fg = 
R"(
#version 330 compatibility
uniform sampler2D texture;
uniform float resolution;
uniform float radius;
uniform vec2 dir;

void main()
{
    vec4 sum = vec4(0.0);

    vec2 tc = gl_TexCoord[0].xy;

    float blur = radius/resolution;

    float hstep = dir.x;
    float vstep = dir.y;

    sum += texture2D(texture, vec2(tc.x - 4.0*blur*hstep, tc.y - 4.0*blur*vstep)) * 0.0162162162;
	sum += texture2D(texture, vec2(tc.x - 3.0*blur*hstep, tc.y - 3.0*blur*vstep)) * 0.0540540541;
	sum += texture2D(texture, vec2(tc.x - 2.0*blur*hstep, tc.y - 2.0*blur*vstep)) * 0.1216216216;
	sum += texture2D(texture, vec2(tc.x - 1.0*blur*hstep, tc.y - 1.0*blur*vstep)) * 0.1945945946;
	
	sum += texture2D(texture, vec2(tc.x, tc.y)) * 0.2270270270;
	
	sum += texture2D(texture, vec2(tc.x + 1.0*blur*hstep, tc.y + 1.0*blur*vstep)) * 0.1945945946;
	sum += texture2D(texture, vec2(tc.x + 2.0*blur*hstep, tc.y + 2.0*blur*vstep)) * 0.1216216216;
	sum += texture2D(texture, vec2(tc.x + 3.0*blur*hstep, tc.y + 3.0*blur*vstep)) * 0.0540540541;
	sum += texture2D(texture, vec2(tc.x + 4.0*blur*hstep, tc.y + 4.0*blur*vstep)) * 0.0162162162;

	//discard alpha for our simple demo, multiply by vertex color and return
	gl_FragColor =  vec4(sum.rgb, 1.0);

}


)";

    Blur::Blur( unsigned width, unsigned height, float scale, float intensity, sf::Vector2<bool> axis )
    : m_region( sf::Vector2i(0,0), sf::Vector2i(width,height) ),
    m_axis(axis),
    m_scale(scale),
    m_intensity(intensity)
    {
        m_buffers[0].create(width*scale,height*scale);
        m_buffers[0].setSmooth(true);
        m_buffers[1].create(width*scale,height*scale);
        m_buffers[1].setSmooth(true);
        
        if (blur_shader.getNativeHandle() == 0)
        {
            assert(blur_shader.loadFromMemory( base_vt, blur_fg ));
        }

    }


    sf::IntRect Blur::getRegion() const
    {
        return m_region;
    }
    void Blur::setRegion( const sf::IntRect& rect )
    {
        m_region = rect;
        m_buffers[0].create(rect.width*m_scale,rect.height*m_scale);
        m_buffers[0].setSmooth(true);
        m_buffers[1].create(rect.width*m_scale,rect.height*m_scale);
        m_buffers[1].setSmooth(true);
        
    }

    sf::Vector2<bool> Blur::getAxis() const
    {
        return m_axis;
    }
    void Blur::setAxis(sf::Vector2<bool> axis)
    {
        m_axis = axis;
    }

    sf::Sprite Blur::operator()( const sf::Texture& t )
    {
        sf::Sprite drawer {t};
        drawer.setScale( m_scale, m_scale );
        drawer.setTextureRect(m_region);
        
        if (!m_axis.x and !m_axis.y)
        {
            sf::Vector2f size = (sf::Vector2f) m_buffers[0].getSize();
            drawer.setScale( {1,1} );
            return drawer;
        }

        blur_shader.setUniform( "texture", sf::Shader::CurrentTexture );
        blur_shader.setUniform( "radius", m_intensity );
        
        sf::Sprite partA = drawer;
        int idx = 0;
        if ( m_axis.x )
        {
            blur_shader.setUniform( "resolution", (float) m_region.width*m_scale );
            blur_shader.setUniform( "dir", sf::Vector2f( 1,0 ) );

            m_buffers[idx].clear();
            m_buffers[idx].draw( drawer, &blur_shader );
            m_buffers[idx].display();
            idx++;
            partA = sf::Sprite(m_buffers[0].getTexture());
        }

        if (m_axis.y)
        {
            blur_shader.setUniform( "resolution", (float) m_region.height*m_scale );
            blur_shader.setUniform( "dir", sf::Vector2f( 0,1 ) );

            m_buffers[idx].clear();
            m_buffers[idx].draw( partA, &blur_shader );
            m_buffers[idx].display();
        }
        else {
            idx --;
        }

        sf::Sprite blurred { m_buffers[idx].getTexture() };
        sf::Vector2f size = (sf::Vector2f) m_buffers[0].getSize();
        blurred.setScale( {m_region.width/size.x,m_region.height/size.y} );
        blurred.setPosition( m_region.left, m_region.top );
        return blurred;
    }


}