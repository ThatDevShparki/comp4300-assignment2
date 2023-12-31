#pragma once

#include "Entity.h"
#include "EntityManager.h"

#include <SFML/Graphics.hpp>


/* Config Structs */


struct PlayerConfig
{
	int   SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
	float S;
};

struct EnemyConfig
{
	int   SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
	float SMIN, SMAX;
};

struct BulletConfig
{
	int   SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L, SI;
	float S;
};


/* Game Class */


class Game
{
	sf::RenderWindow m_window;
	EntityManager    m_entities;
	sf::Font         m_font;
	sf::Text         m_text;
	PlayerConfig     m_playerConfig;
	EnemyConfig      m_enemyConfig;
	BulletConfig     m_bulletConfig;
	int              m_score               = 0;
	int              m_currentFrame        = 0;
	int              m_lastEnemySpawnTime  = 0;
	int              m_lastBulletSpawnTime = 0;
	bool             m_paused              = false;
	bool             m_running             = true;
	bool             m_debug               = false;

	std::shared_ptr<Entity> m_player;

	void init(const std::string& path);
	void setPaused(bool paused);

	void sMovement();
	void sUserInput();
	void sLifespan();
	void sRender();
	void sCollision();
	void sEnemySpawner();
	void sBulletSpawner();

	void spawnPlayer();
	void spawnEnemy();
	void clearEntities();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:
	Game(const std::string& path);

	void run();
};