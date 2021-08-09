#include "ParticleEngine.hpp"
#include "Utils/utils.hpp"

namespace sffx
{


    /**
     * Thread loop to process particles
     */
    void particleEngineProcessParticles(ParticleEngine* engine)
    {

        // Deal with new particles:
        {
            std::lock_guard<std::mutex> lock(engine->mutsyncParticles);
            engine->particles.splice( engine->particles.end(), engine->newParticles );
            engine->newParticles.clear();
        }

        // lock the particle array so that the threads do not interfere
        std::lock_guard<std::mutex> lock(engine->mutsyncParticles);

        // loop through particles
        size_t counter = 0;
        for( 
            auto iter = engine->particles.begin();
            iter != engine->particles.end();
            counter ++, 
            iter++
            )
        {
            // Update a particle
            (*iter)->tick();

            // test for end-of-life
            if (!(*iter)->properties.alive)
            {
                // if so, delete
                engine->particles.erase(iter);
                delete *iter;

                // After a deletion the iterator becomes invalid, so we must rebuild it
                iter = engine->particles.begin();
                std::advance(iter, counter);
            }
        }
    }

    void particleEngineProcessEvents(ParticleEngine* engine)
    {
        // loop through events
        size_t counter = 0;
        for( 
            auto iter = engine->events.begin();
            iter != engine->events.end();
            counter ++, 
            iter++
            )
        {
            (*iter)->tickEvent();
            if (!(*iter)->alive)
            {
                delete *iter;
                engine->events.erase(iter);
                iter = engine->events.begin();
                std::advance(iter, counter);
            }
        }


    }

    void particleEngineThread(ParticleEngine* engine)
    {
        while (engine->alive)
        {
            auto starttime = now ();
            particleEngineProcessEvents(engine);
            particleEngineProcessParticles(engine);
            auto totalTime = now()-starttime;
            if (totalTime < engine->timePerTick)
                std::this_thread::sleep_for( engine->timePerTick - totalTime );
            
        }
    }

    void ParticleEngine::start() noexcept
    {
        particlethread.launch( particleEngineThread, this );
    }
    void ParticleEngine::add(Particle* p) noexcept
    {
        std::lock_guard<std::mutex> lock( mutsyncParticles ); // lock the particles while a new one is added
        p->parent = this;
        p->tick();
        particles.push_back( p );
    }

    void ParticleEngine::addEvent(ParticleEvent* event) noexcept
    {
        std::lock_guard<std::mutex> lock (mutsyncEvents);
        event->parent = this;
        events.push_back(event);
    }

    void ParticleEngine::draw(sf::RenderTarget& dest, sf::RenderStates states) const
    {
        // lock the particles while ALL are drawn ( this might be in the middle of a tick loop ) 
        std::lock_guard<std::mutex> lock( mutsyncParticles ); 
        for ( const auto& particle : particles ) 
        {
            particle->display();
            dest.draw(*particle, states);

        }
    }

    ParticleEngine::~ParticleEngine()
    {
        alive = false;
        particlethread.join();

        for ( auto p : particles )      delete p;
        for ( auto p : newParticles )   delete p;
        for ( auto e : events )         delete e;

    }

}
