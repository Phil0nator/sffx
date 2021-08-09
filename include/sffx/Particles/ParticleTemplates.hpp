#pragma once
#include "Particle.hpp"
#include "ParticleEvent.hpp"
#include "Utils/utils.hpp"

namespace sffx{
    /**
     * Smoke particles are circles that float in a direction with some
     * slight randomness, then fade out.
     */
    struct SmokeParticle : virtual public CircleParticle
    {
        // lateral movement
        float wind = 1.0;
        // vertical movement
        float speed = 1.0;
        // rate at which alpha channel fades
        int faderate = 5;
        

        SmokeParticle
        (
            sf::Vector2f location,
            float radius = randomDouble()*5,
            sf::Color color = {100,100,100},
            float wind = 1.0,
            float speed = 1.0,
            int faderate = 5
        ):  
            Particle( GenericParticleProperties(location, 0.0, color, true) ), 
            CircleParticle( sf::CircleShape(radius) ),
            wind(wind),
            speed(speed),
            faderate(faderate)
                {} 


        virtual void behavior_tick() noexcept override;

        virtual ~SmokeParticle() override = default;

    };

    /**
     * Explosion particles are glowing circles that bounce and release SmokeParticle 
     * instances, then fade out
     */
    struct ExplosionParticle : virtual public CircleParticle
    {
        // Controls speed of particles
        float intensity = 10;
        // rate at which the alpha channel fades
        int faderate = 5;
        sf::Vector2f velocity = { (float)(randomDouble()-0.5)*2, (float)(randomDouble())*-2 };
        // Initial location
        sf::Vector2f starterloc;

        ExplosionParticle(
            sf::Vector2f location,
            sf::Color c = {235, 50, 3, 255},
            float radius = 5,
            float intensity = 10,
            int faderate = 5,
            sf::Vector2f initialVelocity = {}
        ): 
            Particle( GenericParticleProperties(location, 0.0, c, true) ),
            CircleParticle( sf::CircleShape( radius ) ), 
            intensity(intensity),
            faderate(faderate),
            velocity(randomize( sf::Vector2f(), {2,-4} )+initialVelocity),
            starterloc(location)
                {}


        virtual void behavior_tick() noexcept override;

        virtual ~ExplosionParticle() override = default;
    };

    /**
     * A particle explosion spawns some SmokeParticle instances, and some ExplosionParticle
     * instances, which in turn create more smoke.
     */
    struct ParticleExplosionEvent : virtual public ParticleEvent
    {
        // origin
        sf::Vector2f location;
        // initial velocity
        sf::Vector2f iv;
        // speed + quantity of particles
        float intensity;
        // lateral movement of smoke
        float wind;
        // vertical movement of smoke
        float speed;
        // Rate at which the alpha channel fades
        int faderate;

        sf::Color smokeColor;
        sf::Color partsColor;


        ParticleExplosionEvent
        (
            sf::Vector2f location,
            sf::Vector2f iv = {},
            sf::Color smokeColor = {100,100,100,255},
            sf::Color partsColor = {230,50,3,255},
            float intensity = randomize(5.0f, 3.0f),
            float wind = randomize(1.0f, 1.0f),
            float speed = randomize(1.0f, 1.0f),
            int faderate = 5
        ):
            location(location),
            iv(iv),
            intensity(intensity),
            wind(wind),
            speed(speed),
            faderate(faderate),
            smokeColor(smokeColor),
            partsColor(partsColor)
                {}


        virtual void tickEvent(  ) override;


    };

    /**
     * A SparkParticle is a single pixel glowing yellowish-orange bouncing particle
     * that fades out.
     */
    struct SparkParticle : virtual public RectParticle
    {
        float intensity = 5.0;
        float iy; // initial y
        sf::Vector2f velocity;
        int faderate;

        SparkParticle(
            sf::Vector2f location,
            sf::Vector2f iv = {},
            float intensity = 5.0,
            int faderate = 5
        ):
            Particle( GenericParticleProperties( location, 0, {240, 215, 50}, true ) ),
            RectParticle( sf::RectangleShape( sf::Vector2f(1,1) ) ),
            intensity(intensity),
            iy ( location.y ),
            velocity( randomize( sf::Vector2f(), {1,2} )+velocity ),
            faderate(faderate)
                {}
        
        virtual void behavior_tick() noexcept override;


    };

    /**
     * Emmit spark particles from a given point / initial velocity indefinately.
     */
    struct SparkEmitter : virtual public ParticleEvent{

        // speed and quantity of sparks
        float intensity;
        // rate at which alpha channel fades
        int faderate;
        sf::Vector2f location;
        // initial velocity
        sf::Vector2f iv;


        SparkEmitter(
            sf::Vector2f location,
            sf::Vector2f iv = {},
            float intensity = 5.0,
            int faderate = 5
        ):
            intensity(intensity),
            faderate(faderate),
            location(location),
            iv(iv)
                {}


        /**
         * Warning:
         *  After calling kill on an event that is currently running in an engine,
         *  the pointer becomes unsafe because it will be deleted from another thread.
         */
        void kill() noexcept;

        virtual void tickEvent(  ) override;
    };

}