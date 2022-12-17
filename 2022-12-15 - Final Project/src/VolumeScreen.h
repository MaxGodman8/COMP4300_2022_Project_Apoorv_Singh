#pragma once
#include "Scene.h"
class VolumeScreen : public Scene
{
protected:

	std::string					m_levelPath;
	std::vector<std::string>    m_levelPaths;
	
	std::vector<std::string>    m_menuStrings;
	sf::Text                    m_menuText;
	sf::Text                    VolumeText;

	void init(const std::string& levelPath);
	void loadLevel(const std::string& filename);

public:

	VolumeScreen(GameEngine* gameEngine, const std::string& levelPath);
	Vec2 getPosition(int rx, int ry, int tx, int ty);
	bool IsInside(Vec2 pos, std::shared_ptr<Entity> e);

	void update();
	void onEnd();
	void sRender();
	void sDoAction(const Action& action);

};