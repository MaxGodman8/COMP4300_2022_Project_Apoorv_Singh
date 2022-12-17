#include "LevelEditor.h"
#include "GameEngine.h"
#include <string> 
#include "Scene_Menu.h"


LevelEditor::LevelEditor(GameEngine* gameEngine, const std::string& levelPath) :Scene(gameEngine), m_levelPath(levelPath)
{

	init(m_levelPath);

}

void LevelEditor::init(const std::string& levelPath) {

	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::E, "TOOLS");
	registerAction(sf::Keyboard::Q, "DESELECT");
	registerAction(sf::Keyboard::Enter, "SAVE");
	registerAction(sf::Keyboard::Escape, "QUIT");


	m_mouseShape.setRadius(8);
	m_mouseShape.setOrigin(8, 8);
	m_mouseShape.setPointCount(32);
	m_mouseShape.setFillColor(sf::Color(0, 255, 0, 185));

	ArrowText.setFont(m_game->assets().getFont("Megaman"));
	ArrowText.setCharacterSize(16);

	TitleText.setFont(m_game->assets().getFont("Megaman"));
	TitleText.setCharacterSize(18);

	TitleText2.setFont(m_game->assets().getFont("Megaman"));
	TitleText2.setCharacterSize(18);
	
	ArrowText2.setFont(m_game->assets().getFont("Megaman"));
	ArrowText2.setCharacterSize(16);

	loadLevel(levelPath);



}

void LevelEditor::loadLevel(const std::string& filename) {

	m_entityManager = EntityManager();

	FileName = filename;

	Fake = m_entityManager.addEntity("fake");
}

void LevelEditor::InstantiateToolsObjects() {

	std::ifstream stream(FileName);
	std::string entityName, name, aibehaviour;
	int rx, ry, px, py, movement, vision, health, damage, speed, numOfPoints, point, rotatingSpeed;
	int tileSpeed = 2;
	int maxDistance = 3;

	while (stream.good())
	{

		stream >> entityName;

		if (entityName == Tile) {

			stream >> name >> rx >> ry >> px >> py >> movement >> vision;

			bool move = false;
			bool vis = false;

			if (movement == 1) { move = true; }
			if (vision == 1) { vis = true; }

			auto tile = BaseTileInitialization(Tile, name, rx, ry, px, py, move, vis);

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;

		}

		if (entityName == MovingTile) {

			stream >> name >> rx >> ry >> px >> py >> movement >> vision >> maxDistance >> tileSpeed;

			bool move = false;
			bool vis = false;

			if (movement == 1) { move = true; }
			if (vision == 1) { vis = true; }

			auto tile = BaseTileInitialization(MovingTile, name, rx, ry, px, py, move, vis);

			tile->addComponent<CPingPongMovement>(maxDistance * 64, 2);

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;


			CPingPongMovement& ping = tile->getComponent<CPingPongMovement>();

			ping.centerPos = tile->getComponent<CTransform>().pos;

		}

		if (entityName == Blade) {

			stream >> name >> rx >> ry >> px >> py >> movement >> vision;
			bool move = false;
			bool vis = false;

			if (movement == 1) { move = true; }
			if (vision == 1) { vis = true; }

			auto tile = BaseTileInitialization(Blade, name, rx, ry, px, py, move, vis);

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;

			tile->addComponent<CRotator>();

		}

		if (entityName == Collectible) {

			stream >> name >> rx >> ry >> px >> py >> movement >> vision;
			bool move = false;
			bool vis = false;

			if (movement == 1) { move = true; }
			if (vision == 1) { vis = true; }

			auto tile = BaseTileInitialization(Collectible, name, rx, ry, px, py, move, vis);

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;



		}


		if (entityName == MovementOff || entityName == MovementOn || entityName == VisionOff || entityName == VisionOn
			|| entityName == Save || entityName == Symbol) {

			stream >> name >> rx >> ry >> px >> py >> movement >> vision;

			bool move = false;
			bool vis = false;

			if (movement == 1) { move = true; }
			if (vision == 1) { vis = true; }



			if (entityName == MovementOff) {
				auto tile = BaseTileInitialization(MovementOff, name, rx, ry, px, py, move, vis, false);
				tile->addComponent<CButton>(MovementOff);
				EnableButton(tile, false);
			}
			if (entityName == MovementOn) {
				auto tile = BaseTileInitialization(MovementOn, name, rx, ry, px, py, move, vis, false);
				tile->addComponent<CButton>(MovementOn);
				EnableButton(tile, false);
			}
			if (entityName == VisionOff) {
				auto tile = BaseTileInitialization(VisionOff, name, rx, ry, px, py, move, vis, false);
				tile->addComponent<CButton>(VisionOff);
				EnableButton(tile, false);
			}
			if (entityName == VisionOn) {
				auto tile = BaseTileInitialization(VisionOn, name, rx, ry, px, py, move, vis, false);
				tile->addComponent<CButton>(VisionOn);
				EnableButton(tile, false);
			}

			if (entityName == Save) {
				//auto tile = BaseTileInitialization(Save, name, rx, ry, px, py, move, vis, false);
				//tile->addComponent<CButton>(Save);
			}

			if (entityName == Symbol) {

				auto tile = BaseTileInitialization(Symbol, name, rx, ry, px, py, move, vis, false);
				tile->addComponent<CButton>(Symbol);

				auto tile2 = BaseTileInitialization(Symbol, name, rx, ry, px - 2, py, move, vis, false);
				tile2->addComponent<CButton>(Symbol);

				auto tile3 = BaseTileInitialization(Symbol, name, rx, ry, px + 7, py, move, vis, false);
				tile3->addComponent<CButton>(Symbol);

				auto tile4 = BaseTileInitialization(Symbol, name, rx, ry, px + 5, py, move, vis, false);
				tile4->addComponent<CButton>(Symbol);

				EnableButton(tile, false);
				EnableButton(tile2, false);
				
				EnableButton(tile3, false);
				EnableButton(tile4, false);

			}

		}


		

		

		if (entityName == Robot) {

			stream >> name >> rx >> ry >> px >> py >> movement >> vision;
			bool move = false;
			bool vis = false;

			if (movement == 1) { move = true; }
			if (vision == 1) { vis = true; }

			auto tile = BaseTileInitialization(Robot, name, rx, ry, px, py, move, vis);

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;

			tile->addComponent<CPatrol>();

		}


		if (entityName == Following) {

			stream >> name >> rx >> ry >> px >> py >> movement >> vision;
			bool move = false;
			bool vis = false;

			if (movement == 1) { move = true; }
			if (vision == 1) { vis = true; }

			auto tile = BaseTileInitialization(Following, name, rx, ry, px, py, move, vis);

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;

			tile->addComponent<CFollowPlayer>();

		}

		if (entityName == BackgroundTile) {

			stream >> name >> rx >> ry >> px >> py >> movement >> vision;
			bool move = false;
			bool vis = false;

			if (movement == 1) { move = true; }
			if (vision == 1) { vis = true; }

			std::shared_ptr<Entity> tile = m_entityManager.addEntity(entityName);

			auto& anim = m_game->assets().getAnimation(name);

			tile->addComponent<CAnimation>(anim, true);

			tile->addComponent<CTransform>(getPosition(rx, ry, px, py));

			tile->getComponent<CTransform>().animName = name;

			tile->addComponent<CDraggable>();

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;

		}

		if (entityName == Acid) {

			stream >> name >> rx >> ry >> px >> py >> movement >> vision;
			bool move = false;
			bool vis = false;

			if (movement == 1) { move = true; }
			if (vision == 1) { vis = true; }

			auto tile = BaseTileInitialization(Acid, name, rx, ry, px, py, move, vis);

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;

		}

		if (entityName == Spike) {

			stream >> name >> rx >> ry >> px >> py >> movement >> vision;
			bool move = false;
			bool vis = false;

			if (movement == 1) { move = true; }
			if (vision == 1) { vis = true; }

			auto tile = BaseTileInitialization(Spike, name, rx, ry, px, py, move, vis);

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;

		}

		if (entityName == BoxName) {

			stream >> name >> rx >> ry >> px >> py >> movement >> vision;
			bool move = false;
			bool vis = false;

			if (movement == 1) { move = true; }
			if (vision == 1) { vis = true; }

			auto tile = BaseTileInitialization(BoxName, name, rx, ry, px, py, move, vis);

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;

		}



		if (entityName == FlagE) {

			stream >> name >> rx >> ry >> px >> py >> movement >> vision;
			bool move = false;
			bool vis = false;

			if (movement == 1) { move = true; }
			if (vision == 1) { vis = true; }

			auto tile = BaseTileInitialization(FlagE, name, rx, ry, px, py, move, vis);

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;

		}

		if (entityName == BoatE) {

			stream >> name >> rx >> ry >> px >> py >> movement >> vision;
			bool move = false;
			bool vis = false;

			if (movement == 1) { move = true; }
			if (vision == 1) { vis = true; }

			auto tile = BaseTileInitialization(BoatE, name, rx, ry, px, py, move, vis);

			tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;

		}

	}

}

void LevelEditor::DeselectAll()
{
	auto entities = m_entityManager.getEntities();

	for (auto e : entities) {

		if (e->hasComponent<CSelectable>()) {

			CSelectable& sel = e->getComponent<CSelectable>();

			sel.Selected = false;

		}

	}

}

std::shared_ptr<Entity> LevelEditor::BaseTileInitialization(std::string entityName, std::string name, int rx, int ry, int px,
	int py, bool move, bool vis, bool allowdrag)
{
	std::shared_ptr<Entity> tile = m_entityManager.addEntity(entityName);

	auto& anim = m_game->assets().getAnimation(name);

	tile->addComponent<CAnimation>(anim, true);

	tile->addComponent<CTransform>(getPosition(rx, ry, px, py));

	tile->addComponent<CBoundingBox>(anim.getSize(), move, vis);

	CTransform& tra = tile->getComponent<CTransform>();
	tra.animName = name;
	tra.tilePos = Vec2(px, py);
	tra.roomPos = Vec2(rx, ry);



	if (allowdrag) {
		tile->addComponent<CDraggable>();
	}


	std::cout << "base tile" << "\n";

	return tile;
}

void LevelEditor::update()
{
	m_entityManager.update();

	PositionUpdate();
	ToolsScreenControl();
	sCamera();

}

void LevelEditor::onEnd()
{
	m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
}

void LevelEditor::sCamera() {

	if ((RightPressed && RightAllowChange) ||(LeftPressed && LeftAllowChange)) {

		sf::View view = m_game->window().getView();

		print(std::to_string(offsetX));
		


		int centerPixelX = (offsetX * 20 * 64) + ((20 * 64) / 2);
		int centerPixelY = ((12 * 64) / 2);

		view.setCenter(centerPixelX, centerPixelY);

		m_game->window().setView(view);

		if(RightPressed){ RightAllowChange = false; }
		if (LeftPressed) { LeftAllowChange = false; }
		
	}

	

}

bool LevelEditor::IsInside(Vec2 pos, std::shared_ptr<Entity> e)
{
	if (!e->hasComponent<CAnimation>()) {
		return false;

	}

	auto& halfSize = e->getComponent<CAnimation>().animation.getSize() / 2;

	Vec2 delta = (e->getComponent<CTransform>().pos - pos).abs();

	return (delta.x <= halfSize.x) && (delta.y <= halfSize.y);

}

void LevelEditor::InstantiateEntity(std::shared_ptr<Entity> e)
{
	std::string entityName = e->tag();

	CAnimation& anim = e->getComponent<CAnimation>();
	CBoundingBox& box = e->getComponent<CBoundingBox>();
	CTransform& trans = e->getComponent<CTransform>();

	std::string name = anim.animation.getName();

	bool move = box.blockMove;
	bool vis = box.blockVision;

	if (m_mouseShape.getPosition().y > 64) {
		
		auto tile = BaseInstantiation(entityName, name, Vec2(m_mouseShape.getPosition().x, m_mouseShape.getPosition().y), move, vis);

		LevelEntities.push_back(tile);

	}

}

std::shared_ptr<Entity> LevelEditor::BaseInstantiation(std::string entityName, std::string name, Vec2 pos, bool move, bool vis)
{

	Vec2 tilePos = GetTilePosition(pos);

	int tx = tilePos.x;
	int ty = tilePos.y;

	Vec2 finalpos = getPosition(offsetX, 0, tx, ty);

	std::shared_ptr<Entity> tile = m_entityManager.addEntity(entityName);

	auto& anim = m_game->assets().getAnimation(name);

	tile->addComponent<CAnimation>(anim, true);

	tile->addComponent<CTransform>(finalpos);

	tile->addComponent<CBoundingBox>(anim.getSize(), move, vis);

	tile->addComponent<CSelectable>();

	if (entityName == Robot) {
		tile->addComponent<CPatrol>();
	}

	if (entityName == Following) {
		tile->addComponent<CFollowPlayer>();
	}

	CTransform& tTRans = tile->getComponent<CTransform>();
	tTRans.animName = name;
	tTRans.tilePos = tilePos;
	tTRans.roomPos = Vec2(offsetX, 0);

	return tile;

}

void LevelEditor::PositionUpdate()
{
	if (RightPressed && RightAllowChange) { offsetX += 1; }
	if (LeftPressed && LeftAllowChange) { offsetX -= 1; }

	for (auto e : m_entityManager.getEntities()) {

		CTransform& transform = e->getComponent<CTransform>();

		if (e->hasComponent<CDraggable>()) {

			transform.pos.x = transform.iniPos.x + (offsetX * 20 * 64);
			transform.pos.y = transform.iniPos.y;

		}


		if (e->hasComponent<CDraggable>() && e->getComponent<CDraggable>().Dragging) {

			transform.pos = Vec2(m_mouseShape.getPosition().x, m_mouseShape.getPosition().y);

		}

	}


}


void LevelEditor::OnSelected(std::shared_ptr<Entity> e)
{
	std::string tag = e->tag();


	if (tag == Tile || tag == Collectible || tag == Blade || tag == Robot || tag == Following ||
		tag == MovingTile || tag == BackgroundTile || tag == Spike || tag == Acid) {

		CBoundingBox& box = e->getComponent<CBoundingBox>();
		bool movement = box.blockMove;
		bool vision = box.blockVision;

		if (movement) {

			MovementVisionButtonEnabler(MovementOn, MovementOff, false, true, e);

		}
		else {
			MovementVisionButtonEnabler(MovementOn, MovementOff, true, false, e);
		}

		if (vision) {

			MovementVisionButtonEnabler(VisionOn, VisionOff, false, true, e);

		}
		else {
			MovementVisionButtonEnabler(VisionOn, VisionOff, true, false, e);
		}

	}

	if (tag == Collectible || tag == Blade || tag == MovingTile) {
		
		auto arrows = m_entityManager.getEntities(Symbol);
		
		auto rightArrow = arrows.at(0);
		auto leftArrow = arrows.at(1);

		leftArrow->getComponent<CTransform>().angle = 180;

		rightArrow->getComponent<CButton>().EntityName = e->tag();
		rightArrow->getComponent<CButton>().OriginalEntityPos = e->getComponent<CTransform>().pos;
		
		leftArrow->getComponent<CButton>().EntityName = e->tag();
		leftArrow->getComponent<CButton>().OriginalEntityPos = e->getComponent<CTransform>().pos;

		leftArrow->getComponent<CButton>().LeftArrow = true;
		rightArrow->getComponent<CButton>().RightArrow = true;

		EnableButton(leftArrow, true);
		EnableButton(rightArrow, true);
		
	}

	if (tag == Robot || tag == Following) {

		auto arrows = m_entityManager.getEntities(Symbol);

		auto rightArrow = arrows.at(0);
		auto leftArrow = arrows.at(1);

		auto rightArrow2 = arrows.at(2);
		auto leftArrow2 = arrows.at(3);


		leftArrow->getComponent<CTransform>().angle = 180;
		leftArrow2->getComponent<CTransform>().angle = 180;

		rightArrow->getComponent<CButton>().EntityName = e->tag();
		rightArrow->getComponent<CButton>().OriginalEntityPos = e->getComponent<CTransform>().pos;
		rightArrow2->getComponent<CButton>().EntityName = e->tag();
		rightArrow2->getComponent<CButton>().OriginalEntityPos = e->getComponent<CTransform>().pos;

		leftArrow->getComponent<CButton>().EntityName = e->tag();
		leftArrow->getComponent<CButton>().OriginalEntityPos = e->getComponent<CTransform>().pos;
		leftArrow2->getComponent<CButton>().EntityName = e->tag();
		leftArrow2->getComponent<CButton>().OriginalEntityPos = e->getComponent<CTransform>().pos;

		leftArrow->getComponent<CButton>().LeftArrow = true;
		rightArrow->getComponent<CButton>().RightArrow = true;
		leftArrow2->getComponent<CButton>().LeftArrow2 = true;
		rightArrow2->getComponent<CButton>().RightArrow2 = true;

		EnableButton(leftArrow, true);
		EnableButton(rightArrow, true);
		EnableButton(leftArrow2, true);
		EnableButton(rightArrow2, true);

	}


}



void LevelEditor::MovementVisionButtonEnabler(std::string on, std::string off, bool onVal, 
	bool offVal, std::shared_ptr<Entity> originalEntity)
{
	auto entities = m_entityManager.getEntities(on);
	auto offentities = m_entityManager.getEntities(off);
	auto onButton = entities.at(0);
	auto offButton = offentities.at(0);
	EnableButton(onButton, onVal);
	EnableButton(offButton, offVal);

	if (onVal) {
		onButton->getComponent<CButton>().EntityName = originalEntity->tag();
		onButton->getComponent<CButton>().OriginalEntityPos = originalEntity->getComponent<CTransform>().pos;
	}
	if (offVal) {
		offButton->getComponent<CButton>().EntityName = originalEntity->tag();
		offButton->getComponent<CButton>().OriginalEntityPos = originalEntity->getComponent<CTransform>().pos;
	}


}

void LevelEditor::EnableButton(std::shared_ptr<Entity> button, bool enable)
{
	if (!button->hasComponent<CButton>())return;

	if (enable) {

		auto& anim = m_game->assets().getAnimation(button->getComponent<CTransform>().animName);

		button->addComponent<CAnimation>(anim, true);

		CTransform& bTrans = button->getComponent<CTransform>();

		bTrans.pos = getPosition(offsetX, 0, bTrans.tilePos.x, bTrans.tilePos.y);

		std::cout << "enable entity" << "\n";

		return;
	}

	button->removeComponent<CAnimation>();
	

}

std::shared_ptr<Entity> LevelEditor::GetButtonEntity(std::shared_ptr<Entity> thisbutton)
{
	CButton& button = thisbutton->getComponent<CButton>();

	auto originalEntityName = button.EntityName;
	Vec2 originalEntityPos = button.OriginalEntityPos;

	

	auto tiles = m_entityManager.getEntities(originalEntityName);

	for (auto thistile : tiles) {

		CTransform& tileTransform = thistile->getComponent<CTransform>();

		if (tileTransform.pos == originalEntityPos) {

			return thistile;
			
		}

	}

	return Fake;

}

void LevelEditor::SetButtonValueToEntity(std::shared_ptr<Entity> buttonEntity, std::string name) {

	CBoundingBox& box = buttonEntity->getComponent<CBoundingBox>();

	if (name == MovementOn) {
		box.blockMove = true; return;
	}

	if (name == MovementOff) {
		box.blockMove = false; return;
	}

	if (name == VisionOn) {
		box.blockVision = true; return;
	}

	if (name == VisionOff) {
		box.blockVision = false; return;
	}

}


void LevelEditor::sDoAction(const Action& action)
{
	Vec2 worldPos(action.pos().x + (offsetX * 20 * 64), action.pos().y);

	if (action.type() == "START") {

		if (action.name() == "LEFT") {

			LeftPressed = true;
		}

		if (action.name() == "TOOLS") { m_drawToolsScreen = !m_drawToolsScreen; }

		if (action.name() == "DESELECT") { DeselectAll(); }

		else if (action.name() == "RIGHT") {

			RightPressed = true;

		}

		else if (action.name() == "SAVE") { EditorToFile(); }
		
		if (action.name() == "QUIT")
		{
			onEnd();
		}
	}
		
	else if (action.name() == "RIGHT_CLICK") {

			for (auto e : m_entityManager.getEntities()) {

				if (e->hasComponent<CDraggable>()) {

					CDraggable& drag = e->getComponent<CDraggable>();

					if (drag.Dragging) {

						//drag.Dragging = false;

						//CTransform& transform = e->getComponent<CTransform>();

						//transform.pos.x = transform.iniPos.x + (offsetX * 20 * 64);
						//transform.pos.y = transform.iniPos.y;

						e->destroy();

					}


				}
				else {

					if (IsInside(worldPos, e) && !e->hasComponent<CDraggable>()) {

						if (e->hasComponent<CSelectable>()) {

							CSelectable& selectable = e->getComponent<CSelectable>();
							
							if (!selectable.Selected) {
								LevelEntities.erase(std::remove(LevelEntities.begin(), LevelEntities.end(), e), LevelEntities.end());
								e->destroy();
							}

						}


					}

				}

			}

		}
		
		
	else if (action.name() == "LEFT_CLICK") {


		for (auto e : m_entityManager.getEntities()) {

			if (IsInside(worldPos, e)) {

				if (e->hasComponent<CDraggable>()) {
					
					m_drawToolsScreen = false;
					CDraggable& drag = e->getComponent<CDraggable>();

					if (drag.Dragging) {

						InstantiateEntity(e);
						return;
					}

					drag.Dragging = !drag.Dragging;

				}

				if (e->hasComponent<CSelectable>()) {

					CSelectable& selectable = e->getComponent<CSelectable>();
					selectable.Selected = true;
					OnSelected(e);

				}

				if (e->hasComponent<CButton>()) {

					CButton& button = e->getComponent<CButton>();

					if (button.buttonName == Save) {

						EditorToFile();
					}
					else {

						auto buttonEntity = GetButtonEntity(e);

						std::string name = button.buttonName;

						SetButtonValueToEntity(buttonEntity, name);
						OnSelected(buttonEntity);

						if (buttonEntity->hasComponent<CSelectable>()) {
							CSelectable& entitySelectable = buttonEntity->getComponent<CSelectable>();
							entitySelectable.Selected = true;

							if (button.LeftArrow) {
								entitySelectable.Amount -= 5;
							}
							if (button.RightArrow) {
								entitySelectable.Amount += 5;
							}
							if (button.LeftArrow2) {
								entitySelectable.Amount2 -= 5;
							}
							if (button.RightArrow2) {
								entitySelectable.Amount2 += 5;
							}


							if (buttonEntity->hasComponent<CRotator>()) {
								
								CRotator& rotator = buttonEntity->getComponent<CRotator>();

								rotator.rotatingSpeed = entitySelectable.Amount;

							}

							return;
						}
					}

					

				}


			}
			else {
				if (e->hasComponent<CSelectable>() && !e->hasComponent<CPatrol>()) {

					CSelectable& selectable = e->getComponent<CSelectable>();
					selectable.Selected = false;

				}

				if (e->hasComponent<CPatrol>()) {

					if (e->hasComponent<CSelectable>()) {
						
						CSelectable& selectable = e->getComponent<CSelectable>();
						if (selectable.Selected) 
						{
							CPatrol& patrol = e->getComponent<CPatrol>();
							print("in patrol");
							patrol.positions.push_back(Vec2(m_mouseShape.getPosition().x, m_mouseShape.getPosition().y));
						}
					
					}

					
				}

			}


		}


	}
	

	if (action.type() == "END") {

		if (action.name() == "LEFT") {

			LeftPressed = false;
			LeftAllowChange = true;
		}

		else if (action.name() == "RIGHT") {

			RightPressed = false;
			RightAllowChange = true;
		}

		else if (action.name() == "SAVE") { AllowSave = true; }

	}

	if (action.name() == "MOUSE_MOVE") {

		m_mouseShape.setPosition(action.pos().x + (offsetX * 20 * 64), action.pos().y);

	}


}

void LevelEditor::EditorToFile()
{
	std::ofstream file("test.txt");

	for (auto e : LevelEntities) {

		if (e->tag() == Tile || e->tag() == BackgroundTile || e->tag() == Acid ||
			e->tag() == Spike || e->tag() == FloorButton || e->tag() == Gate || e->tag() == BoxName
			|| e->tag() == FlagE || e->tag() == SensorE || e->tag() == BarrelName || e->tag() == BoatE) {
			
			file << TileToFile(e) << std::endl;
		}

		if (e->tag() == Collectible) {

			file << CollectibleToFile(e) << std::endl;

		}

		if (e->tag() == Robot) {

			file << PartolToFile(e) << std::endl;
		}

		if (e->tag() == Following) {

			file << FollowToFile(e) << std::endl;
		}

		if (e->tag() == MovingTile) {

			file << MovingTileToFile(e) << std::endl;
		}
		if (e->tag() == Blade) {

			file << BladeToFile(e) << std::endl;
		}

	}

		std::string in1 = "Inventory  inventory     0 0 0 0";
		std::string in2 = "Inventory  arrowicon      0 0 5 3";
		std::string in3 = "Inventory  stone1icon     0 0 7 2";
		std::string in4 = "Inventory  fireballicon   0 0 7 4";
		std::string player = "Player 640 360 48 48 5 3";

		file << in1 << std::endl;
		file << in2 << std::endl;
		file << in3 << std::endl;
		file << in4 << std::endl;
		file << player << std::endl;

	AllowSave = false;
	file.close();

}

/// <summary>
/// Tile objects to file 
/// </summary>
/// <param name="e"></param>
std::string LevelEditor::TileToFile(std::shared_ptr<Entity> e) {


	CTransform& trans = e->getComponent<CTransform>();
	CBoundingBox& box = e->getComponent<CBoundingBox>();
	
	

	Vec2 tilePos = trans.tilePos;
	Vec2 roomPos = trans.roomPos;
	
	int moveVal = box.blockMove ? 1 : 0;
	int visionVal = box.blockVision ? 1 : 0;

	std::string tag = e->tag().substr(0, e->tag().size() - 1);
	std::string animName = trans.animName;

	std::string rx = std::to_string((int)roomPos.x);
	std::string ry = std::to_string((int)roomPos.y);
	std::string tx = std::to_string((int)tilePos.x);
	std::string ty = std::to_string((int)tilePos.y);

	std::string move = std::to_string(moveVal);
	std::string vision = std::to_string(visionVal);

	return tag + " " + animName + " " + rx + " " + ry + " " + tx + " " + ty + " " + move + " " + vision;

	
}

std::string LevelEditor::CollectibleToFile(std::shared_ptr<Entity> e)
{
	std::string part1 = TileToFile(e);

	CSelectable& sel = e->getComponent<CSelectable>();

	int num = sel.Amount;

	return part1 + " " + std::to_string(num);
	
}

std::string LevelEditor::FollowToFile(std::shared_ptr<Entity> e)
{
	std::string part1 = TileToFile(e);

	CSelectable& sel = e->getComponent<CSelectable>();

	int health = sel.Amount;
	int damage = sel.Amount2;

	std::string follow = "Follow";

	int speed = 1;


	
	return part1 + " " + std::to_string(health) + " " + std::to_string(damage) + " " + follow + " " + std::to_string(speed);

}

std::string LevelEditor::MovingTileToFile(std::shared_ptr<Entity> e)
{
	std::string part1 = TileToFile(e);
	
	CSelectable& sel = e->getComponent<CSelectable>();
	int offset = sel.Amount;
	int speed = 1 + rand() % (6 - 1);

	return part1 + " " + std::to_string(offset) + " " + std::to_string(speed);

}
std::string LevelEditor::BladeToFile(std::shared_ptr<Entity> e)
{
	std::string part1 = TileToFile(e);
	CSelectable& sel = e->getComponent<CSelectable>();

	int rotationAmount = sel.Amount;

	return part1 + " " + std::to_string(rotationAmount);

}



std::string LevelEditor::PartolToFile(std::shared_ptr<Entity> e)
{
	std::string part1 = TileToFile(e);

	CSelectable& sel = e->getComponent<CSelectable>();

	int health = sel.Amount;
	int damage = sel.Amount2;
	int speed = 2;
	
	std::string pat = "Patrol";

	std::string stringpoints = "";

	auto allpoints = e->getComponent<CPatrol>().positions;

	for (auto p : allpoints) {

		Vec2 tilepos = GetTilePosition(p);

		stringpoints = stringpoints +  " " + std::to_string((int)tilepos.x) + " " + std :: to_string((int)tilepos.y);

	}

	return part1 + " " + std::to_string(health) + " " + std::to_string(damage) + " " + pat + " " + std::to_string(speed) + " "
		+ std::to_string(allpoints.size()) + " " + stringpoints;
}

Vec2 LevelEditor::GetTilePosition(Vec2 pixelPos)
{
	int tx = (int)(pixelPos.x - 32 - (offsetX * 20 * 64)) / 64;
	int ty = (int)(pixelPos.y - 32) / 64;

	Vec2 vec(tx, ty);

	

	print(std::to_string(offsetX)+ " " + std::to_string(tx) + " " + std::to_string(ty));

	return vec;
	
}

void LevelEditor::TextRendering()
{
	auto entities = m_entityManager.getEntities();
	ArrowText.setPosition(getPosition(offsetX, 0, 9, 0).x, getPosition(offsetX, 0, 9, 0).y);
	TitleText.setPosition(getPosition(offsetX, 0, 5, 0).x, getPosition(offsetX, 0, 5, 0).y);

	ArrowText2.setPosition(getPosition(offsetX, 0, 16, 0).x, getPosition(offsetX, 0, 16, 0).y);
	TitleText2.setPosition(getPosition(offsetX, 0, 12, 0).x, getPosition(offsetX, 0, 12, 0).y);

	for (auto e : entities) {

		if (e->hasComponent<CSelectable>()) {

			CSelectable& selectable = e->getComponent<CSelectable>();

			if (e->tag() == Collectible) {

				if (selectable.Selected) {
					TitleText.setString("Amount");
					ArrowText.setString(std::to_string(selectable.Amount));
					m_game->window().draw(ArrowText);
					m_game->window().draw(TitleText);
					
				}

			}

			if (e->tag() == Blade) {

				if (selectable.Selected) {
					
					TitleText.setString("Rotation Amount");
					ArrowText.setString(std::to_string(selectable.Amount));
					m_game->window().draw(ArrowText);
					m_game->window().draw(TitleText);

				}

			}

			if (e->tag() == MovingTile) {

				if (selectable.Selected) {

					TitleText.setString("Offset");
					ArrowText.setString(std::to_string(selectable.Amount));
					m_game->window().draw(ArrowText);
					m_game->window().draw(TitleText);

				}

			}


			if (e->tag() == Robot || e->tag() == Following) {

				if (selectable.Selected) {

					TitleText.setString("Health");
					ArrowText.setString(std::to_string(selectable.Amount));
					TitleText2.setString("Damage");
					ArrowText2.setString(std::to_string(selectable.Amount2));

					m_game->window().draw(ArrowText);
					m_game->window().draw(TitleText);
					m_game->window().draw(ArrowText2);
					m_game->window().draw(TitleText2);

				}

			}

		}

	}

	
}

void LevelEditor::OnSaveClicked()
{


}

void LevelEditor::ToolsScreenControl()
{
	if (ToolsScreenDrawn == m_drawToolsScreen)return;

	ToolsScreenDrawn = m_drawToolsScreen;

	if (m_drawToolsScreen) {

		InstantiateToolsObjects();

		return;

	}

	auto entities = m_entityManager.getEntities();
	

	for (auto e : entities) {

		if (e->hasComponent<CDraggable>()) {

			CDraggable& drag = e->getComponent<CDraggable>();

			if (!drag.Dragging) {
				e->destroy();
			}
			

		}

	}


}

void LevelEditor::print(std::string value)
{
	std::cout << value << "\n";
}


Vec2 LevelEditor::getPosition(int rx, int ry, int tx, int ty) const
{
	int X = (rx * 20 * 64) + (tx * 64) + 32;
	int Y = (ry * 12 * 64) + (ty * 64) + 32;


	return Vec2(X, Y);


}

void LevelEditor::sRender()
{

	m_game->window().clear(sf::Color(0, 0, 0));
	sf::RectangleShape tick({ 1.0f, 6.0f });
	tick.setFillColor(sf::Color::Black);

	if (m_drawTextures) {

		for (auto e : m_entityManager.getEntities()) {
			auto& transform = e->getComponent<CTransform>();
			sf::Color c = sf::Color::White;
			if (e->hasComponent<CAnimation>() && !e->hasComponent<CDraggable>())
			{
				auto& animation = e->getComponent<CAnimation>().animation;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				animation.getSprite().setColor(c);
				m_game->window().draw(animation.getSprite());

			}

			if (e->hasComponent<CPatrol>()) {

				if (e->hasComponent<CSelectable>()) {

					CSelectable& sel = e->getComponent<CSelectable>();
					CPatrol& pat = e->getComponent<CPatrol>();

					if (sel.Selected) {
						
						auto points = pat.positions;

						for (auto p : points) {
							sf::CircleShape circle;

							circle.setRadius(8);
							circle.setOrigin(8, 8);
							circle.setPointCount(32);
							circle.setFillColor(sf::Color(0, 255, 0, 185));
							circle.setPosition(p.x, p.y);
							
							m_game->window().draw(circle);
						}
					}
					

				}

			}


		}
		
		if (m_drawToolsScreen) {
			Vec2 size(64 * 20, 64 * 12);
			sf::RectangleShape rect({ size.x, size.y });
			rect.setPosition((offsetX * 20 * 64) + (64 * 0), 0);
			rect.setFillColor(sf::Color(96, 96, 96, 255));
			rect.setOutlineColor(sf::Color::Black);
			rect.setOutlineThickness(2);
			m_game->window().draw(rect);
		}

		for (auto e : m_entityManager.getEntities()) {
			
			auto& transform = e->getComponent<CTransform>();
			sf::Color c = sf::Color::White;
			if (e->hasComponent<CDraggable>()) {

				auto& animation = e->getComponent<CAnimation>().animation;
				animation.getSprite().setRotation(transform.angle);
				animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
				animation.getSprite().setScale(transform.scale.x, transform.scale.y);
				animation.getSprite().setColor(c);
				m_game->window().draw(animation.getSprite());
			}
		}


		for (auto e : m_entityManager.getEntities()) {

			if (e->hasComponent<CButton>()) {
				CButton& but = e->getComponent<CButton>();

				if (but.EntityName != "-1" && but.OriginalEntityPos != Vec2(-1, -1)) {
					auto entity = GetButtonEntity(e);
					
					if (entity != Fake) {
						if (entity->hasComponent<CSelectable>()) {
							CSelectable& select = entity->getComponent<CSelectable>();
							
							if (!select.Selected) {
								EnableButton(e, false);
								
							}
						}
					}	
				}
			}

			if (e->hasComponent<CBoundingBox>() && e->hasComponent<CSelectable>()) {

				CSelectable& selectable = e->getComponent<CSelectable>();

				if (selectable.Selected) {

					auto& box = e->getComponent<CBoundingBox>();
					auto& transform = e->getComponent<CTransform>();
					sf::RectangleShape rect;
					rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
					rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
					rect.setPosition(transform.pos.x, transform.pos.y);
					rect.setFillColor(sf::Color(0, 0, 0, 0));

					rect.setOutlineColor(sf::Color::Yellow);
					rect.setOutlineThickness(1);
					m_game->window().draw(rect);
				}

			}
		}
	}

	//make this boolean false to disable the objects
	m_game->window().draw(m_mouseShape);
	TextRendering();

}