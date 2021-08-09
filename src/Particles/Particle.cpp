#include "Particle.hpp"


namespace sffx
{

    // Each tick of a particle, some behavior is done and lifetime is incremented
    void Particle::tick() noexcept
    { 
        behavior_tick();
        
        lifetime++;
    }

}


