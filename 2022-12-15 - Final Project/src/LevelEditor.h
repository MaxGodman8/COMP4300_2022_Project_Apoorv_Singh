#pragma once

#include "Common.h"
#include "Scene.h"
#include <map>
#include <memory>
#include <deque>

#include "EntityManager.h"
#include <iostream>
#include <fstream>


class LevelEditor :public Scene
{


protected:
    std::string             m_levelPath;
    bool                    m_drawTextures = true;
    bool                    m_drawToolsScreen = false;
    int                     offsetX = 0;
    bool                    RightPressed = false;
    bool                    LeftPressed = false;
    bool                    RightAllowChange = true;
    bool                    LeftAllowChange = true;
    int                     ArrowValue = 10;
    bool                    ToolsScreenDrawn = false;
    //bool                    NPCSelected = false;
    bool                    Selected = false;
    bool                    AllowSave = true;

    std::shared_ptr<Entity>  Fake;
   

    const std::string MovingTile = "MovingTileE";
    const std::string Blade = "BladeE";
    const std::string Tile = "TileE";
    const std::string MovementOff = "MovementOffE";
    const std::string MovementOn = "MovementOnE";
    const std::string VisionOff = "VisionOffE";
    const std::string VisionOn = "VisionOnE";
    const std::string Save = "SaveE";
    const std::string Collectible = "CollectibleE";
    const std::string Symbol = "SymbolE";
    const std::string Gate = "GateE";
    const std::string FloorButton = "ButtonE";
    const std::string Robot = "NPCE";
    const std::string Following = "FollowE";
    const std::string BackgroundTile = "BGTileE";
    const std::string Acid = "AcidE";
    const std::string Spike = "SpikeE";
    const std::string BoxName = "BoxE";
    const std::string BarrelName = "BarrelE";
    const std::string SensorE = "SensorE";
    const std::string FlagE = "FlagE";
    const std::string BoatE = "BoatE";




    





    std::shared_ptr<Entity> SelectedTile;
    std::string FileName = "";
    sf::Text                    ArrowText;
    sf::Text                    TitleText;
    sf::Text                    ArrowText2;
    sf::Text                    TitleText2;

    

    std::vector<std::shared_ptr<Entity>> LevelEntities;

    std::vector<Vec2> NpcPoints;

    void init(const std::string& levelPath);
    void loadLevel(const std::string& filename);

    sf::CircleShape     m_mouseShape;

public:

    LevelEditor(GameEngine* gameEngine, const std::string& levelPath);


    void update();
    void onEnd();
    void sRender();
    void sDoAction(const Action& action);
    Vec2 getPosition(int sx, int sy, int tx, int ty) const;
    void sCamera();
    bool IsInside(Vec2 pos, std::shared_ptr<Entity> e);
    void InstantiateEntity(std::shared_ptr<Entity> e);
    std::shared_ptr<Entity> BaseInstantiation(std::string entityName, std::string name, Vec2 pos, bool move, bool vis);
    std::shared_ptr<Entity> BaseTileInitialization(std::string entityName, std::string name, int rx, int ry, int px, int py, bool move, bool vis, bool allowdrag = true);
    void PositionUpdate();
    void EditorToFile();
    std::string TileToFile(std::shared_ptr<Entity> e);
    std::string CollectibleToFile(std::shared_ptr<Entity> e);
    std::string PartolToFile(std::shared_ptr<Entity> e);
    std::string FollowToFile(std::shared_ptr<Entity> e);
    std::string MovingTileToFile(std::shared_ptr<Entity> e);
    std::string BladeToFile(std::shared_ptr<Entity> e);
    std::string BoxToFile(std::shared_ptr<Entity> e);

    void OnSelected(std::shared_ptr<Entity> e);
    void EnableButton(std::shared_ptr<Entity> button, bool enable);
    void MovementVisionButtonEnabler(std::string on, std::string off, bool onVal, bool offVal, std::shared_ptr<Entity> e);
    std::shared_ptr<Entity> GetButtonEntity(std::shared_ptr<Entity> button);
    void SetButtonValueToEntity(std::shared_ptr<Entity> buttonEntity, std::string name );
    Vec2 GetTilePosition(Vec2 pos);
    void ValueChanger();
    void TextRendering();
    void OnSaveClicked();
    void ToolsScreenControl();
    void InstantiateToolsObjects();
    void DeselectAll();
    
    void print(std::string value);
};

