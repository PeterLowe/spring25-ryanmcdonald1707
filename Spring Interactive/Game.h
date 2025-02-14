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
#include <time.h>
#include <fstream>
#include <vector>

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

	//UNIVERSAL FUNCTIONS
	void updateGameState();
	void setupFontAndText();
	void setupSprite();
	void getMousePos();
	void checkButtons();
	void setupSpeech();

	//FUNCTIONS FOR MENU
	void setupMenu();
	void drawMenu();

	//FUNCTIONS FOR PREBATTLE
	void setupPlayer();
	void setupNPC();
	void setupButtons();
	void getDirection();
	void move();
	void checkbounds();
	void interactWith();
	void continueSpeech();

	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen
	bool m_exitGame; // control exiting game
	bool click = false;
	bool startHover = false;
	bool endHover = false;

	sf::Vector2i mousePos;
	sf::Vector2f mousePosF;

	//TEXTURES, SPRITES, AND VARIABLES FOR MENU
	sf::Text m_title;

	sf::Texture m_menuTexture;
	sf::Sprite m_menuSprite;

	sf::RectangleShape startButton;
	sf::Text m_startButtonMessage;

	sf::RectangleShape endButton;
	sf::Text m_endButtonMessage;

	Direction m_heading;
	sf::Vector2f m_location;
	float m_speed{ 1.6f };

	//TEXTURES, SPRITES AND VARIABLES FOR PREBATTLE
	bool interactHover = false;
	bool interactPressed = false;
	bool textFinished = false;

	sf::Texture m_backgroundTexture;
	sf::Sprite m_backgroundSprite;

	sf::RectangleShape m_playerBody; 
	sf::RectangleShape m_npcBody; 
	sf::Vector2f m_npcLocation;

	sf::RectangleShape m_interactPrompt;
	sf::Text m_interactE;
	sf::Vector2f m_interactOffset;

	sf::RectangleShape m_textBox;
	sf::Text m_speech;
	std::vector<std::string> lineByLine;
	std::string currentLine;
	sf::Clock cutsceneClock;
	size_t lineIndex;
};

#endif // !GAME_HPP

