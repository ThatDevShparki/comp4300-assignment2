#include "Game.h"

#include <iostream>
#include <fstream>
#include <random>
#include <cmath>


/* Constructors and Initializers */


Game::Game(const std::string& path)
{
	init(path);
}

void Game::init(const std::string& path)
{
	// TODO: read in config file here
	//		 use the pre-made PlayerConfig, EnemyConfig, and BulletConfig structs
	//	std::ifstream fin(path); // something like this to start
	//	fin >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.FR >> m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >> m_playerConfig.V >> m_playerConfig.S;
	m_playerConfig = { 32, 40, 10, 10, 10, 0, 0, 255, 4, 9, 4.0f };
	m_enemyConfig  = { 16, 18, 255, 0, 0, 4, 4, 8, 15, 60, 1.0f, 5.0f };
	m_bulletConfig = { 2, 8, 255, 0, 0, 255, 255, 255, 1, 3, 60, 3, 5.0f };

	// setup default window parameters
	m_window.create(sf::VideoMode::getFullscreenModes().back(), "Assignment 2");
	m_window.setFramerateLimit(120);
	m_window.setVerticalSyncEnabled(true);

	m_font.loadFromFile("font.ttf");

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	spawnPlayer();
}

/* Game Loop */


void Game::setPaused(bool paused)
{
	m_paused = paused;
}

void Game::run()
{
	// TODO: Add pause functionality. Some systems should function while paused (like rendering)
	//		 but others should not (like movement / input)

	while (m_running)
	{
		m_entities.update();

		sBulletSpawner();
		sEnemySpawner();
		sMovement();
		sCollision();
		sUserInput();
		sLifespan();
		sRender();

		m_currentFrame++;
	}
}


/* Spawners */


void Game::spawnPlayer()
{
	auto entity = m_entities.addEntity("player");

	size_t mx = m_window.getSize().x / 2;
	size_t my = m_window.getSize().y / 2;

	entity->cTransform = std::make_shared<CTransform>(
		Vec2(float(mx), float(my)),
		Vec2(0, 0),
		0.0f
	);
	entity->cShape     = std::make_shared<CShape>(
		float(m_playerConfig.SR),
		m_playerConfig.V,
		sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
		sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB),
		float(m_playerConfig.OT)
	);
	entity->cCollision = std::make_shared<CCollision>(float(m_playerConfig.CR));
	entity->cInput     = std::make_shared<CInput>();

	m_player = entity;
}

void Game::spawnEnemy()
{
	auto entity = m_entities.addEntity("enemy");

	size_t ex = rand() % m_window.getSize().x;
	size_t ey = rand() % m_window.getSize().y;
	int    es = rand() % int(m_enemyConfig.SMAX - m_enemyConfig.SMIN) +
				m_enemyConfig.SMIN;
	int    ev = rand() % int(m_enemyConfig.VMAX - m_enemyConfig.VMIN) +
				m_enemyConfig.VMIN;

	std::cout << m_enemyConfig.VMIN << " " << m_enemyConfig.VMAX << " " << ev
			  << std::endl;

	entity->cTransform = std::make_shared<CTransform>(
		Vec2(float(ex), float(ey)),
		Vec2(float(es), float(es)),
		0.0f
	);
	entity->cShape     = std::make_shared<CShape>(
		float(m_enemyConfig.SR),
		ev,
		sf::Color(0, 0, 0),
		sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB),
		float(m_enemyConfig.OT)
	);
	entity->cCollision = std::make_shared<CCollision>(float(m_enemyConfig.CR));
	entity->cLifespan  = std::make_shared<CLifespan>(m_enemyConfig.L);

	m_lastEnemySpawnTime = m_currentFrame;
}

void Game::clearEntities()
{
	for (const auto& enemy: m_entities.getEntities())
	{
		if (enemy->tag() != "player")
		{
			enemy->destroy();
		}
	}
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	// TODO: Implement this
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
	auto bullet = m_entities.addEntity("bullet");

	bullet->cTransform = std::make_shared<CTransform>(
		m_player->cTransform->pos,
		(target - m_player->cTransform->pos).norm() * m_bulletConfig.S,
		0
	);
	bullet->cShape     = std::make_shared<CShape>(
		float(m_bulletConfig.SR),
		m_bulletConfig.V,
		sf::Color::White,
		sf::Color::Green,
		m_bulletConfig.OT
	);
	bullet->cCollision = std::make_shared<CCollision>(
		m_bulletConfig.CR
	);
	bullet->cLifespan  = std::make_shared<CLifespan>(m_bulletConfig.L);

	m_lastBulletSpawnTime = m_currentFrame;
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
	// TODO: Implement this
}


/* Services */


void Game::sMovement()
{
	float modx = 0.0f;
	float mody = 0.0f;

	if (m_player->cInput->up)
	{
		mody -= 1.0f;
	}
	if (m_player->cInput->down)
	{
		mody += 1.0f;
	}
	if (m_player->cInput->left)
	{
		modx -= 1.0f;
	}
	if (m_player->cInput->right)
	{
		modx += 1.0f;
	}

	m_player->cTransform->vel = Vec2(
		modx * m_playerConfig.S,
		mody * m_playerConfig.S
	);

	for (const auto& entity: m_entities.getEntities())
	{
		if (entity->cTransform and entity->cCollision)
		{
			entity->cTransform->pos += entity->cTransform->vel;

			auto& ex = entity->cTransform->pos.x;
			auto& ey = entity->cTransform->pos.y;
			auto& er = entity->cCollision->radius;

			if (ex - er < 0 || ex + er > float(m_window.getSize().x))
			{
				if (entity->tag() == "bullet")
				{
					entity->destroy();
				}
				else
				{
					entity->cTransform->vel.x *= -1;
					if (ex < er)
					{
						ex = er;
					}
					else
					{
						ex = float(m_window.getSize().x) - er;
					}
				}
			}

			if (ey - er < 0 || ey + er > float(m_window.getSize().y))
			{
				if (entity->tag() == "bullet")
				{
					entity->destroy();
				}
				else
				{
					entity->cTransform->vel.y *= -1;
					if (ey < er)
					{
						ey = er;
					}
					else
					{
						ey = float(m_window.getSize().y) - er;
					}

				}

			}
		}
	}
}

void Game::sUserInput()
{
	sf::Event event{};
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				m_running = false;
				break;
			case sf::Keyboard::P:
				m_paused = !m_paused;
				break;
			case sf::Keyboard::W:
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = true;
				break;
			case sf::Keyboard::C:
				clearEntities();
				break;
			case sf::Keyboard::R:
				m_debug = !m_debug;
				break;
			case sf::Keyboard::Space:
				m_player->cInput->shoot = true;
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = false;
				break;
			case sf::Keyboard::Space:
				m_player->cInput->shoot = false;
				break;
			default:
				break;
			}
		}
	}
}

void Game::sLifespan()
{
	// TODO: implement this
}

void Game::sRender()
{
	m_window.clear();

	for (const auto& entity: m_entities.getEntities())
	{
		if (entity->cShape)
		{
			entity->cShape->circle.setPosition(
				entity->cTransform->pos.x,
				entity->cTransform->pos.y
			);
			if (entity->tag() == "enemy")
			{
				entity->cTransform->angle += 1.0f;
			}
			entity->cShape->circle.setRotation(entity->cTransform->angle);
			m_window.draw(entity->cShape->circle);

			if (m_debug)
			{
				if (entity->cCollision)
				{
					sf::CircleShape collisionCircle(entity->cCollision->radius);
					collisionCircle.setOrigin(
						entity->cCollision->radius,
						entity->cCollision->radius
					);
					collisionCircle.setPosition(
						entity->cTransform->pos.x,
						entity->cTransform->pos.y
					);
					collisionCircle.setFillColor(sf::Color::Transparent);
					collisionCircle.setOutlineColor(sf::Color::Magenta);
					collisionCircle.setOutlineThickness(2.0f);
					m_window.draw(collisionCircle);
				}
			}

		}
	}

	if (m_debug)
	{
		sf::Text liveEntities;
		liveEntities.setFont(m_font);
		liveEntities.setString(
			"Entities: " + std::to_string(m_entities.entityCount()));
		liveEntities.setCharacterSize(24);
		liveEntities.setFillColor(sf::Color::White);
		liveEntities.setStyle(sf::Text::Bold);
		m_window.draw(liveEntities);
	}

	m_window.display();
}

void Game::sCollision()
{
	for (const auto& enemy: m_entities.getEntities("enemy"))
	{
		for (const auto& bullet: m_entities.getEntities("bullet"))
		{
			if (!enemy->isActive() || !bullet->isActive())
			{
				continue;
			}

			auto ex = enemy->cTransform->pos.x;
			auto ey = enemy->cTransform->pos.y;
			auto er = enemy->cCollision->radius;

			auto bx = bullet->cTransform->pos.x;
			auto by = bullet->cTransform->pos.y;
			auto br = bullet->cCollision->radius;

			if (powf(ex - bx, 2) + powf(ey - by, 2) < powf(br + er, 2))
			{
				enemy->destroy();
				bullet->destroy();
			}
		}
	}
}

void Game::sEnemySpawner()
{
	if (m_currentFrame - m_lastEnemySpawnTime > m_enemyConfig.SI)
	{
		spawnEnemy();
	}
}

void Game::sBulletSpawner()
{
	if (m_player->cInput->shoot and
		m_currentFrame - m_lastBulletSpawnTime > m_bulletConfig.SI)
	{
		auto mousePos = sf::Mouse::getPosition(m_window);
		spawnBullet(
			m_player,
			Vec2(float(mousePos.x), float(mousePos.y))
		);
	}
}

