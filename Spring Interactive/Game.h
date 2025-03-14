/// <summary>
/// author Ryan McDonald 2024
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
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <time.h>

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
	battle,
	gameOver
};

enum battleChecker
{
	NONE,
	fight,
	magic,
};

enum fightAbilities
{
	pacifist,
	stab,
	crush,
};

enum enemyAbilities
{
	slash,
	meow,
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
	unsigned screenWidth = sf::VideoMode::getDesktopMode().width;
	unsigned screenHeight = sf::VideoMode::getDesktopMode().height;

	void processEvents();
	void processKeys(sf::Event t_event);
	void processMouseClick(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();

	//UNIVERSAL FUNCTIONS
	void setupFontAndText();
	void setupSprite();
	void getMousePos();
	void checkButtons();

	//FUNCTIONS FOR MENU
	void setupMenu();

	//FUNCTIONS FOR PREBATTLE
	void setupPlayer();
	void setupNPC();
	void setupSnowdin();
	void setupButtons();
	void drawEnvironment();

	void getDirection();
	void move();
	void checkbounds();
	void interactWith();


	//FUNCTIONS FOR BATTLE
	void failsafe();
	void setupEnemy();
	void setupBattleMenu();

	void enemySelect();
	void optionSelect();
	void optionAnimate(); //FLICKER SELECTABLE ENEMY
	void playerAttack();
	void enemyTurn();
	void enemyAttack();

	//FUNCTIONS FOR AUDIO
	bool loadMusic();
	void playMusic();


	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	sf::Text m_welcomeMessage; // text used for message on screen
	bool m_exitGame; // control exiting game

	sf::Vector2i m_mousePos;
	sf::Vector2f m_mousePosF;
	
	//TEXTURES, SPRITES, AND VARIABLES FOR MENU

	bool m_startHover = false; //IF HOVERING OVER BUTTON, CAN CLICK.
	bool m_optionHover = false;
	bool m_endHover = false;
	bool m_optionEndHover = false;


	bool m_optionsOpen = false;

	sf::Text m_title;

	sf::Texture m_menuTexture;
	sf::Sprite m_menuSprite;

	sf::Text m_startButtonMessage;

	sf::Text m_OptionButtonMessage;

	sf::Text m_endButtonMessage;

	sf::Texture m_buttonTexture;
	sf::Sprite m_startSprite;
	sf::Sprite m_optionSprite;
	sf::Sprite m_endSprite;

	sf::RectangleShape m_optionsMenuRect;
	sf::RectangleShape m_resolutionDropdown;
	sf::RectangleShape m_optionClose;
	sf::Text m_optionCloseText;


	//TEXTURES, SPRITES AND VARIABLES FOR PREBATTLE
	Direction m_heading;
	sf::Vector2f m_location;
	float m_speed{ 1.6f };

	bool m_interactHover = false;
	bool m_interactPressed = false;

	sf::Texture m_backgroundTexture;
	sf::Sprite m_backgroundSprite;

	sf::RectangleShape m_playerBody; 
	sf::RectangleShape m_npcBody; 
	sf::Vector2f m_npcLocation;

	sf::RectangleShape m_interactPrompt;
	sf::Text m_interactE;
	sf::Vector2f m_interactOffset;

	//TEXTURES, SPRITES AND VARIABLES FOR BATTLE
	sf::Texture m_battleBackgroundTexture;
	sf::Sprite m_battleBackgroundSprite;

	sf::Texture m_sansSpriteSheetTexture;
	sf::Sprite m_enemyPlaceholderSprite;
	sf::Text m_enemyName;

	//SPRITES,RECTS AND TEXT FOR THE BATTLE MENUS
	sf::RectangleShape m_battleScreenRect;
	sf::RectangleShape m_subMenu;
	sf::RectangleShape m_Fight;
	sf::RectangleShape m_Magic;
	sf::RectangleShape m_Item;
	sf::RectangleShape m_Bag;

	sf::Text m_fightText;
	sf::Text m_magicText;
	sf::Text m_itemText;
	sf::Text m_bagText;

	sf::Vector2f m_enemyPos;

	//BOOLEANS FOR MENU SELECTIONS
	bool enemySelected{ false };
	bool enterPressed{ false };
	bool subMenuOpen{ true };
	bool fightMenu{ false };
	bool magicMenu{ false };

	int m_enemyNum{ 0 }; //Detects the picked enemy (for multiple enemy fights. 0 = first enemy, 1 = second, etc etc.)

	int m_enemyHealth{ 100 };
	sf::RectangleShape m_enemyHealthRect;
	sf::RectangleShape m_enemyHealthRectDamage;

	int m_playerHealth{ 100 };
	sf::Text m_playerHealthText;

	int m_blueValue{ 0 };
	battleChecker m_subMenuChecker{NONE}; //Checks the current Sub Menu. 
	fightAbilities m_fightMenuChecker{pacifist};

	int m_currentChoice{ 0 }; //Gets number of current choice without having to convert an enum to an int. Bandaid solution, will fix in final iteration.

	bool m_enemyDead{ false }; //For checking if sans has been murdered.


	//SUBMENUS
	sf::RectangleShape m_Fstab;
	sf::Text m_FstabText;

	sf::RectangleShape m_Fcrush;
	sf::Text m_FcrushText;

	//ENEMY TURN THINGS
	bool m_enemyAttacking{ false };
	bool m_canAttack{ true };
	sf::Text m_enemyAbilityText;
	enemyAbilities m_enemyAbilityChecker{slash};
	sf::Clock m_enemyAttackClock;

	//MUSIC AND SOUND EFFECTS
	sf::Music m_menuMusic;
	sf::Music m_preBattleMusic;
	sf::Music m_fightMusic;
	bool m_musicPlaying = false;

	//PREBATTLE ANIMATIONS AND SPRITES
	std::vector<sf::Sprite> snowdinSprites;
	int tileOffset{ 0 };
	sf::Vector2f m_oldFloorLocation{};
	sf::Vector2f m_newFloorLocation{};

	sf::Texture m_snowdinSpritesheet;
	sf::Sprite m_snowTileSprite;

	//BATTLE ANIMATION SPRITES
	sf::Sprite m_sansInjuredHead;
	sf::Sprite m_sansDefaultHead;
};

#endif // !GAME_HPP

