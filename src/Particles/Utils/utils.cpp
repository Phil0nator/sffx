#include "Particles/Utils/utils.hpp"
#include <random>

namespace sffx
{


    struct RandInitializer
    {
        RandInitializer()
        {
            srand(time(NULL));
        }
    };

    static RandInitializer randinit = RandInitializer();


    std::chrono::milliseconds now() noexcept
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    }

    double randomDouble()
    { return ((float)rand()/(float)(RAND_MAX)) * 1.0; }

    template<>
    sf::Vector2f randomize(const sf::Vector2f & x, const sf::Vector2f & factor)
    {
        return x + sf::Vector2f{ randomize(x.x, factor.x), randomize(x.y, factor.y) };
    }

}
