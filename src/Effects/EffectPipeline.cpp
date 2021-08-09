#include "Effects/EffectPipeline.hpp"

namespace sffx
{

    EffectPipeline::EffectPipeline( unsigned width, unsigned height )
    {
        m_buffer.create(width,height);
    }
    EffectPipeline::EffectPipeline
    ( 
        unsigned width, 
        unsigned height, 
        std::list<Effect*> && effects 
    ): m_effects(effects)
    {
        m_buffer.create(width,height);
    }

    void EffectPipeline::add( Effect* e )
    {
        m_effects.push_back(e);
    }

    void EffectPipeline::clean()
    {
        for ( auto e : m_effects ) delete e;
        m_effects.clear();
    }

    [[nodiscard]] 
    std::list< Effect* > && EffectPipeline::release()
    {
        return std::move(m_effects);
    }

    void EffectPipeline::setSize( unsigned width, unsigned height )
    {
        m_buffer.create(width, height);
    }
    sf::Vector2u EffectPipeline::getSize() const
    {
        return m_buffer.getSize();
    }

    sf::Sprite EffectPipeline::operator()( const sf::Texture& texture )
    {

        sf::Sprite sprite_buffer[2];
        bool sprite_buffer_iter = false;
        sprite_buffer[ 0 ] = sf::Sprite(texture);

        for ( auto e : m_effects )
        {
            sf::Sprite sprite = (*e)( *sprite_buffer[sprite_buffer_iter].getTexture());
            sprite_buffer_iter = !sprite_buffer_iter;
            
            if 
            (
                sprite.getScale() != sf::Vector2f{1,1} or
                sprite.getTextureRect() != sf::IntRect
                ( 
                    sf::Vector2i{}, 
                    (sf::Vector2i) m_buffer.getSize() 
                ) or
                sprite.getPosition() != sf::Vector2f{}
            )
            {
                m_buffer.draw( sprite );
                m_buffer.display();
                sprite_buffer[ sprite_buffer_iter ] = sf::Sprite(m_buffer.getTexture());
            }
            else
            {
                sprite_buffer[ sprite_buffer_iter ] = sprite;
            }
        }
        return sf::Sprite(m_buffer.getTexture());
    }

}