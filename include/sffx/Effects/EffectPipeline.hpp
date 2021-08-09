#pragma once
#include <list>

#include "Effects/Effect.hpp"


namespace sffx
{

    class EffectPipeline : virtual public Effect
    {
        std::list< Effect* > m_effects;
        sf::RenderTexture m_buffer;
        public:
        EffectPipeline( unsigned width, unsigned height );
        EffectPipeline
        ( 
            unsigned width, 
            unsigned height, 
            std::list<Effect*> && effects 
        );

        void add( Effect* e );
        
        void setSize( unsigned width, unsigned height );
        sf::Vector2u getSize() const;

        void clean();

        [[nodiscard]] 
        std::list< Effect* > && release();

        virtual sf::Sprite operator()( const sf::Texture& texture ) override;
        virtual ~EffectPipeline() = default;

    };

};