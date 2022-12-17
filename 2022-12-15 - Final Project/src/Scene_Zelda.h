#pragma once

#include "Common.h"
#include "Scene.h"
#include <map>
#include <memory>

#include "EntityManager.h"
using namespace std;

class Scene_Zelda : public Scene
{

    struct PlayerConfig
    {
        float X, Y, CX, CY, SPEED, HEALTH;
        std::string WEAPON;
    };

protected:

    std::shared_ptr<Entity> m_player;
    std::string             m_levelPath;
    PlayerConfig            m_playerConfig;
    bool                    m_drawTextures = true;
    bool                    m_drawCollision = false;
    bool                    m_follow = false;
    bool                    collisionRight = false;
    bool                    collisionLeft = false;
    bool                    collisionUp = false;
    bool                    collisionDown = false;
    bool                    swordHit = false;
    bool                    attackPressed = false;
    int                     swordFrame = 0;
    bool                    m_OnGround = false;
    bool                    m_jumping = false;
    bool                    m_tileUp = true;
    bool                    OpenInventory = false;
    bool                    Interact = false;
    bool                    Shoot = false;
    bool                    AllowShoot = false;
    bool                    AllSensorsActive = false;
    bool                    MovingBox = false;
    bool                    BossShooting = false;
    bool                    BossRolling = false;

    bool                    JumpPlaying = false;
    int                     JumpFrame = 0;
    
    sf::Text                ScoreText;
    int                     Score = 0;

    std::shared_ptr<Entity> CurrentBox;

    int                     OffsetX = 0;
    int                     CurrentMaxOffset = 0;
    int                     SensorNum = 0;
    std::vector<std::string>    m_levelPaths;

    int ArrowNum = 0;
    int StoneNum = 0;
    int FireBallNum = 0;
    int HighestOffsetX = 0;
    int HighestLevel = 1;
    int CurrentLevel = 1;
    std::string KnifeString = "Knife";
    std::string HammerString = "Hammer";
    std::string ElectricBallString = "Ball";



    sf::Text                    InventoryText;
    std::vector<std::string> InventoryTextList;

    


    const std::string MovingTile = "MovingTile";
    const std::string Blade = "Blade";
    const std::string BoxName = "Box";
    const std::string StoneName = "Stone";
    const std::string ButtonName = "Button";
    const std::string RobotName = "Robot";
    const std::string InventoryName = "Inventory";
    const std::string IconName = "Icon";
    const std::string GateName = "Gate";
    const std::string Bullet = "Bullet";
    const std::string CollectibleName = "Collectible";
    const std::string BackgroundTile = "BGTile";
    const std::string Acid = "Acid";
    const std::string Sensor = "Sensor";
    const std::string Door = "Door";
    const std::string Flag = "Flag";
    const std::string Boss = "Boss";
    const std::string Heart = "Heart";
    const std::string Boat = "Boat";






    const std::string ArrowIAnim = "arrowicon";
    const std::string StoneIAnim = "stone1icon";
    const std::string FireBallIAnim = "fireballicon";
    const std::string ArrowAnim = "arrow";
    const std::string FireBallAnim = "fireball";
    const std::string StoneAnim= "stone1";
    const std::string GlowingBall = "ball";



    const std::string ArrowBullet = "ArrowBullet";
    

    const std::string FireBallBullet = "FireBallBullet";

    const std::string StoneBullet = "StoneBullet";

    std::string CurrentWeapon = StoneBullet;

    vector<std::string> ShaderFiles;
    sf::Shader RedTintShader;

    
    sf::CircleShape     m_mouseShape;

    void init(const std::string& levelPath);

    void loadLevel(const std::string& filename);

    void onEnd();
    void update();
    void spawnPlayer();
    void spawnSword(std::shared_ptr<Entity> entity);
    void AnimationSet(std::string anim, bool repeat);
    Vec2 GetDesiredDirection(Vec2 target, CTransform& transform);
    int GetNextIndex(CTransform& transform, vector<Vec2> allPoint, int currentIndex);
    Vec2 getPosition(int sx, int sy, int tx, int ty) const;
    Vec2 GetTilePos(Vec2 pixelPos);



    void sMovement();
    void sAI();
    void sStatus();
    void sAnimation();
    void sCollision();
    void sCamera();
    void PlayerEnemyCollision();
    void PlayerBladeCollision();
    void SwordNPC();
    void EntityHeart();
    void BlackTile();
    void EntityTile();
    void MoveTiles();
    void RotateTiles();
    void ActualImpact(CTransform& transform, Vec2& currentoverlap, Vec2& prev);
    std::shared_ptr<Entity> BaseTileInitialization(string en, string name, int rx, int ry, int px, int py, bool move, bool vis);
    void InventoryScreen();
    void EnableInventory(bool enable, std::shared_ptr<Entity> e);
    void Renderer(std::shared_ptr<Entity> e, sf::Color c);
    Vec2 InventoryPositioning(Vec2 PTilePos);
    void ScoreTextRendering();
    void InventoryTextRender();
    void UpdateWeaponCount(std::shared_ptr<Entity> e, bool add, int value);
    void PlayerBoxCollision();
   
    void NPCwithBullets();
    void BoxWithEntities();
   
   
    
    void MoveControlledEntity(std::shared_ptr<Entity> e, bool open);
    void SpawnBullet(std::string bulletName, std::shared_ptr<Entity> entity);
    void EnemyBulletsSpawn(std::string bulletName, std::shared_ptr<Entity> entity);
    void LevelChange();
    void EnemyBulletsCleanup();
    void BulletShootController();
    void BulletsMovement();
    void BulletsCollision();
    
    void OpenDoor();
    void CollectibleCollision();
    void LeftMouseClick(Vec2 worldpos);
    void RightMouseClick(Vec2 worldPos);
    bool IsInside(Vec2 pos, std::shared_ptr<Entity> e);
    void ResetCamera();
    void GameOverScene();
    void GameOverCheck();
    void AutoGameSave();
    void LoadGame();
    void ChangeLevel();
    void BossRoll();
    void BossShoot();
    void BossController();
    void SoundControl();
    
  
    void print(std::string value);


public:

    Scene_Zelda(GameEngine* gameEngine, const std::string& levelPath);

    void sRender();
    void sDoAction(const Action& action);
};