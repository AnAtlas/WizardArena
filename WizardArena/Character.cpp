#include "Character.hpp"
#include "EntityManager.hpp"
#include "StateManager.hpp"

Character::Character(EntityManager* entityMgr)
	:EntityBase(entityMgr), spriteSheet(entityManager->getContext()->textureManager),
	jumpVelocity(250), hitpoints(5)
{
	name = "Character";
}

Character::~Character() {

}
void Character::move(const Direction& dir) {
	if (getState() == EntityState::Dying)
		return;
	spriteSheet.setDirection(dir);
	if (dir == Direction::Left)
		accelerate(-speed.x, 0);
	else
		accelerate(speed.x, 0);
	if (getState() == EntityState::Idle)
		setState(EntityState::Walking);
}

void Character::jump() {
	if (getState() == EntityState::Dying ||
		getState() == EntityState::Jumping ||
		getState() == EntityState::Hurt)
		return;
	setState(EntityState::Jumping);
	addVelocity(0, -jumpVelocity);
}

void Character::attack() {
	if (getState() == EntityState::Dying ||
		getState() == EntityState::Jumping ||
		getState() == EntityState::Hurt ||
		getState() == EntityState::Attacking)
		return;
	setState(EntityState::Attacking);
}

void Character::getHurt(const int& damage) {
 	if (getState() == EntityState::Dying ||
		getState() == EntityState::Hurt)
		return;
	hitpoints = (hitpoints - damage > 0 ? hitpoints - damage : 0);
	if (hitpoints)
		setState(EntityState::Hurt);
	else
		setState(EntityState::Dying);
}

void Character::updateAttackAABB() {
	attackAABB.left = (spriteSheet.getDirection() == Direction::Left ? (AABB.left - attackAABB.width) - attackAABBoffset.x :
		(AABB.left + AABB.width) + attackAABBoffset.x);
	attackAABB.top = AABB.top + attackAABBoffset.y;
}

void Character::animate() {
	EntityState st = getState();

	if (st == EntityState::Walking && spriteSheet.getCurrentAnim()->getName() != "Walk")
		spriteSheet.setAnimation("Walk", true, true);
	else if (st == EntityState::Jumping && spriteSheet.getCurrentAnim()->getName() != "Jump")
		spriteSheet.setAnimation("Jump", true, false);
	else if (st == EntityState::Attacking && spriteSheet.getCurrentAnim()->getName() != "Attack")
		spriteSheet.setAnimation("Attack", true, false);
	else if (st == EntityState::Hurt && spriteSheet.getCurrentAnim()->getName() != "Hurt")
		spriteSheet.setAnimation("Hurt", true, false);
	else if (st == EntityState::Dying && spriteSheet.getCurrentAnim()->getName() != "Dying")
		spriteSheet.setAnimation("Dying", true, false);
	else if (st == EntityState::Idle && spriteSheet.getCurrentAnim()->getName() != "Idle")
		spriteSheet.setAnimation("Idle", true, true);
}

void Character::update(float dT) {
	EntityBase::update(dT);
	if (attackAABB.width != 0 && attackAABB.height != 0) 
		updateAttackAABB();
	if (getState() != EntityState::Dying && getState() != EntityState::Attacking && getState() != EntityState::Hurt) {
		if (abs(velocity.y) >= 0.001f)
			setState(EntityState::Jumping);
		else if (abs(velocity.x) >= 0.1f)
			setState(EntityState::Walking);
		else
			setState(EntityState::Idle);
	}
	else if (getState() == EntityState::Attacking || getState() == EntityState::Hurt) {
		if (!spriteSheet.getCurrentAnim()->isPlaying()) {
			setState(EntityState::Idle);
		}
	}
	else if (getState() == EntityState::Dying) {
		if (spriteSheet.getCurrentAnim()->isPlaying())
			entityManager->remove(id);
	}
	animate();
	spriteSheet.update(dT);
	spriteSheet.setSpritePosition(position);
}

void Character::draw(sf::RenderWindow* window) {
	spriteSheet.draw(window);
}

void Character::load(const std::string& path) {
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + std::string("media/Characters/") + path);
	if (!file.is_open()) { std::cout << "! Failed loading the character file: " << path << std::endl; return; }
	std::string line;
	while (std::getline(file, line)) {
		if (line[0] == '|')
			continue;
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == "Name") {
			keystream >> name;
		}
		else if (type == "Spritesheet") {
			std::string path;
			keystream >> path;
			spriteSheet.loadSheet("media/SpriteSheets/" + path);
		}
		else if (type == "Hitpoints") {
			keystream >> hitpoints;
		}
		else if (type == "BoundingBox") {
			sf::Vector2f boundingSize;
			keystream >> boundingSize.x >> boundingSize.y;
			setSize(boundingSize.x, boundingSize.y);
		}
		else if (type == "DamageBox") {
			keystream >> attackAABBoffset.x >> attackAABBoffset.y >> attackAABB.width >> attackAABB.height;
		}
		else if (type == "Speed") {
			keystream >> speed.x >> speed.y;
		}
		else if (type == "JumpVelocity") {
			keystream >> jumpVelocity;
		}
		else if (type == "MaxVelocity") {
			keystream >> maxVelocity.x >> maxVelocity.y;
		}
		else {
			std::cerr << "! Unknown type in character file: " << type << std::endl;
		}
	}
	file.close();
}