#pragma once
#include "Effect.hpp"
#include <list>


namespace sffx
{

    class MotionBlur : virtual public Effect
    {
        public:
            MotionBlur
            ( 
                unsigned width, 
                unsigned height, 
                size_t frames = 3,
                sf::Time delay = sf::milliseconds(25) 
            );

            void setFrames( size_t frames );
            size_t getFrames() const;    
            
            virtual sf::Sprite operator()( const sf::Texture& t ) override;
            virtual ~MotionBlur() = default;


        private:
            std::list< sf::RenderTexture > m_frames;
            sf::RenderTexture m_composite;
            sf::Clock m_timer;
            sf::Time m_delay;
    };

}