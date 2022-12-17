#include "LevelPassed.h"
#include "Overworld.h"


LevelPassed::LevelPassed(GameEngine* gameEngine, const std::string& levelPath) : Scene(gameEngine), m_levelPath(levelPath)
{
	init(m_levelPath);
}

void LevelPassed::init(const std::string& levelPath)
{
	loadLevel(levelPath);
	m_levelPaths.push_back("overworld.txt");
}

void LevelPassed::loadLevel(const std::string& filename)
{
	m_entityManager = EntityManager();

	std::ifstream stream(filename);
	std::string entityName, name;
	int rx, ry, px, py;


	while (stream.good())
	{
		stream >> entityName;

		if (entityName == "Passed") {

			stream >> name >> rx >> ry >> px >> py;

			std::shared_ptr<Entity> tile = m_entityManager.addEntity(entityName);

			auto& anim = m_game->assets().getAnimation(name);

			tile->addComponent<CAnimation>(anim, true);
			tile->addComponent<CTransform>(getPosition(rx, ry, px, py));
			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;
			print("coming in");

		}

		if (entityName == "Next") {

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





void LevelPassed::update()
{
	m_entityManager.update();
}

bool LevelPassed::IsInside(Vec2 pos, std::shared_ptr<Entity> e)
{
	if (!e->hasComponent<CAnimation>()) {
		return false;

	}

	auto& halfSize = e->getComponent<CAnimation>().animation.getSize() / 2;
	Vec2 delta = (e->getComponent<CTransform>().pos - pos).abs();
	return (delta.x <= halfSize.x) && (delta.y <= halfSize.y);
}

void LevelPassed::onEnd()
{
}

void LevelPassed::sRender()
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
			print("rendering");

		}
	}
}

void LevelPassed::sDoAction(const Action& action)
{
	Vec2 worldPos(action.pos().x, action.pos().y);
	if (action.name() == "LEFT_CLICK") 
	{
		for (auto e : m_entityManager.getEntities()) 
		{
			if (IsInside(worldPos, e)) 
			{
				CTransform& tran = e->getComponent<CTransform>();
				if (tran.animName == "next") 
				{
					m_game->changeScene("OVERWORLD", std::make_shared<Overworld>(m_game, m_levelPaths[0]));

				}

			}

		}

	}

}

void LevelPassed::print(std::string value)
{
	std::cout << value << "\n";
}

Vec2 LevelPassed::getPosition(int rx, int ry, int tx, int ty)
{
	int X = (rx * 20 * 64) + (tx * 64) + 32;
	int Y = (ry * 12 * 64) + (ty * 64) + 32;

	return Vec2(X, Y);
}


