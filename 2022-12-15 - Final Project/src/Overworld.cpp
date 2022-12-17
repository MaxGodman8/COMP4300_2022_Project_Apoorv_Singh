#include "Overworld.h"
#include "GameEngine.h"
#include "Scene_Menu.h"

Overworld::Overworld(GameEngine* gameEngine, const std::string& levelPath): Scene(gameEngine), m_levelPath(levelPath)
{
	init(m_levelPath);
}

void Overworld::init(const std::string& levelPath)
{
	registerAction(sf::Keyboard::Space, "CHANGE");
	registerAction(sf::Keyboard::Enter, "SELECT");
	registerAction(sf::Keyboard::Escape, "QUIT");


	loadLevel(levelPath);

	m_levelPaths.push_back("level1.txt");
	m_levelPaths.push_back("level2.txt");
	m_levelPaths.push_back("level3.txt");
}

void Overworld::loadLevel(const std::string& filename)
{
	m_entityManager = EntityManager();

	std::ifstream stream(filename);
	std::string entityName, name;
	int rx, ry, px, py;

	while (stream.good()) {

		stream >> entityName;

		if (entityName == "Map") {

			stream >> name >> rx >> ry >> px >> py;

			std::shared_ptr<Entity> tile = m_entityManager.addEntity(entityName);

			auto& anim = m_game->assets().getAnimation(name);

			tile->addComponent<CAnimation>(anim, true);

			tile->addComponent<CTransform>(getPosition(rx, ry, px, py));

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;

		}

		if (entityName == "Castle") {

			stream >> name >> rx >> ry >> px >> py;

			std::shared_ptr<Entity> tile = m_entityManager.addEntity(entityName);

			auto& anim = m_game->assets().getAnimation(name);

			tile->addComponent<CAnimation>(anim, true);

			tile->addComponent<CBoundingBox>(anim.getSize(), 0, 0);

			tile->addComponent<CTransform>(getPosition(rx, ry, px, py));

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;

			tile->getComponent<CTransform>().animName = name;

		}

	}


}

void Overworld::SaveGameImplementation()
{
	fstream fileStream;
	fileStream.open("SavedGame.txt");

	if (fileStream.fail()) {

		std::ofstream file("SavedGame.txt");

		file << "1 0 1" << endl; //HighestLevel  Maxoffset CurrentLevel

		MaxSavedLevel = 1;
		MaxSavedOffsetX = 0;
		
		file.close();

		return;

	}

	

	std::ifstream stream("SavedGame.txt");

	std::string maxlevel, maxoffset, currentLevel;

	

	while (stream.good())
	{
		stream >> maxlevel >> maxoffset >>currentLevel;

		MaxSavedLevel = stoi(maxlevel);
		MaxSavedOffsetX = stoi(maxoffset);
		print(maxlevel);

	}

	stream.close();

}

void Overworld::update()
{
	m_entityManager.update();
}

void Overworld::onEnd()
{
	m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
}



void Overworld::sDoAction(const Action& action)
{
	if (action.type() == "START") {

		if (action.name() == "CHANGE") {

			UpdateLevelValue();

		}

		if (action.name() == "SELECT") {
			
			if (AllowSelection) 
			{
				LevelSelection();
			}
			
		}
	}
	else if (action.name() == "QUIT")
	{
		onEnd();
	}

	if (action.type() == "END") {

		if (action.name() == "CHANGE") {

			AllowLevelChange = true;

		}

		if (action.name() == "SELECT") {

			AllowSelection = true;
		}
	}
}

void Overworld::LevelSelection()
{
	AllowSelection = false;

	SaveGameImplementation();

	if (SelectedLevel > MaxSavedLevel)return;

	std::string finalWrite = std::to_string(MaxSavedLevel) + " " + std::to_string(MaxSavedOffsetX) +
		" " + std::to_string(SelectedLevel);

	std::ofstream file("SavedGame.txt");

	file << finalWrite << endl;

	m_game->changeScene("SELECT", std::make_shared<Scene_Zelda>(m_game, m_levelPaths[SelectedLevel - 1]));
}

void Overworld::UpdateLevelValue()
{
	if (!AllowLevelChange)return;

	SelectedLevel += 1;

	if (SelectedLevel > 3) { SelectedLevel = 1; }

	AllowLevelChange = false;
	
}

void Overworld::print(std::string value)
{
	std::cout << value << endl;
}

Vec2 Overworld::getPosition(int rx, int ry, int tx, int ty)
{
	int X = (rx * 20 * 64) + (tx * 64) + 32;
	int Y = (ry * 12 * 64) + (ty * 64) + 32;



	return Vec2(X, Y);

}

void Overworld::BorderRender(std::shared_ptr<Entity> e)
{
	auto& box = e->getComponent<CBoundingBox>();
	auto& transform = e->getComponent<CTransform>();
	sf::RectangleShape rect;
	rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
	rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
	rect.setPosition(transform.pos.x, transform.pos.y);
	rect.setFillColor(sf::Color(0, 0, 0, 0));

	rect.setOutlineColor(sf::Color::Yellow);
	rect.setOutlineThickness(3);
	m_game->window().draw(rect);
}

void Overworld::sRender()
{
	m_game->window().clear(sf::Color(0, 0, 0));
	sf::RectangleShape tick({ 1.0f, 6.0f });
	tick.setFillColor(sf::Color::Black);

	for (auto e : m_entityManager.getEntities()) {
		auto& transform = e->getComponent<CTransform>();
		sf::Color c = sf::Color::White;
		
		if (e->hasComponent<CAnimation>()) {

			auto& animation = e->getComponent<CAnimation>().animation;
			animation.getSprite().setRotation(transform.angle);
			animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
			animation.getSprite().setScale(transform.scale.x, transform.scale.y);
			animation.getSprite().setColor(c);
			m_game->window().draw(animation.getSprite());

		}

		if (e->hasComponent<CBoundingBox>()) {

			if (SelectedLevel == 1 && transform.animName == "castle1") {
				BorderRender(e);
			}

			if (SelectedLevel == 2 && transform.animName == "castle2") {
				BorderRender(e);
			}

			if (SelectedLevel == 3 && transform.animName == "castle3") {
				BorderRender(e);
			}

		}

	}
}
