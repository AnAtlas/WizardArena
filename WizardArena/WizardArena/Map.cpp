#include "Map.hpp"
#include "StateManager.hpp"

Map::Map(SharedContext* context, BaseState* currentState)
	:context(context), defaultTile(context), maxMapSize(32, 32), tileCount(0),
	tileSetCount(0), mapGravity(512.f), loadNextMap(false), currentState(currentState)
{
	context->gameMap = this;
	loadTiles("tiles.cfg");
}

Map::~Map() {
	purgeMap();
	purgeTileSet();
	context->gameMap = nullptr;
}

Tile* Map::getTile(unsigned int x, unsigned int y, unsigned int layer) {
	if (x < 0 || y < 0 || x >= maxMapSize.x || y >= maxMapSize.y || layer < 0 || layer >= Sheet::NumLayers)
		return nullptr;
	
	auto itr = tileMap.find(convertCoords(x, y, layer));
	if (itr == tileMap.end())
		return nullptr;
	return itr->second;
}

TileInfo* Map::getDefaultTile() { return &defaultTile; }
float Map::getGravity() { return mapGravity; }
unsigned int Map::getTileSize() { return Sheet::TileSize; }
const sf::Vector2u& Map::getMapSize() { return maxMapSize; }
const sf::Vector2f& Map::getPlayerStart() { return playerStart; }
unsigned int Map::convertCoords(unsigned int x, unsigned int y, unsigned int layer) {
	return ((layer * maxMapSize.y + y) * maxMapSize.x + x);
}

void Map::loadNext() {
	loadNextMap = true;
}

int Map::getPlayerId() {
	return playerId;
}

void Map::update(float dT) {
	if (loadNextMap) {
		purgeMap();
		loadNextMap = false;
		if (nextMap != "")
			loadMap("media/maps/" + nextMap);
		else {
			currentState->getStateManager()->switchTo(StateType::GameOver);
		}
		nextMap = "";
	}
	sf::FloatRect viewSpace = context->window->getViewSpace();
	background.setPosition(viewSpace.left, viewSpace.top);
}

void Map::draw(unsigned int layer) {
	if (layer >= Sheet::NumLayers)
		return;
	sf::RenderWindow* window = context->window->getRenderWindow();
	sf::FloatRect viewSpace = context->window->getViewSpace();

	sf::Vector2i tileBegin(
		floor(viewSpace.left / Sheet::TileSize),
		floor(viewSpace.top / Sheet::TileSize));

	sf::Vector2i tileEnd(
		ceil((viewSpace.left + viewSpace.width) / Sheet::TileSize),
		ceil((viewSpace.top + viewSpace.height) / Sheet::TileSize));

	unsigned int count = 0;
	for (int x = tileBegin.x; x <= tileEnd.x; ++x) {
		for (int y = tileBegin.y; y <= tileEnd.y; ++y) {
			Tile* tile = getTile(x, y, layer);
			if (!tile)
				continue;
			sf::Sprite& sprite = tile->properties->sprite;
			sprite.setPosition(x * Sheet::TileSize, y * Sheet::TileSize);
			window->draw(sprite);
			++count;
		}
	}
}

void Map::purgeMap() {
	tileCount = 0;
	for (auto &itr : tileMap)
		delete itr.second;
	tileMap.clear();
	context->entityManager->purge();

	if (backgroundTexture == "")
		return;
	context->textureManager->releaseResource(backgroundTexture);
	backgroundTexture = "";
}

void Map::purgeTileSet() {
	for (auto &itr : tileSet)
		delete itr.second;
	tileSet.clear();
	tileSetCount = 0;
}

void Map::loadMap(const std::string& path) {
	std::ifstream mapFile;
	mapFile.open(Utils::GetWorkingDirectory() + path);
	if (!mapFile.is_open()) {
		std::cerr << "! Failed loading map file: " << path << std::endl;
		return;
	}
	EntityManager* entityMgr = context->entityManager;
	std::string line;
	std::cout << "--- Loading a map: " << path << std::endl;

	int playerId = -1;
	while (std::getline(mapFile, line)) {
		if (line[0] == '|')
			continue;
		std::stringstream keystream(line);
		std::string type;
		keystream >> type;
		if (type == "TILE") {
			int tileId = 0;
			keystream >> tileId;
			if (tileId < 0) {
				std::cerr << "! Bad tile id: " << tileId << std::endl;
				continue;
			}
			auto itr = tileSet.find(tileId);
			if (itr == tileSet.end()) {
				std::cerr << "! Tile id(" << tileId << ") was not found in tileset." << std::endl;
				continue;
			}
			sf::Vector2i tileCoords;
			unsigned int tileLayer = 0;
			unsigned int tileSolidity = 0;
			keystream >> tileCoords.x >> tileCoords.y >> tileLayer >> tileSolidity;
			if (tileCoords.x > maxMapSize.x || tileCoords.y > maxMapSize.y || tileLayer >= Sheet::NumLayers) {
				std::cout << "! Tile is out of range: " << tileCoords.x << " " << tileCoords.y << std::endl;
				continue;
			}
			Tile* tile = new Tile();
			// Bind properties of a tile from a set
			tile->properties = itr->second;
			tile->solid = (bool)tileSolidity;
			if (!tileMap.emplace(convertCoords(tileCoords.x, tileCoords.y, tileLayer), tile).second) {
				//Duplicate tile detected!
				std::cerr << "! Duplicate Tile! : " << tileCoords.x << " " << tileCoords.y << std::endl;
				delete tile;
				tile = nullptr;
				continue;
			}
			std::string warp;
			keystream >> warp;
			tile->warp = false;
			if (warp == "WARP")
				tile->warp = true;
		}
		else if (type == "BACKGROUND") {
			if (backgroundTexture != "")
				continue;
			keystream >> backgroundTexture;
			if (!context->textureManager->requireResource(backgroundTexture)) {
				backgroundTexture = "";
				continue;
			}
			sf::Texture* texture = context->textureManager->getResource(backgroundTexture);
			background.setTexture(*texture);
			sf::Vector2f viewSize = currentState->getView().getSize();
			sf::Vector2u textureSize = texture->getSize();
			sf::Vector2f scaleFactors;
			scaleFactors.x = viewSize.x / textureSize.x;
			scaleFactors.y = viewSize.y / textureSize.y;
			background.setScale(scaleFactors);
		}
		else if (type == "SIZE") {
			keystream >> maxMapSize.x >> maxMapSize.y;
		}
		else if (type == "GRAVITY") {
			keystream >> mapGravity;
		}
		else if (type == "DEFAULT_FRICTION") {
			keystream >> defaultTile.friction.x >> defaultTile.friction.y;
		}
		else if (type == "NEXTMAP") {
			keystream >> nextMap;
		}
		else if (type == "ENTITY") {
			//Set up entity here
			std::string name;
			keystream >> name;
			if (name == "Player" && playerId != -1)
				continue;
			int entityId = context->entityManager->addEntity(name);
			if (entityId < 0)
				continue;
			if (name == "Player")
				playerId = entityId;
			C_Base* position = context->entityManager->getComponent<C_Position>(entityId, Component::Position);
			if (position)
				keystream >> *position;
		}
		else {
			std::cerr << "! Unknown type \"" << type << "\"." << std::endl;
		}
	}
}

void Map::loadTiles(const std::string& path) {
	std::ifstream file;
	file.open(Utils::GetWorkingDirectory() + path);
	if (!file.is_open()) {
		std::cerr << "! Failed loading tile set file: " << path << std::endl;
		return;
	}
	std::string line;
	while (std::getline(file, line)) {
		if (line[0] == '|')
			continue;
		std::stringstream keystream(line);
		int tileId;
		keystream >> tileId;
		if (tileId < 0)
			continue;
		TileInfo* tile = new TileInfo(context, "TileSheet", tileId);
		keystream >> tile->name >> tile->friction.x >> tile->friction.y >> tile->deadly;
		if (!tileSet.emplace(tileId, tile).second) {
			std::cerr << "! Duplicate tile type: " << tile->name << std::endl;
			delete tile;
		}
	}
	file.close();
}