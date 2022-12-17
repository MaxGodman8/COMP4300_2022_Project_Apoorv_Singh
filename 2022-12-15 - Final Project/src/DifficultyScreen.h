#pragma once
#include "Common.h"
#include "Scene.h"
#include <map>
#include <memory>
#include <deque>

#include "EntityManager.h"
class DifficultyScreen : public Scene
{
protected:
	
	std::vector<std::string>    m_menuStrings;
	sf::Text                    m_menuText;
	
	std::vector<std::string>    m_levelPaths;

	int                      m_selectedMenuIndex = 0;

	void init();
	void update();
	void onEnd();
	void sDoAction(const Action& action);

public:
	
	DifficultyScreen(GameEngine* gameEngine = nullptr);
	void sRender();

};

