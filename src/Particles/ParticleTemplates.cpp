#include "ParticleTemplates.hpp"
#include "ParticleEngine.hpp"
#include "Utils/utils.cpp"

namespace sffx
{


    void SmokeParticle::behavior_tick() noexcept 
    {
        properties.location.x += randomize(wind, 1.f); // wind accounts for lateral movement
        properties.location.y -= randomize(speed, 1.f); // speed accounts for how fast the smoke rises
        properties.color.a-= randomize( faderate, 2 ); // the color fades
        if ( // detect end-of-life
            lifetime >= 255.0 / faderate or // if the lifetime has gotten far enough 
            properties.color.a > 255-faderate // or if the alpha value has overflown
            )
            {
                properties.alive = false;
            }
    }

    void ExplosionParticle::behavior_tick() noexcept
    {
        properties.location += velocity*intensity; // movement

        velocity = { velocity.x*0.95f, velocity.y+(0.1f) }; // friction on x, gravity on y
        
        // do a bounce animation over the particle's inital y coordinate
        if ( properties.location.y >= starterloc.y ) 
        {
            velocity.y *= -0.5; // bounce
        }

        properties.color.a -= faderate; // fade the color 
        
        if (lifetime % 8 == 0){ // only do every 10 ticks
            // Generate smoke particles to trail behind
            auto newpart = new SmokeParticle(properties.location);
            newpart->parent = parent; // setup parent
            newpart->tick();
            parent->newParticles.push_back( newpart ); // add to the newlist
        }
        // detect end of life
        if (lifetime >= 255U / ( unsigned ) faderate) properties.alive = false;

    }

    void ParticleExplosionEvent::tickEvent(  )
    {
        if (tickCount < 1)
        {
            for (int i = 0 ; i < 10; i++)
            {
                Particle* newpart = new ExplosionParticle(
                    location,
                    partsColor,
                    5.0,
                    intensity,
                    faderate,
                    iv
                    );
                newpart->parent = parent;
                newpart->tick();
                parent->particles.push_back(newpart);

            }
        }

        if (tickCount < 9)
        {
            Particle* smoke = new SmokeParticle(
                location, 
                5.0,
                smokeColor,
                wind,
                speed,
                faderate
            );
            smoke->tick();
            smoke->parent = parent;
            parent->particles.push_back(smoke);
        }

        if (tickCount >= 10) alive = false;    
        tickCount++;
    }


    void SparkParticle::behavior_tick() noexcept 
    {
        properties.location += velocity*intensity; // movement

        velocity = { velocity.x*0.95f, velocity.y+(0.1f) }; // friction on x, gravity on y
        
        // do a bounce animation over the particle's inital y coordinate
        if ( properties.location.y >= iy ) 
        {
            velocity.y *= -0.9; // bounce
        }

        properties.color.a -= faderate;

        // detect end of life
        if (lifetime >= 255U / ( unsigned ) faderate) properties.alive = false;


    }
    void SparkEmitter::kill() noexcept
    { alive = false; }


    void SparkEmitter::tickEvent(  ) 
    {

        for (int i = 0 ; i < intensity; i++)
        {
            parent->particles.push_back( new SparkParticle(
                location,
                iv,
                intensity,
                faderate
            ) );
        }
    }



}
