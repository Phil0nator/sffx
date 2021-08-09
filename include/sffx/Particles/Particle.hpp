#pragma once
#include <SFML/Graphics.hpp>


namespace sffx
{
        

    struct ParticleEngine; // forward decl

    /**
     * GenericParticleProperties represents any properties generic to any
     * particle. They are grouped in this struct so that they are easier to 
     * construct seperately from other more specific particle properties.
     */
    struct GenericParticleProperties
    {
        sf::Vector2f location;
        float rotation = 0;
        sf::Color color;
        bool alive = false;
        
        GenericParticleProperties() = default;
        GenericParticleProperties(sf::Vector2f loc, float rot, sf::Color col, bool alive) : 
            location(loc), rotation(rot), color(col), alive(alive) {}
    };

    /**
     * The Particle abstract class defines only the generic properties of a partle:
     *  GenericParticleProperties,
     *  parent,
     *  lifetime,
     *  tick(),
     * 
     *  Setters: for color, location, and rotation
     * 
     */
    struct Particle : virtual public sf::Drawable
    {

        GenericParticleProperties properties{};
        ParticleEngine* parent;
        size_t lifetime = 0;

        Particle() = default;
        Particle( GenericParticleProperties starters = {} ) :
        properties(starters) {}

        Particle(const Particle& other) = delete;

        Particle(Particle&& other) = default;

        virtual void behavior_tick() noexcept = 0;
        virtual void setFillColor(const sf::Color& c) = 0;
        virtual void setRotation(float r) = 0;
        virtual void setPosition(const sf::Vector2f& loc) = 0;
        virtual void display() = 0;
        void tick() noexcept;

        virtual ~Particle() = default;

    };

    /**
     * ShapeParticle provides an implimentation of Particle using
     * either an sf::Shape or sf::Sprite as the renderable object.
     * No physics or other implimentation is present, so the tick()
     * function still needs to be overriden by a derived class
     */
    template<class Shape>
    struct ShapeParticle : virtual public Particle
    {
        static_assert( 
            std::is_base_of< sf::Shape, Shape>::value or std::is_same< sf::Sprite, Shape>::value, 
            "ShapeParticles must be constructed with a sf::Shape | sf::Sprite derivative." 
            );
        Shape m_base;
        ShapeParticle() = default;
        ShapeParticle(const Shape& s) : m_base(s) {}
        void setFillColor(const sf::Color& c) { m_base.setFillColor(c); }
        void setRotation(float r) {m_base.setRotation(r);}
        void setPosition(const sf::Vector2f& loc) { m_base.setPosition(loc); }
        void draw(sf::RenderTarget& dest, sf::RenderStates states) const override { dest.draw(m_base, states); }
        void display() override 
        {
            setFillColor(properties.color);
            setRotation(properties.rotation);
            setPosition(properties.location);
        }

    };


    // Quick definitions
    using CircleParticle = ShapeParticle<sf::CircleShape>;
    using RectParticle = ShapeParticle<sf::RectangleShape>;
    using ImageParticle = ShapeParticle<sf::Sprite>;


} // namespace sffx
