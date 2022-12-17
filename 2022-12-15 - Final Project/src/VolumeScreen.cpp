#include "VolumeScreen.h"
#include "GameEngine.h"
#include "Scene_Menu.h"


int MasterVolume;
VolumeScreen::VolumeScreen(GameEngine* gameEngine, const std::string& levelPath) : Scene(gameEngine), m_levelPath(levelPath)
{
	init(m_levelPath);

}


void VolumeScreen::init(const std::string& levelPath)
{
	m_menuStrings.push_back("Master Volume");
	

	loadLevel(levelPath);
	registerAction(sf::Keyboard::Escape, "QUIT");
	m_menuText.setFont(m_game->assets().getFont("Megaman"));
	m_menuText.setCharacterSize(25);

	VolumeText.setFont(m_game->assets().getFont("Megaman"));
	VolumeText.setCharacterSize(64);

	
}

void VolumeScreen::loadLevel(const std::string& filename)
{
	m_entityManager = EntityManager();

	std::ifstream stream(filename);
	std::string entityName, name;
	int rx, ry, px, py;

	while (stream.good()) {

		stream >> entityName;

		if (entityName == "Arrow") {

			stream >> name >> rx >> ry >> px >> py;

			std::shared_ptr<Entity> tile = m_entityManager.addEntity(entityName);

			std::shared_ptr<Entity> tile2 = m_entityManager.addEntity(entityName);

			auto& anim = m_game->assets().getAnimation(name);

			tile->addComponent<CAnimation>(anim, true);

			tile->addComponent<CTransform>(getPosition(rx, ry, px, py));

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;


			tile2->addComponent<CAnimation>(anim, true);

			tile2->addComponent<CTransform>(getPosition(rx, ry, px - 3, py));

			tile2->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;

			tile2->getComponent<CTransform>().angle = 180;

		}

		

	}

}

Vec2 VolumeScreen::getPosition(int rx, int ry, int tx, int ty)
{
	int X = (rx * 20 * 64) + (tx * 64) + 32;
	int Y = (ry * 12 * 64) + (ty * 64) + 32;

	return Vec2(X, Y);

}



void VolumeScreen::update()
{
	m_entityManager.update();
}

void VolumeScreen::onEnd()
{
	m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
}

void VolumeScreen::sRender()
{
	m_game->window().setView(m_game->window().getDefaultView());
	m_game->window().clear(sf::Color(0, 0, 0));

	VolumeText.setString("VOLUME");
	Vec2 pos1 = getPosition(0, 0, 7, 3);
	VolumeText.setPosition(pos1.x, pos1.y);
	m_game->window().draw(VolumeText);

	for (size_t i = 0; i < m_menuStrings.size(); i++)
	{
		m_menuText.setString(std::to_string(MasterVolume));
		m_menuText.setFillColor(sf::Color::White);
		
		Vec2 pos = getPosition(0, 0, 9, 5);

		m_menuText.setPosition(pos.x,pos.y);
		
		m_game->window().draw(m_menuText);
	}


	
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
	}
}

bool VolumeScreen::IsInside(Vec2 pos, std::shared_ptr<Entity> e)
{
	if (!e->hasComponent<CAnimation>()) 
	{
		return false;
	}

	auto& halfSize = e->getComponent<CAnimation>().animation.getSize() / 2;
	Vec2 delta = (e->getComponent<CTransform>().pos - pos).abs();
	return (delta.x <= halfSize.x) && (delta.y <= halfSize.y);
}


void VolumeScreen::sDoAction(const Action& action)
{

	Vec2 worldPos(action.pos().x, action.pos().y);

	if (action.type() == "START") {
		if (action.name() == "LEFT_CLICK") {

			for (auto e : m_entityManager.getEntities()) {

				if (IsInside(worldPos, e)) {

					CTransform& tran = e->getComponent<CTransform>();

					if (tran.angle == 0) {

						MasterVolume += 5;
						if (MasterVolume > 100) {
							MasterVolume = 100;
						}
					}

					if (tran.angle == 180) {

						MasterVolume -= 5;
						if (MasterVolume < 0) {
							MasterVolume = 0;
						}
					}
				}
			}
		}

		if (action.name() == "QUIT")
		{
			onEnd();
		}
	}
}
