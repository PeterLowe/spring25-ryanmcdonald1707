/// <summary>
/// author Pete Lowe May 2022
/// you need to change the above line or lose marks
/// </summary>

#include "Game.h"
#include <iostream>
#include <time.h>

/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 1920U, 1080U, 32U }, "Spring Interactive 2025", sf::Style::Fullscreen},
	m_exitGame{false} //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture
	setupMenu();
	setupPlayer();
	setupNPC();
	setupButtons();
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
	if (sf::Keyboard::Enter == t_event.key.code)
	{
		currentState = menu;
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
	if (sf::Mouse::Left == t_event.key.code && startHover == true)
	{
		currentState = preBattle;

	}
	if (sf::Mouse::Left == t_event.key.code && endHover == true)
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
	if (currentState == preBattle)
	{
		getDirection();
		move();
		checkbounds();
		interactWith();
	}

	checkButtons();
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
		m_window.clear(sf::Color(78, 78, 78, 255));
		m_window.draw(m_menuSprite);
		m_window.draw(startButton);
		m_window.draw(m_startButtonMessage);
		m_window.draw(endButton);
		m_window.draw(m_endButtonMessage);
		m_window.draw(m_title);
	}
	else if (currentState == preBattle)
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
	else if (currentState == battle)
	{
		m_window.clear(sf::Color::White);
		m_window.draw(m_welcomeMessage);
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
		|| !m_menuTexture.loadFromFile("ASSETS\\IMAGES\\menuBackground.png"))
	{
		std::cout << "Error loading Textures" << std::endl;
	}

	m_backgroundSprite.setTexture(m_backgroundTexture);
	m_menuSprite.setTexture(m_menuTexture);
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
			m_welcomeMessage.setString("PREBATTLE");
			break;
		}
		case battle:
		{
			m_welcomeMessage.setString("BATTLE");
			break;
		}
	}
}

void Game::setupMenu()
{
	startButton.setFillColor(sf::Color::Red);
	startButton.setPosition(940.0f, 540.0f);
	startButton.setSize(sf::Vector2f(501.0f, 100.0f));
	startButton.setOrigin(250.5f, 50.0f);

	m_startButtonMessage.setFont(m_ArialBlackfont);
	m_startButtonMessage.setString("START");
	m_startButtonMessage.setPosition(1135.f, 565.0f);
	m_startButtonMessage.setOrigin(250.5f, 50.0f);

	endButton.setFillColor(sf::Color::Red);
	endButton.setPosition(940.0f, 650.0f);
	endButton.setSize(sf::Vector2f(501.0f, 100.0f));
	endButton.setOrigin(250.5f, 50.0f);

	m_endButtonMessage.setFont(m_ArialBlackfont);
	m_endButtonMessage.setString("END");
	m_endButtonMessage.setPosition(1135.f,675.0f);
	m_endButtonMessage.setOrigin(230.75f, 50.0f);

	m_title.setFont(m_ArialBlackfont);
	m_title.setString("Petebound");
	m_title.setPosition(693.0f, 300.0f);
	m_title.setOrigin(2.75f, 50.0f);
	m_title.setScale(3.0f, 3.0f);
}

void Game::drawMenu()
{
	m_welcomeMessage.setString("MENU");
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
			std::cout << "MOVEMENT UP PRESSED";
			move.y = -m_speed;
			break;
		}
		case Direction::Down:
		{
			std::cout << "MOVEMENT DOWN PRESSED";
			move.y = m_speed;
			break;
		}
		case Direction::Left:
		{
			std::cout << "MOVEMENT LEFT PRESSED";
			move.x = -m_speed;
			break;
		}
		case Direction::Right:
		{
			std::cout << "MOVEMENT RIGHT PRESSED";
			move.x = m_speed;
			break;
		}
		case Direction::TopRight:
		{
			std::cout << "MOVEMENT TOPRIGHT PRESSED";
			move.y = -m_speed;
			move.x = m_speed;
			break;
		}
		case Direction::TopLeft:
		{
			std::cout << "MOVEMENT TOPLEFT PRESSED";
			move.y = -m_speed;
			move.x = -m_speed;
			break;
		}
		case Direction::BottomRight:
		{
			std::cout << "MOVEMENT BOTTOMRIGHT PRESSED";
			move.y = m_speed;
			move.x = m_speed;
			break;
		}
		case Direction::BottomLeft:
		{
			std::cout << "MOVEMENT BOTTOMLEFT PRESSED";
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
		m_interactPrompt.setFillColor(sf::Color::Red);
		m_interactE.setFillColor(sf::Color::White);
	}
	else
	{
		interactHover = false;
		m_interactPrompt.setFillColor(sf::Color(0, 0, 0, 0));
		m_interactE.setFillColor(sf::Color(0, 0, 0, 0));
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
	if (startButton.getGlobalBounds().contains(mousePosF))
	{
		startHover = true;
		startButton.setOutlineColor(sf::Color(141, 0, 0, 255));
		startButton.setOutlineThickness(4);
		startButton.setFillColor(sf::Color(171, 3, 3, 255));
	}
	else if (!startButton.getGlobalBounds().contains(mousePosF))
	{
		startHover = false;
		startButton.setOutlineThickness(0);
		startButton.setFillColor(sf::Color::Red);
	}

	if (endButton.getGlobalBounds().contains(mousePosF))
	{
		endHover = true;
		endButton.setOutlineColor(sf::Color(141, 0, 0, 255));
		endButton.setOutlineThickness(4);
		endButton.setFillColor(sf::Color(171, 3, 3, 255));
	}
	else if (!endButton.getGlobalBounds().contains(mousePosF))
	{
		endHover = false;
		endButton.setOutlineThickness(0);
		endButton.setFillColor(sf::Color::Red);
	}
}


