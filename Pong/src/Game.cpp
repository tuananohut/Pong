#include<iostream>

#include "Game.h"


Game::Game()
{
	init();
}

void Game::init()
{
	settings.antialiasingLevel = 8;

	m_window.create(sf::VideoMode(m_width, m_height), "Pong", sf::Style::Default, settings);

	m_window.setFramerateLimit(60);

	m_player_f = spawnPlayer(m_player_f, "player", m_window.getSize().x / 15);
	m_player_s = spawnPlayer(m_player_s, "player", m_window.getSize().x - m_window.getSize().x / 15);

	spawnBall();
}

void Game::run()
{
	while (m_running)
	{
		m_entities.update();

		if (!m_paused)
		{
			sMovement();
			sCollision();
			sUserInput();
		}

		sRender();
	}
}

void Game::setPaused(bool paused)
{
	m_paused = paused;
}

std::shared_ptr<Entity> Game::spawnPlayer(std::shared_ptr<Entity>& player, std::string tag, float x)
{
	player = m_entities.addEntity(tag);

	float mx = x;
	float my = m_window.getSize().y / 2;

	player->cTransform = std::make_shared<CTransform>(Vec2(mx, my), Vec2(1.0f, 1.0f), 0.0f);

	player->cRectangle = std::make_shared<CRectangle>(25.f, 150.f, sf::Color::White);
	
	player->cInput = std::make_shared<CInput>();

	return player;
}

void Game::spawnBall()
{
	auto entity = m_entities.addEntity("ball");

	float ex = m_window.getSize().x / 2;
	float ey = m_window.getSize().y / 2;

	entity->cTransform = std::make_shared<CTransform>(Vec2(ex, ey), Vec2(20.0f, 10.0f), 0.0f);

	entity->cShape = std::make_shared<CShape>(20, 100, sf::Color::White, sf::Color::White, 4.0f);
}


void Game::sMovement()
{
	m_player_f->cTransform->velocity = { 0, 0 };
	m_player_s->cTransform->velocity = { 0, 0 };

	if (m_player_f->cInput->up)
	{
		m_player_f->cTransform->velocity.y = -10;
	}

	if (m_player_f->cInput->down)
	{
		m_player_f->cTransform->velocity.y = +10;
	}

	if (m_player_s->cInput->up)
	{
		m_player_s->cTransform->velocity.y = -10;
	}

	if (m_player_s->cInput->down)
	{
		m_player_s->cTransform->velocity.y = +10;
	}


	for (auto& e : m_entities.getEntities())
	{
		if(e->cShape)
		{
			if (e->cTransform->pos.x + e->cShape->circle.getRadius() >= m_window.getSize().x)
			{
				e->destroy();
				m_score_f += 1;
				spawnBall();
			}

			if(e->cTransform->pos.x < 0)
			{
				e->destroy();
				m_score_s += 1;
				spawnBall();
			}
			
			if (e->cTransform->pos.y + e->cShape->circle.getRadius() >= m_window.getSize().y || e->cTransform->pos.y < 0)
			{
				e->cTransform->velocity.y *= -1;
			}
		}

		if(e->cRectangle)
		{
			float playerTop = e->cTransform->pos.y;
			float playerBottom = e->cTransform->pos.y + e->cRectangle->rectangle.getSize().y;
			
			if(playerTop < 0)
			{
				e->cTransform->pos.y = 0;
			}

			if(playerBottom > m_window.getSize().y)
			{
				e->cTransform->pos.y = m_window.getSize().y - e->cRectangle->rectangle.getSize().y;
			}
			
		}
		
		
		e->cTransform->pos.x += e->cTransform->velocity.x;
		e->cTransform->pos.y += e->cTransform->velocity.y;
	}

}

void Game::sCollision()
{
	auto players = m_entities.getEntities("player");
	auto enemies = m_entities.getEntities("ball");

	bool isCollided = false;

	for (auto& player : players)
	{
		for (auto& enemy : enemies)
		{
			float playerLeft = player->cTransform->pos.x;
			float playerRight = player->cTransform->pos.x + player->cRectangle->rectangle.getSize().x;
			float playerTop = player->cTransform->pos.y;
			float playerBottom = player->cTransform->pos.y + player->cRectangle->rectangle.getSize().y;

			float enemyRadius = enemy->cShape->circle.getRadius();
			float enemyX = enemy->cTransform->pos.x;
			float enemyY = enemy->cTransform->pos.y;

			float closestX = std::clamp(enemyX, playerLeft, playerRight);
			float closestY = std::clamp(enemyY, playerTop, playerBottom);

			float distanceX = enemyX - closestX;
			float distanceY = enemyY - closestY;

			float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
			float radiusSquared = enemyRadius * enemyRadius;

			if(distanceSquared < radiusSquared)
			{
				enemy->cTransform->velocity.x *= -1;
				enemy->cTransform->pos.x += enemy->cTransform->velocity.x;
			}
		}
	}
}


void Game::sRender()
{
	m_window.clear();
	
	if (!m_font.loadFromFile("font/opensans.ttf"))
	{
		std::cout << "work";

	}
	else
	{
		m_text_f.setFont(m_font);
		m_text_f.setString("Score: " + std::to_string(m_score_f));
		m_text_f.setCharacterSize(40);
		m_text_f.setPosition(10.0f, 2.0f);
		m_text_f.setFillColor(sf::Color::White);

		m_text_s.setFont(m_font);
		m_text_s.setString("Score: " + std::to_string(m_score_s));
		m_text_s.setCharacterSize(40);
		m_text_s.setPosition(1090.f, 2.f);
		m_text_s.setFillColor(sf::Color::White);

		m_window.draw(m_text_f);
		m_window.draw(m_text_s);
	}

	for (auto& e : m_entities.getEntities())
	{
		if (e->cRectangle)
		{
			e->cRectangle->rectangle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
			m_window.draw(e->cRectangle->rectangle);
		}
		
		if(e->cShape)
		{
			e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
			m_window.draw(e->cShape->circle);
		}

	}

	m_window.display();
}


void Game::sUserInput()
{
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
			case sf::Keyboard::W:
				m_player_f->cInput->up = true;
				break;

			case sf::Keyboard::S:
				m_player_f->cInput->down = true;
				break;

			case sf::Keyboard::P:
				setPaused(!m_paused);
				break;

			case sf::Keyboard::Up:
				m_player_s->cInput->up = true;
				break;

			case sf::Keyboard::Down:
				m_player_s->cInput->down = true;
				break;

			

			default: break;
			}
		}

		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player_f->cInput->up = false;
				break;
			case sf::Keyboard::S:
				m_player_f->cInput->down = false;
				break;

			case sf::Keyboard::Up:
				m_player_s->cInput->up = false;
				break;

			case sf::Keyboard::Down:
				m_player_s->cInput->down = false;
				break;

			default: break;
			}
		}
	}
}
