#pragma once
#include "GameEngine.h"
class LevelPassed : public Scene
{
protected:


	std::string             m_levelPath;
	bool                    m_drawTextures = true;
	
	std::vector<std::string>    m_levelPaths;
	void init(const std::string& levelPath);
	void loadLevel(const std::string& filename);

public:
	
	LevelPassed(GameEngine* gameEngine, const std::string& levelPath);
	
	Vec2 getPosition(int rx, int ry, int tx, int ty);
	bool IsInside(Vec2 pos, std::shared_ptr<Entity> e);

	void update();
	void onEnd();
	void sRender();
	void sDoAction(const Action& action);

	void print(std::string value);
};

