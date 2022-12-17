#pragma once
#include "GameEngine.h"
class GameOverScreen : public Scene
{
protected:

	std::string             m_levelPath;
	bool                    m_drawTextures = true;

	int						CurrentLevel = 1;

	std::vector<std::string>    m_levelPaths;

	void init(const std::string& levelPath);
	void loadLevel(const std::string& filename);

public:

	GameOverScreen(GameEngine* gameEngine, const std::string& levelPath);

	void update();
	void onEnd();
	void sRender();
	void sDoAction(const Action& action);
	Vec2 getPosition(int rx, int ry, int tx, int ty);
	bool IsInside(Vec2 pos, std::shared_ptr<Entity> e);

};

