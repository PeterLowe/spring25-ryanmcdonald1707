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

enum battleChecker
{
	none,
	fight,
	magic,
	bag,
	run,
	MAX,
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


	//FUNCTIONS FOR BATTLE
	void failsafe();
	void setupEnemy();
	void setupBattleMenu();
	void enemySelect();
	void optionAnimate();
	void optionChange();

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

	sf::Text m_startButtonMessage;

	sf::Text m_endButtonMessage;

	sf::Texture m_buttonTexture;
	sf::Sprite m_startSprite;
	sf::Sprite m_endSprite;

	Direction m_heading;
	sf::Vector2f m_location;
	float m_speed{ 1.6f };

	//TEXTURES, SPRITES AND VARIABLES FOR PREBATTLE
	bool interactHover = false;
	bool interactPressed = false;

	sf::Texture m_backgroundTexture;
	sf::Sprite m_backgroundSprite;

	sf::RectangleShape m_playerBody; 
	sf::RectangleShape m_npcBody; 
	sf::Vector2f m_npcLocation;

	sf::RectangleShape m_interactPrompt;
	sf::Text m_interactE;
	sf::Vector2f m_interactOffset;

	//TEXTURES, SPRITES AND VARIABLES FOR BATTLE
	sf::RectangleShape m_enemyRect;

	sf::Text m_enemyName;
	sf::RectangleShape m_battleScreenRect;
	sf::RectangleShape m_subMenu;
	sf::RectangleShape m_Fight;
	sf::RectangleShape m_Magic;
	sf::RectangleShape m_Item;
	sf::RectangleShape m_Bag;

	sf::Text fightText;
	sf::Text magicText;
	sf::Text itemText;
	sf::Text bagText;

	sf::Vector2f m_enemyPos;


	bool enemySelected{ false };
	bool enterPressed{ false };
	bool subMenuOpen{ false };
	int enemyNum{ 0 };

	int enemyHealth{ 69 };
	sf::Text m_enemyHealthText;

	int blueValue{ 0 };
	battleChecker subMenuChecker{none};
	bool forward{ false };

};

#endif // !GAME_HPP

