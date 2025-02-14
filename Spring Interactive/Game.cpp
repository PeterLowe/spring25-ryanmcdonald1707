/// <summary>
/// author Pete Lowe May 2022
/// you need to change the above line or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



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
		m_window.draw(startButton);
		m_window.draw(m_startButtonMessage);
		m_window.draw(endButton);
		m_window.draw(m_endButtonMessage);
		m_window.draw(m_title);
	}
	else
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
