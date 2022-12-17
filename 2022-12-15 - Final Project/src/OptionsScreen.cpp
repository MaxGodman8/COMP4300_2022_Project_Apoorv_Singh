#include "OptionsScreen.h"
#include "GameEngine.h"
#include "Scene_Menu.h"
#include "VolumeScreen.h"
#include "DifficultyScreen.h"

OptionsScreen::OptionsScreen(GameEngine* gameEngine) : Scene(gameEngine)
{
	init();

}

void OptionsScreen::init()
{
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::D, "ZELDA");
	registerAction(sf::Keyboard::Escape, "QUIT");

	m_menuStrings.push_back("Master Volume");
	m_menuStrings.push_back("Difficulty");

    m_levelPaths.push_back("volumescreen.txt");


	m_menuText.setFont(m_game->assets().getFont("Megaman"));
	m_menuText.setCharacterSize(64);
}

void OptionsScreen::update()
{
	m_entityManager.update();
}

void OptionsScreen::onEnd()
{
    m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
}

void OptionsScreen::sDoAction(const Action& action)
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
        else if (action.name() == "ZELDA")
        {
            if (m_selectedMenuIndex == 0) {

                m_game->changeScene("ZELDA", std::make_shared<VolumeScreen>(m_game, m_levelPaths[0]));
            }

            if (m_selectedMenuIndex == 1) {

                
                m_game->changeScene("MENU", std::make_shared<DifficultyScreen>(m_game));
            }


            m_game->assets().getSound("MusicTitle").stop();
        }

        else if (action.name() == "QUIT")
        {
            onEnd();
        }
    }

}



void OptionsScreen::sRender()
{
    m_game->window().setView(m_game->window().getDefaultView());
    m_game->window().clear(sf::Color(0, 0, 0));

    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color(100, 100, 100));
        m_menuText.setPosition(sf::Vector2f(10, 110 + i * 72));
        m_game->window().draw(m_menuText);
    }
}
