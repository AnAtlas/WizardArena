#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

enum class EntityType{ Base, Enemy, Player};

enum class EntityState{ Idle, Walking, Jumping, Attacking, Hurt, Dying};

struct TileInfo;

struct CollisionElement {
	CollisionElement(float area, TileInfo* info, const sf::FloatRect& bounds)
		:area(area), tile(info), tileBounds(bounds) {}
	float area;
	TileInfo* tile;
	sf::FloatRect tileBounds;
};

using Collisions = std::vector < CollisionElement>;
bool sortCollisions(const CollisionElement& e1, const CollisionElement& e2);

class EntityManager;
class EntityBase {
	friend class EntityManager;

protected:
	void updateAABB();
	void checkCollisions();
	void resolveCollisions();

	//Method for what THIS entity does TO the collider entity.
	virtual void onEntityCollision(EntityBase* collider, bool attack) = 0;

	std::string name;
	EntityType type;
	unsigned int id;
	sf::Vector2f position;
	sf::Vector2f positionOld;
	sf::Vector2f velocity;
	sf::Vector2f maxVelocity;
	sf::Vector2f speed;
	sf::Vector2f acceleration;
	sf::Vector2f friction;
	TileInfo* referenceTile; // Tile underneath entity
	sf::Vector2f size;
	sf::FloatRect AABB;
	EntityState state;

	bool collidingOnX;
	bool collidingOnY;

	Collisions collisions;
	EntityManager* entityManager;

public:
	EntityBase(EntityManager* entityMgr);
	virtual ~EntityBase();

	void move(float x, float y);
	void addVelocity(float x, float y);
	void accelerate(float x, float y);
	void setAcceleration(float x, float y);
	void applyFriction(float x, float y);
	virtual void update(float dT);
	virtual void draw(sf::RenderWindow* window) = 0;

	const sf::Vector2f& getPosition();
	const sf::Vector2f& getSize();
	EntityState getState();
	std::string getName();
	unsigned int getId();
	EntityType getType();

	void setPosition(float& x, float& y);
	void setPosition(const sf::Vector2f& pos);
	void setSize(float& x, float& y);
	void setState(const EntityState& state);
};