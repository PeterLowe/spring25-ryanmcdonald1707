/// <summary>
/// author Pete Lowe May 2022
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>

enum Direction
{
	None,
	Up,
	Down,
	Left,
	Right,
	TopRight,
	TopLeft,
	BottomRight,
	BottomLeft,
};

enum gameState
{
	menu,
	preBattle,
	battle
};

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

	gameState currentState = menu;

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void processMouseClick(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();
	
	void updateGameState();
	void setupFontAndText();
	void setupSprite();

	void setupMenu();
	void drawMenu();

	void setupPlayer();
	void getDirection();
	void move();
	void checkbounds();

	void getMousePos();
	void checkButtons();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen
	sf::Texture m_logoTexture; // texture used for sfml logo
	sf::Sprite m_logoSprite; // sprite used for sfml logo
	bool m_exitGame; // control exiting game
	bool click = false;
	bool startHover = false;
	bool endHover = false;

	sf::Vector2i mousePos;
	sf::Vector2f mousePosF;


	sf::Text m_title;

	sf::RectangleShape startButton;
	sf::Text m_startButtonMessage;

	sf::RectangleShape endButton;
	sf::Text m_endButtonMessage;

	Direction m_heading;
	sf::Vector2f m_location;
	float m_speed{ 1.6f };

	//TEXTURES AND SPRITES FOR PREBATTLE
	sf::Texture m_backgroundTexture;
	sf::Sprite m_backgroundSprite;

	sf::RectangleShape m_playerBody;

};

#endif // !GAME_HPP

