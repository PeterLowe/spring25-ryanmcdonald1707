/// <summary>
/// author Ryan McDonald 2024
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
	loadMusic();
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
	if (sf::Keyboard::Enter == t_event.key.code && !enemySelected && !m_enemyDead) //SELECTS the enemy so you can fight him
	{
		subMenuOpen = true;
		enemySelected = true;
	}

	if (sf::Keyboard::Enter == t_event.key.code && m_subMenuChecker == fight) //Opens the Fight menu
	{
		fightMenu = true;
		subMenuOpen = false;
	}
	else if (sf::Keyboard::Enter == t_event.key.code && m_subMenuChecker == magic) //Opens the magic menu
	{
		magicMenu = true;
		subMenuOpen = false;
	}

	if (sf::Keyboard::Enter == t_event.key.code 
		&& m_fightMenuChecker == stab
		&& m_canAttack) //Initiate Stab Attack
	{
		playerAttack();
	}
	if (sf::Keyboard::Enter == t_event.key.code 
		&& m_fightMenuChecker == crush
		&& m_canAttack) //Initiate Crush Attack
	{
		playerAttack();
	}

	if (sf::Keyboard::Escape == t_event.key.code && currentState == battle ) //Brings back to first menu
	{
		subMenuOpen = true;
		m_fightMenuChecker = pacifist;
	}

	if (sf::Keyboard::Up == t_event.key.code && currentState == battle && !m_enemyDead) //Decreases choice 
	{
		m_currentChoice = m_currentChoice--;
		optionSelect();
	}
	if (sf::Keyboard::Down == t_event.key.code && currentState == battle && !m_enemyDead) //Increases choice
	{
		m_currentChoice = m_currentChoice++;
		optionSelect();
	}
	

	if (sf::Keyboard::F2 == t_event.key.code)
	{
		currentState = preBattle;
	}

	if (sf::Keyboard::E == t_event.key.code && m_interactHover == true)
	{
		currentState = battle;
		m_musicPlaying = false;
	}
}

void Game::processMouseClick(sf::Event t_event)
{
	if (m_startHover == true)
	{
		currentState = preBattle;
		m_musicPlaying = false;
	}

	if (m_optionHover == true)
	{
		m_optionsOpen = true;
	}

	if (m_optionEndHover == true)
	{
		m_optionsOpen = false;
	}

	if (m_endHover == true)
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

	if (m_enemyAttackClock.getElapsedTime().asSeconds() >= 3)
	{
		m_canAttack = true;
		m_enemyAttacking = false;
	}

	if (!m_musicPlaying)
	{
		playMusic();
	}

	getMousePos();

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
		m_window.draw(m_endSprite);
		m_window.draw(m_startButtonMessage);
		m_window.draw(m_OptionButtonMessage);
		m_window.draw(m_endButtonMessage);
		m_window.draw(m_title);
		if (m_optionsOpen == true)
		{
			m_window.draw(m_optionsMenuRect);
			m_window.draw(m_resolutionDropdown);
			m_window.draw(m_optionClose);
			m_window.draw(m_optionCloseText);
		}
	}

	else if(currentState == preBattle)
	{
		m_window.clear(sf::Color::White);
		m_window.draw(m_backgroundSprite);
		m_window.draw(m_npcBody);
		m_window.draw(m_playerBody);
		m_window.draw(m_welcomeMessage);
		if (m_interactHover == true)
		{
			m_window.draw(m_interactPrompt);
			m_window.draw(m_interactE);
		}
	}
	else if(currentState == battle)
	{
		m_window.clear(sf::Color::White);
		m_window.draw(m_battleBackgroundSprite);
		m_window.draw(m_enemyPlaceholderSprite);
		if (m_enemyHealth < 50)
		{
			m_window.draw(m_sansInjuredHead);
		}
		m_window.draw(m_enemyHealthRectDamage);
		m_window.draw(m_enemyHealthRect);
		if (subMenuOpen == true && enemySelected)
		{
			m_window.draw(m_battleScreenRect);
			m_window.draw(m_subMenu);
			m_window.draw(m_enemyName);
			m_window.draw(m_playerHealthText);
			m_window.draw(m_Fight);
			m_window.draw(m_fightText);
			m_window.draw(m_Magic);
			m_window.draw(m_magicText);
		}
		if (!subMenuOpen && m_subMenuChecker == fight && enemySelected)
		{
			m_window.draw(m_battleScreenRect);
			m_window.draw(m_subMenu);
			if (!m_enemyAttacking)
			{
				m_window.draw(m_enemyName);
				m_window.draw(m_playerHealthText);
			}
			else if (m_enemyAttacking)
			{
				m_window.draw(m_enemyAbilityText);
			}
			m_window.draw(m_Fstab);
			m_window.draw(m_FstabText);
			m_window.draw(m_Fcrush);
			m_window.draw(m_FcrushText);
		}
		else if (!subMenuOpen && m_subMenuChecker == magic && enemySelected)
		{
			m_window.draw(m_battleScreenRect);
			m_window.draw(m_subMenu);
			m_window.draw(m_enemyName);
			m_window.draw(m_playerHealthText);
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
	if (!m_ArialBlackfont.loadFromFile("ASSETS\\FONTS\\chara.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}
	m_welcomeMessage.setFont(m_ArialBlackfont);

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
	m_menuSprite.setScale(screenWidth / 1920.0f,screenHeight/ 1080.0f);
}

void Game::getMousePos()
{
	m_mousePos = sf::Mouse::getPosition(m_window);
	float mouseX = m_mousePos.x;
	float mouseY = m_mousePos.y;
	m_mousePosF = sf::Vector2f(mouseX, mouseY);
}

void Game::setupMenu()
{
	if (!m_buttonTexture.loadFromFile("ASSETS\\IMAGES\\ButtonSprite1.png"))
	{
		std::cout << "Error loading Button Textures" << std::endl;
	}

	m_startSprite.setTexture(m_buttonTexture);
	m_startSprite.setPosition((screenWidth / 2),540.0f);
	m_startSprite.setOrigin(m_startSprite.getLocalBounds().width / 2, m_startSprite.getLocalBounds().height / 2);

	m_optionSprite.setTexture(m_buttonTexture);
	m_optionSprite.setPosition((screenWidth / 2), 650.0f);
	m_optionSprite.setOrigin(m_optionSprite.getLocalBounds().width / 2, m_optionSprite.getLocalBounds().height / 2);

	m_endSprite.setTexture(m_buttonTexture);
	m_endSprite.setPosition((screenWidth / 2), 760.0f);
	m_endSprite.setOrigin(m_endSprite.getLocalBounds().width / 2, m_endSprite.getLocalBounds().height / 2);

	m_startButtonMessage.setFont(m_ArialBlackfont);
	m_startButtonMessage.setString("START");
	m_startButtonMessage.setOrigin(m_startButtonMessage.getLocalBounds().width / 2, m_startButtonMessage.getLocalBounds().height / 2);
	m_startButtonMessage.setPosition(screenWidth / 2, m_startSprite.getPosition().y - 5);

	m_OptionButtonMessage.setFont(m_ArialBlackfont);
	m_OptionButtonMessage.setString("OPTIONS");
	m_OptionButtonMessage.setOrigin(m_OptionButtonMessage.getLocalBounds().width / 2, m_OptionButtonMessage.getLocalBounds().height / 2);
	m_OptionButtonMessage.setPosition(screenWidth / 2, m_optionSprite.getPosition().y - 5);

	m_endButtonMessage.setFont(m_ArialBlackfont);
	m_endButtonMessage.setString("END");
	m_endButtonMessage.setOrigin(m_endButtonMessage.getLocalBounds().width / 2, m_endButtonMessage.getLocalBounds().height / 2);
	m_endButtonMessage.setPosition(screenWidth / 2, m_endSprite.getPosition().y - 5);

	m_title.setFont(m_ArialBlackfont);
	m_title.setString("PETEBOUND");
	m_title.setCharacterSize(120u);
	m_title.setPosition(screenWidth / 2, 300.0f);
	m_title.setOrigin(m_title.getLocalBounds().width / 2, m_title.getLocalBounds().height / 2);

	m_optionsMenuRect.setPosition(screenWidth / 2, screenHeight / 2);
	m_optionsMenuRect.setSize(sf::Vector2f(screenWidth / 1.5, screenHeight / 1.5));
	m_optionsMenuRect.setOrigin(m_optionsMenuRect.getSize().x / 2.0f, m_optionsMenuRect.getSize().y / 2.0f);
	m_optionsMenuRect.setFillColor(sf::Color(0, 0, 0, 200));

	m_resolutionDropdown.setPosition(m_optionsMenuRect.getOrigin().x,m_optionsMenuRect.getOrigin().y);
	m_resolutionDropdown.setSize(sf::Vector2f(screenWidth / 3, screenHeight / 9));
	m_resolutionDropdown.setFillColor(sf::Color::Black);

	m_optionClose.setPosition(m_optionsMenuRect.getPosition() - m_optionsMenuRect.getOrigin());
	m_optionClose.setSize(sf::Vector2f(50.0f,50.0f));
	m_optionClose.setFillColor(sf::Color::Red);

	m_optionCloseText.setFont(m_ArialBlackfont);
	m_optionCloseText.setString("X");
	m_optionCloseText.setCharacterSize(60u);
	m_optionCloseText.setPosition(m_optionClose.getPosition().x + 24, m_optionClose.getPosition().y + 10);
	m_optionCloseText.setOrigin(m_optionCloseText.getLocalBounds().width / 2, m_optionCloseText.getLocalBounds().height / 2);
}

void Game::checkButtons()
{
	if (m_startSprite.getGlobalBounds().contains(m_mousePosF) && !m_optionsOpen)
	{
		m_startHover = true;
		m_startSprite.setColor(sf::Color(106, 106, 106, 255));
	}
	else if (!m_startSprite.getGlobalBounds().contains(m_mousePosF))
	{
		m_startHover = false;
		m_startSprite.setColor(sf::Color::White);
	}

	if (m_optionSprite.getGlobalBounds().contains(m_mousePosF) && !m_optionsOpen)
	{
		m_optionHover = true;
		m_optionSprite.setColor(sf::Color(106, 106, 106, 255));
	}
	else if (!m_optionSprite.getGlobalBounds().contains(m_mousePosF))
	{
		m_optionHover = false;
		m_optionSprite.setColor(sf::Color::White);
	}

	if (m_optionCloseText.getGlobalBounds().contains(m_mousePosF))
	{
		m_optionEndHover = true;
	}
	else if (!m_optionCloseText.getGlobalBounds().contains(m_mousePosF))
	{
		m_optionEndHover = false;
	}

	if (m_endSprite.getGlobalBounds().contains(m_mousePosF) && !m_optionsOpen)
	{
		m_endHover = true;
		m_endSprite.setColor(sf::Color(106, 106, 106, 255));
	}
	else if (!m_endSprite.getGlobalBounds().contains(m_mousePosF))
	{
		m_endHover = false;
		m_endSprite.setColor(sf::Color::White);
	}

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
		m_interactHover = true;
		m_interactPrompt.setPosition(m_location - m_interactOffset);
		m_interactE.setPosition(m_location - m_interactOffset);
	}
	else
	{
		m_interactHover = false;
	}
}


//BATTLE FUNCTIONS
void Game::failsafe() // MAKES SURE NO BUTTONS CAN BE CLICKED SOMEHOW WHEN GAME STATE CHANGES.
{
	m_startHover = false;
	m_optionHover = false;
	m_optionEndHover = false;
	m_endHover = false;
}

void Game::setupEnemy()
{
	if (!m_sansSpriteSheetTexture.loadFromFile("ASSETS//IMAGES//SansSpriteSheet.png"))
	{
		std::cout << "URUuruURUruURUruRUuUuru" << std::endl;
	}
	m_enemyPlaceholderSprite.setTexture(m_sansSpriteSheetTexture);
	m_sansInjuredHead.setTexture(m_sansSpriteSheetTexture);

	m_enemyPos = sf::Vector2f(screenWidth / 2, (screenHeight / 2) + 50);

	m_enemyPlaceholderSprite.setOrigin(170, 185);
	m_enemyPlaceholderSprite.setScale(3,3);
	m_enemyPlaceholderSprite.setTextureRect(sf::IntRect(0, 0, 348, 335));
	m_enemyPlaceholderSprite.setPosition(m_enemyPos);

	m_sansInjuredHead.setTextureRect(sf::IntRect(1008, 130, 1151, 271));
	m_sansInjuredHead.setScale(3, 3);
	m_sansInjuredHead.setPosition(m_enemyPos - sf::Vector2f(213, 400));


}

void Game::setupBattleMenu()
{
	if (!m_battleBackgroundTexture.loadFromFile("ASSETS//IMAGES//dune.png"))
	{
		std::cout << "bug with dune file" << std::endl;
	}

	m_battleBackgroundSprite.setTexture(m_battleBackgroundTexture);
	m_battleBackgroundSprite.setScale((screenWidth) / m_battleBackgroundSprite.getLocalBounds().width, (screenHeight) / m_battleBackgroundSprite.getLocalBounds().height);

	m_battleScreenRect.setPosition(0.0f,600.0f);
	m_battleScreenRect.setSize(sf::Vector2f(1920.0f, 400.0f));
	m_battleScreenRect.setFillColor(sf::Color::Black);

	m_subMenu.setPosition(1000.0f, 610.0f);
	m_subMenu.setSize(sf::Vector2f(700.0f, 375.0f));
	m_subMenu.setFillColor(sf::Color::Black);

	m_Fight.setPosition(1000.0f, 615.0f);
	m_Fight.setSize(sf::Vector2f(663.0f, 100.0f));
	m_Fight.setFillColor(sf::Color::Black);
	m_Fight.setOutlineColor(sf::Color(255, 165, 0, 255));
	m_Fight.setOutlineThickness(4);

	m_Magic.setPosition(1000.0f, 745.0f);
	m_Magic.setSize(sf::Vector2f(663.0f, 100.0f));
	m_Magic.setFillColor(sf::Color::Black);
	m_Magic.setOutlineColor(sf::Color(255, 165, 0, 255));
	m_Magic.setOutlineThickness(4);

	m_fightText.setFont(m_ArialBlackfont);
	m_fightText.setFillColor(sf::Color(255, 165, 0, 255));
	m_fightText.setString("FIGHT");
	m_fightText.setCharacterSize(51u);
	m_fightText.setOrigin(m_fightText.getLocalBounds().width / 2, m_fightText.getLocalBounds().height / 2);
	m_fightText.setPosition(m_Fight.getPosition().x + 331.5f, m_Fight.getPosition().y + 37);

	m_magicText.setFont(m_ArialBlackfont);
	m_magicText.setFillColor(sf::Color(255, 165, 0, 255));
	m_magicText.setString("MAGIC");
	m_magicText.setCharacterSize(51u);
	m_magicText.setOrigin(m_magicText.getLocalBounds().width / 2, m_magicText.getLocalBounds().height / 2);
	m_magicText.setPosition(m_Magic.getPosition().x + 331.5f, m_Magic.getPosition().y + 37);

	m_enemyName.setFont(m_ArialBlackfont);
	m_enemyName.setOrigin(m_enemyName.getLocalBounds().width / 2, m_enemyName.getLocalBounds().height / 2);
	m_enemyName.setPosition(m_battleScreenRect.getPosition());
	m_enemyName.setScale(2.0f,2.0f);

	m_enemyName.setString("PETER");
	m_enemyName.setFillColor(sf::Color::White);

	m_playerHealthText.setFont(m_ArialBlackfont);
	m_playerHealthText.setCharacterSize(50u);
	m_playerHealthText.setString("PLAYER HP: " + std::to_string(m_playerHealth));
	m_playerHealthText.setPosition(m_battleScreenRect.getPosition().x + 400, m_battleScreenRect.getPosition().y + 10);

	m_enemyHealthRectDamage.setFillColor(sf::Color::Red);
	m_enemyHealthRectDamage.setOutlineColor(sf::Color::Black);
	m_enemyHealthRectDamage.setOutlineThickness(5);
	m_enemyHealthRectDamage.setSize(sf::Vector2f(m_enemyHealth * 5, 70));
	m_enemyHealthRectDamage.setOrigin(m_enemyHealthRectDamage.getLocalBounds().width / 2, m_enemyHealthRectDamage.getLocalBounds().height / 2);
	m_enemyHealthRectDamage.setPosition(m_enemyPlaceholderSprite.getPosition().x, m_enemyPlaceholderSprite.getPosition().y - 375);

	m_enemyHealthRect.setFillColor(sf::Color::Green);
	m_enemyHealthRect.setOutlineColor(sf::Color::Black);
	m_enemyHealthRect.setOutlineThickness(5);
	m_enemyHealthRect.setSize(sf::Vector2f(m_enemyHealth * 5, 70));
	m_enemyHealthRect.setOrigin(m_enemyHealthRect.getLocalBounds().width / 2,m_enemyHealthRect.getLocalBounds().height / 2);
	m_enemyHealthRect.setPosition(m_enemyPlaceholderSprite.getPosition().x, m_enemyPlaceholderSprite.getPosition().y - 375);

	m_Fstab.setPosition(1000.0f, 615.0f);
	m_Fstab.setSize(sf::Vector2f(663.0f, 100.0f));
	m_Fstab.setFillColor(sf::Color::Black);
	m_Fstab.setOutlineThickness(4);

	m_FstabText.setFont(m_ArialBlackfont);
	m_FstabText.setString("STAB");
	m_FstabText.setFillColor(sf::Color(255, 165, 0));
	m_FstabText.setCharacterSize(51u);
	m_FstabText.setOrigin(m_FstabText.getLocalBounds().width / 2, m_FstabText.getLocalBounds().height / 2);
	m_FstabText.setPosition(m_Fstab.getPosition().x + 331.5f, m_Fstab.getPosition().y + 37);

	m_Fcrush.setPosition(1000.0f, 745.0f);
	m_Fcrush.setSize(sf::Vector2f(663.0f, 100.0f));
	m_Fcrush.setFillColor(sf::Color::Black);
	m_Fcrush.setOutlineThickness(4);

	m_FcrushText.setFont(m_ArialBlackfont);
	m_FcrushText.setString("CRUSH");
	m_FcrushText.setFillColor(sf::Color(255, 165, 0));
	m_FcrushText.setCharacterSize(51u);
	m_FcrushText.setOrigin(m_FcrushText.getLocalBounds().width / 2, m_FcrushText.getLocalBounds().height / 2);
	m_FcrushText.setPosition(m_Fcrush.getPosition().x + 331.5f, m_Fcrush.getPosition().y + 37);

	m_enemyAbilityText.setFont(m_ArialBlackfont);
	m_enemyAbilityText.setPosition(m_battleScreenRect.getPosition());
	m_enemyAbilityText.setCharacterSize(51u);
}

void Game::enemySelect()
{
	if (m_enemyNum == 0)
	{
		if (!enemySelected && !m_enemyDead)
		{
			m_blueValue = m_blueValue + 15;
			m_enemyPlaceholderSprite.setColor(sf::Color(255, 255, m_blueValue, 255));
		}
		else if (enemySelected)
		{
			m_enemyPlaceholderSprite.setColor(sf::Color::White);
		}
		else
		{
			m_enemyPlaceholderSprite.setColor(sf::Color::Red);
		}
	}

}

void Game::optionSelect()
{
	if (m_currentChoice < 0 )
	{
		m_currentChoice = 1;
	}
	else if (m_currentChoice > 1)
	{
		m_currentChoice = 0;
	}

	std::cout << m_currentChoice << std::endl;

	if (subMenuOpen)
	{
		switch (m_currentChoice)
		{
		case 0:
			m_subMenuChecker = fight;
			break;
		case 1:
			m_subMenuChecker = magic;
			break;
		}
	}
	else if (fightMenu)
	{
		switch (m_currentChoice)
		{
		case 0:
			m_fightMenuChecker = stab;
			break;
		case 1:
			m_fightMenuChecker = crush;
			break;
		}
	}
}


void Game::optionAnimate()
{
	if (m_subMenuChecker == fight)
	{
		m_Fight.setOutlineColor(sf::Color::Yellow);
		m_fightText.setFillColor(sf::Color::Yellow);
	}
	else
	{
		m_Fight.setOutlineColor(sf::Color(255, 165, 0));
		m_fightText.setFillColor(sf::Color(255, 165, 0));
	}

	if (m_subMenuChecker == magic)
	{
		m_Magic.setOutlineColor(sf::Color::Yellow);
		m_magicText.setFillColor(sf::Color::Yellow);
	}
	else
	{
		m_Magic.setOutlineColor(sf::Color(255, 165, 0));
		m_magicText.setFillColor(sf::Color(255, 165, 0));
	}


	if (m_fightMenuChecker == stab)
	{
		m_Fstab.setOutlineColor(sf::Color::Yellow);
		m_FstabText.setFillColor(sf::Color::Yellow);
	}
	else
	{
		m_Fstab.setOutlineColor(sf::Color(255, 165, 0));
		m_FstabText.setFillColor(sf::Color(255, 165, 0));
	}

	if (m_fightMenuChecker == crush)
	{
		m_Fcrush.setOutlineColor(sf::Color::Yellow);
		m_FcrushText.setFillColor(sf::Color::Yellow);
	}
	else
	{
		m_Fcrush.setOutlineColor(sf::Color(255, 165, 0));
		m_FcrushText.setFillColor(sf::Color(255, 165, 0));
	}
}

void Game::playerAttack()
{
	if (m_enemyHealth > 0)
	{
		m_enemyHealth = m_enemyHealth - 15;
	}
	else if (m_enemyHealth < 0)
	{
		m_enemyHealth = 0;
		enemySelected = false;
		m_enemyDead = true;
		m_subMenuChecker = NONE;
		m_fightMenuChecker = pacifist;
	}
	m_enemyHealthRect.setSize(sf::Vector2f(m_enemyHealth * 5, m_enemyHealthRect.getSize().y));

	enemyTurn();
}

void Game::enemyTurn()
{
	m_enemyAttacking = true;
	m_canAttack = false;
	enemyAttack();
}

void Game::enemyAttack()
{
	m_enemyAbilityText.setString("SANS USES SLASH!");
	m_playerHealth = m_playerHealth - 10;
	m_playerHealthText.setString("PLAYER HP: " + std::to_string(m_playerHealth));
	m_enemyAttackClock.restart();
}

bool Game::loadMusic()
{
	if (!m_menuMusic.openFromFile("ASSETS//AUDIO//MENUMUSIC.wav")
		|| !m_preBattleMusic.openFromFile("ASSETS//AUDIO//PREBATTLE.wav")
		|| !m_fightMusic.openFromFile("ASSETS//AUDIO//FIGHT.wav"));
	{
		return false;
	}

	m_menuMusic.setLoop(true);
	m_menuMusic.setVolume(75);

	m_preBattleMusic.setLoop(true);
	m_menuMusic.setVolume(75);

	m_fightMusic.setLoop(true);
	m_fightMusic.setVolume(75);

	return true;
}

void Game::playMusic()
{
	switch (currentState)
	{
		case menu:
			m_menuMusic.play();
			m_musicPlaying = true;
			break;
		case preBattle:
			m_menuMusic.stop();
			m_preBattleMusic.play();
			m_musicPlaying = true;
			break;
		case battle:
			m_fightMusic.play();
			m_preBattleMusic.stop();
			m_musicPlaying = true;
			break;
	}

}







