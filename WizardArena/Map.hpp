#pragma once
#include "SharedContext.hpp"
#include "BaseState.hpp"
#include "Utilities.hpp"

enum Sheet{TileSize = 32, SheetWidth = 256, SheetHeight = 256};

using TileID = unsigned int;

struct TileInfo {
	TileInfo(SharedContext* context, const std::string& texture = "", TileID id = 0)
		: context(context), id(0), deadly(false)
	{
		TextureManager* tmgr = context->textureManager;
		if (texture == "") {
			this->id = id;
			return;
		}
		if (!tmgr->requireResource(texture))
			return;
		this->texture = texture;
		this->id = id;
		sprite.setTexture(*tmgr->getResource(texture));
		sf::IntRect tileBoundaries(id % (Sheet::SheetWidth / Sheet::TileSize) * Sheet::TileSize,
			id / (Sheet::SheetHeight / Sheet::TileSize) * Sheet::TileSize, Sheet::TileSize, Sheet::TileSize);
		sprite.setTextureRect(tileBoundaries);
	}

	~TileInfo() {
		if (texture == "")
			return;
		context->textureManager->releaseResource(texture);
	}

	sf::Sprite sprite;

	TileID id;
	std::string name;
	sf::Vector2f friction;
	bool deadly;

	SharedContext* context;
	std::string texture;
};

struct Tile {
	TileInfo* properties;
	bool warp; // Is the tile a warp
	//Other flags unique to each tile.
};

using TileMap = std::unordered_map<TileID, Tile*>;
using TileSet = std::unordered_map<TileID, TileInfo*>;

class Map {
private:
	//Method to convert 2d coordinats into 1d ints
	unsigned int convertCoords(unsigned int x, unsigned int y);
	void loadTiles(const std::string& path);
	void purgeMap();
	void purgeTileSet();

	TileSet tileSet;
	TileMap tileMap;
	sf::Sprite background;
	TileInfo defaultTile;
	sf::Vector2u maxMapSize;
	sf::Vector2f playerStart;
	unsigned int tileCount;
	unsigned int tileSetCount;
	float mapGravity;
	std::string nextMap;
	bool loadNextMap;
	std::string backgroundTexture;
	BaseState* currentState;
	SharedContext* context;

public:
	Map(SharedContext* context, BaseState* currentState);
	~Map();

	Tile* getTile(unsigned int x, unsigned int y);
	TileInfo* getDefaultTile();
	float getGravity();
	unsigned int getTileSize();
	const sf::Vector2u& getMapSize();
	const sf::Vector2f& getPlayerStart();
	void loadMap(const std::string& path);
	void loadNext();
	void update(float dT);
	void draw();
};