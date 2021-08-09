#pragma once
#include "Particle.hpp"
#include <list>

namespace sffx
{
    struct ParticleEvent
    {
        size_t tickCount = 0;
        bool alive = true;
        ParticleEngine* parent;
        virtual void tickEvent(  ) = 0;
        virtual ~ParticleEvent() = default;
    };
}