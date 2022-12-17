#pragma once
#include "GameEngine.h"
#include "Scene_Zelda.h"
class Overworld : public Scene
{

protected:
	
	std::string             m_levelPath;
	bool                    m_drawTextures = true;
	int						SelectedLevel = 1;
	int						MaxSavedLevel = 1;
	int						MaxSavedOffsetX = 0;
	bool					AllowLevelChange = true;
	bool					AllowSelection = true;

	std::vector<std::string>    m_levelPaths;


	void init(const std::string& levelPath);
	void loadLevel(const std::string& filename);


public:

	Overworld(GameEngine* gameEngine, const std::string& levelPath);


	void update();
	void onEnd();
	void sRender();
	void sDoAction(const Action& action);
	void BorderRender(std::shared_ptr<Entity> e);

	void print(std::string value);
	void UpdateLevelValue();

	Vec2 getPosition(int rx, int ry, int tx, int ty);

	void SaveGameImplementation();
	void LevelSelection();

};

