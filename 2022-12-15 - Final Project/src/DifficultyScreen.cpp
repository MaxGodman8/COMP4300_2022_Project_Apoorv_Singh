#include "DifficultyScreen.h"
#include "GameEngine.h"
#include "Scene_Menu.h"

int Difficulty;
DifficultyScreen::DifficultyScreen(GameEngine* gameEngine) : Scene(gameEngine)
{
	init();
}


void DifficultyScreen::init()
{
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::D, "ZELDA");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_menuStrings.push_back("Normal");
	m_menuStrings.push_back("Hard");

	m_menuText.setFont(m_game->assets().getFont("Megaman"));
	m_menuText.setCharacterSize(64);

	m_selectedMenuIndex = Difficulty;
}

void DifficultyScreen::update()
{
	m_entityManager.update();
}

void DifficultyScreen::onEnd()
{
	m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
}

void DifficultyScreen::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "UP")
        {
            if (m_selectedMenuIndex > 0) { m_selectedMenuIndex--; }
            else { m_selectedMenuIndex = m_menuStrings.size() - 1; }
        }
        else if (action.name() == "DOWN")
        {
            m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
        }
        
        else if (action.name() == "QUIT")
        {
            onEnd();
        }

        Difficulty = m_selectedMenuIndex;
    }

}



void DifficultyScreen::sRender()
{
    m_game->window().setView(m_game->window().getDefaultView());
    m_game->window().clear(sf::Color(0, 0, 0));

    for (int i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color(100, 100, 100));
        m_menuText.setPosition(sf::Vector2f(10, 110 + i * 72));
        m_game->window().draw(m_menuText);
    }
}
