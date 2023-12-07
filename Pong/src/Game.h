#pragma once

#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "EntityManager.h"

class Game
{
public:
	Game();

	void run();

private:
	sf::RenderWindow m_window;
	EntityManager m_entities;
	sf::Font m_font;
	sf::Text m_text_f;
	sf::Text m_text_s;
	sf::ContextSettings settings;
	sf::Event event;

	int m_score_f = 0;
	int m_score_s = 0;

	bool m_paused = false;
	bool m_running = true;

	int m_width = 1280;
	int m_height = 720;

	std::shared_ptr<Entity> m_player_f;
	std::shared_ptr<Entity> m_player_s;

	void init();
	void setPaused(bool paused);

	void sMovement();
	void sUserInput();
	void sRender();
	void sCollision();

	std::shared_ptr<Entity> spawnPlayer(std::shared_ptr<Entity>& player, std::string tag, float x);
	void spawnBall();
};