/// <summary>
/// author Pete Lowe May 2022
/// you need to change the above line or lose marks
/// </summary>

#include "Game.h"
#include <iostream>
#include <time.h>
#include <fstream>
#include <vector>

/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ screenWidth, screenHeight, 32U }, "Spring Interactive 2025", sf::Style::Fullscreen},
	m_exitGame{false} //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setupMenu();
	setupPlayer();
	setupNPC();
	setupButtons();
	setupEnemy();
	setupBattleMenu();
}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
#ifdef _DEBUG
			render(); // want to debug drawing while stepping through code
#endif // _DEBUG

		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
		if (sf::Event::MouseButtonPressed == newEvent.type) //user pressed a key
		{
			processMouseClick(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Enter == t_event.key.code && enemySelected)
	{
		subMenuOpen = true;
	}

	if (sf::Keyboard::Enter == t_event.key.code && subMenuChecker == fight)
	{
		fightMenu = true;
		subMenuOpen = false;
	}
	else if (sf::Keyboard::Enter == t_event.key.code && subMenuChecker == magic)
	{
		magicMenu = true;
		subMenuOpen = false;
	}

	if (sf::Keyboard::Escape == t_event.key.code && currentState == battle )
	{
		subMenuOpen = true;
		subMenuChecker = none;
	}


	if (sf::Keyboard::Down == t_event.key.code && subMenuOpen)
	{
		subMenuChecker = magic;
	}
	if (sf::Keyboard::Up == t_event.key.code && subMenuOpen)
	{
		subMenuChecker = fight;
	}

	if (sf::Keyboard::F2 == t_event.key.code)
	{
		currentState = preBattle;
	}
	if (sf::Keyboard::E == t_event.key.code && interactHover == true)
	{
		currentState = battle;
	}
}

void Game::processMouseClick(sf::Event t_event)
{
	if (startHover == true)
	{
		currentState = preBattle;
	}
	if (endHover == true)
	{
		m_exitGame = true;
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
	if (t_deltaTime.asMilliseconds() != 16)
	{
		std::cout << "time warp" << std::endl; // expecting 60 fps of action
	}
	if (m_exitGame)
	{
		m_window.close();
	}
	if (currentState == menu)
	{
		drawMenu();
		checkButtons();
	}
	if (currentState == preBattle)
	{
		getDirection();
		move();
		checkbounds();
		interactWith();
	}
	if (currentState == battle)
	{
		failsafe();
		enemySelect();
		optionAnimate();
	}

	getMousePos();
	updateGameState();
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{

	if (currentState == menu)
	{
		m_window.clear(sf::Color::White);
		m_window.draw(m_menuSprite);
		m_window.draw(m_startSprite);
		m_window.draw(m_optionSprite);
		m_window.draw(m_startButtonMessage);
		m_window.draw(m_OptionButtonMessage);
		m_window.draw(m_title);
		if (optionsOpen == true)
		{
			m_window.draw(m_optionsMenuRect);
		}
	}

	else if(currentState == preBattle)
	{
		m_window.clear(sf::Color::White);
		m_window.draw(m_backgroundSprite);
		m_window.draw(m_npcBody);
		m_window.draw(m_playerBody);
		m_window.draw(m_welcomeMessage);
		if (interactHover == true)
		{
			m_window.draw(m_interactPrompt);
			m_window.draw(m_interactE);
		}
	}
	else if(currentState == battle)
	{
		m_window.clear(sf::Color::White);
		m_window.draw(m_enemyRect);
		if (subMenuOpen == true)
		{
			m_window.draw(m_battleScreenRect);
			m_window.draw(m_subMenu);
			m_window.draw(m_enemyName);
			m_window.draw(m_enemyHealthText);
			m_window.draw(m_Fight);
			m_window.draw(m_Magic);
		}
		if (!subMenuOpen && subMenuChecker == fight)
		{
			m_window.draw(m_battleScreenRect);
			m_window.draw(m_subMenu);
			m_window.draw(m_enemyName);
			m_window.draw(m_enemyHealthText);
		}
		else if (!subMenuOpen && subMenuChecker == magic)
		{
			m_window.draw(m_battleScreenRect);
			m_window.draw(m_subMenu);
			m_window.draw(m_enemyName);
			m_window.draw(m_enemyHealthText);
		}
	}
	else
	{
		m_window.clear(sf::Color::White);
	}

	m_window.display();
}

/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_welcomeMessage.setFont(m_ArialBlackfont);
	m_welcomeMessage.setString("SFML Game");
	m_welcomeMessage.setStyle(sf::Text::Underlined | sf::Text::Italic | sf::Text::Bold);
	m_welcomeMessage.setPosition(40.0f, 40.0f);
	m_welcomeMessage.setCharacterSize(80U);
	m_welcomeMessage.setOutlineColor(sf::Color::Red);
	m_welcomeMessage.setFillColor(sf::Color::Black);
	m_welcomeMessage.setOutlineThickness(3.0f);

}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!m_backgroundTexture.loadFromFile("ASSETS\\IMAGES\\GrassFloor.png")
		|| !m_menuTexture.loadFromFile("ASSETS\\IMAGES\\gradient1.png"))
	{
		std::cout << "Error loading Textures" << std::endl;
	}

	m_backgroundSprite.setTexture(m_backgroundTexture);
	m_menuSprite.setTexture(m_menuTexture);	
	m_menuSprite.setScale(static_cast<float>(screenWidth) / 1920, static_cast<float> (screenHeight) / 1080);
	std::cout << screenHeight << " and " << screenWidth << std::endl;
}
void Game::updateGameState()
{
	switch (currentState)
	{
		case menu:
		{
			drawMenu();	
			break;
		}
		case preBattle:
		{
			break;
		}
		case battle:
		{
			break;
		}
	}
}

void Game::setupMenu()
{
	if (!m_buttonTexture.loadFromFile("ASSETS\\IMAGES\\ButtonSprite1.png"))
	{
		std::cout << "Error loading Button Textures" << std::endl;
	}

	m_startSprite.setTexture(m_buttonTexture);
	m_startSprite.setPosition((screenWidth / 2),540.0f);
	m_startSprite.setOrigin(250.5f, 50.0f);

	m_optionSprite.setTexture(m_buttonTexture);
	m_optionSprite.setPosition((screenWidth / 2), 650.0f);
	m_optionSprite.setOrigin(250.5f, 50.0f);

	m_startButtonMessage.setFont(m_ArialBlackfont);
	m_startButtonMessage.setString("START");
	m_startButtonMessage.setPosition(screenWidth / 2, 565.0f);
	m_startButtonMessage.setOrigin(0.0f, 0.0f);

	m_OptionButtonMessage.setFont(m_ArialBlackfont);
	m_OptionButtonMessage.setString("OPTIONS");
	m_OptionButtonMessage.setPosition(screenWidth / 2, 650.0f);
	m_OptionButtonMessage.setOrigin(0.0f, 0.0f);

	m_title.setFont(m_ArialBlackfont);
	m_title.setString("Petebound");
	m_title.setPosition(693.0f, 300.0f);
	m_title.setOrigin(2.75f, 50.0f);
	m_title.setScale(3.0f, 3.0f);

	m_optionsMenuRect.setPosition(screenWidth / 2, screenHeight / 2);
	m_optionsMenuRect.setSize(sf::Vector2f(screenWidth / 2, screenHeight / 2));
	m_optionsMenuRect.setOrigin(m_optionsMenuRect.getSize().x / 2.0f, m_optionsMenuRect.getSize().y / 2.0f);

}

void Game::drawMenu()
{
}

void Game::setupPlayer()
{
	m_playerBody.setSize(sf::Vector2f(40.0f, 40.0f));
	m_playerBody.setFillColor(sf::Color::Black);
	m_location = (sf::Vector2f(500.0f, 500.0f));
}

void Game::setupNPC()
{
	m_npcBody.setSize(sf::Vector2f(40.0f, 40.0f));
	m_npcBody.setFillColor(sf::Color::Red);
	m_npcLocation = sf::Vector2f(550.0f, 500.0f);
	m_npcBody.setPosition(m_npcLocation);
}

void Game::setupButtons()
{
	m_interactPrompt.setSize(sf::Vector2f(25.0f, 25.0f));
	m_interactPrompt.setFillColor(sf::Color::Red);
	m_interactPrompt.setOrigin(-8.0f, 0.0f);
	m_interactOffset = sf::Vector2f(0.0f,40.0f);

	m_interactE.setFont(m_ArialBlackfont);
	m_interactE.setString("E");
	m_interactE.setOrigin(-9.5f, 5.5f);
}

void Game::getDirection()
{
	m_heading = Direction::None;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		m_heading = Direction::Up;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_heading = Direction::Down;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_heading = Direction::Left;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_heading = Direction::Right;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_heading = Direction::TopLeft;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_heading = Direction::TopRight;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		m_heading = Direction::BottomLeft;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_heading = Direction::BottomRight;
	}
}

void Game::move()
{
	sf::Vector2f move{0.0f,0.0f};
	switch (m_heading)
	{
		case Direction::None:
		{
			break;
		}
		case Direction::Up:
		{
			move.y = -m_speed;
			break;
		}
		case Direction::Down:
		{
			move.y = m_speed;
			break;
		}
		case Direction::Left:
		{
			move.x = -m_speed;
			break;
		}
		case Direction::Right:
		{
			move.x = m_speed;
			break;
		}
		case Direction::TopRight:
		{
			move.y = -m_speed;
			move.x = m_speed;
			break;
		}
		case Direction::TopLeft:
		{
			move.y = -m_speed;
			move.x = -m_speed;
			break;
		}
		case Direction::BottomRight:
		{
			move.y = m_speed;
			move.x = m_speed;
			break;
		}
		case Direction::BottomLeft:
		{
			move.y = m_speed;
			move.x = -m_speed;
			break;
		}
	}
	m_location += move;
	m_playerBody.setPosition(m_location);
}

void Game::checkbounds()
{
	if (m_location.x < 0.0f )
	{
		m_location.x = 0.0f;
	}
	if (m_location.x > 1080.0f)
	{
		m_location.x = 1080.0f;
	}
	if (m_location.y < 0.0f)
	{
		m_location.y = 0.0f;
	}
	if (m_location.y > 1920.0f)
	{
		m_location.y = 1920.0f;
	}
}

void Game::interactWith()
{
	if (m_playerBody.getGlobalBounds().intersects(m_npcBody.getGlobalBounds()))
	{
		interactHover = true;
		m_interactPrompt.setPosition(m_location - m_interactOffset);
		m_interactE.setPosition(m_location - m_interactOffset);
	}
	else
	{
		interactHover = false;
	}
}


//BATTLE FUNCTIONS
void Game::failsafe()
{
	startHover = false;
	endHover = false;
}

void Game::setupEnemy()
{
	m_enemyPos = sf::Vector2f(960.0f, 540.0f);

	m_enemyRect.setSize(sf::Vector2f(50.0f, 100.0f));
	m_enemyRect.setOrigin(25.0f, 50.0f);
	m_enemyRect.setPosition(m_enemyPos);
	m_enemyRect.setFillColor(sf::Color(255,0,blueValue,0));
}

void Game::setupBattleMenu()
{
	m_battleScreenRect.setPosition(0.0f,600.0f);
	m_battleScreenRect.setSize(sf::Vector2f(1920.0f, 400.0f));
	m_battleScreenRect.setFillColor(sf::Color::Black);

	m_subMenu.setPosition(1200.0f, 610.0f);
	m_subMenu.setSize(sf::Vector2f(700.0f, 375.0f));
	m_subMenu.setFillColor(sf::Color::Red);

	m_Fight.setPosition(1220.0f, 615.0f);
	m_Fight.setSize(sf::Vector2f(663.0f, 100.0f));
	m_Fight.setFillColor(sf::Color::Blue);

	m_Magic.setPosition(1220.0f, 745.0f);
	m_Magic.setSize(sf::Vector2f(663.0f, 100.0f));
	m_Magic.setFillColor(sf::Color::Blue);

	m_enemyName.setFont(m_ArialBlackfont);
	m_enemyName.setScale(2.0f,2.0f);
	m_enemyName.setString("PETER");
	m_enemyName.setFillColor(sf::Color::White);
	m_enemyName.setPosition(20.0f, 610.0f);

	m_enemyHealthText.setFont(m_ArialBlackfont);
	m_enemyHealthText.setString(std::to_string(enemyHealth));
	m_enemyHealthText.setScale(2.0f, 2.0f);
	m_enemyHealthText.setPosition(500.0f, 610.0f);


}

void Game::enemySelect()
{

	if (enemyNum == 0)
	{
		enemySelected = true;
		if (!enterPressed)
		{
			blueValue = blueValue + 5;
			m_enemyRect.setFillColor(sf::Color(255, 0, blueValue, 255));
		}
		else
		{
			m_enemyRect.setFillColor(sf::Color::Red);
		}
	}
	else
	{
		enemySelected = false;
	}

}

void Game::getMousePos()
{
	mousePos = sf::Mouse::getPosition(m_window);
	float mouseX = mousePos.x;
	float mouseY = mousePos.y;
	mousePosF = sf::Vector2f(mouseX, mouseY);
}

void Game::checkButtons()
{
	if (m_startSprite.getGlobalBounds().contains(mousePosF))
	{
		startHover = true;
		m_startSprite.setColor(sf::Color(106, 106, 106, 255));
	}
	else if (!m_startSprite.getGlobalBounds().contains(mousePosF))
	{
		startHover = false;
		m_startSprite.setColor(sf::Color::White);
	}

	if (m_optionSprite.getGlobalBounds().contains(mousePosF))
	{
		m_optionSprite.setColor(sf::Color(106,106,106, 255));
	}
	else if (!m_optionSprite.getGlobalBounds().contains(mousePosF))
	{
		m_optionSprite.setColor(sf::Color::White);
	}
}

void Game::optionAnimate()
{
	if (subMenuChecker == fight)
	{
		m_Fight.setFillColor(sf::Color(106, 106, 106, 255));
	}
	else
	{
		m_Fight.setFillColor(sf::Color::Blue);
	}

	if (subMenuChecker == magic)
	{
		m_Magic.setFillColor(sf::Color(106, 106, 106, 255));
	}
	else
	{
		m_Magic.setFillColor(sf::Color::Blue);
	}
}





