//boss, Editor load level, change level scene, light effect, camera effect, parallex, ai bullets texture, more AIs..
// easy/hard, explosions effect, sounds,health bar etc., improve level editor ,  trailer and video.

#include "Scene_Zelda.h"
#include "Common.h"
#include "Physics.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "GameOverScreen.h"
#include "Overworld.h"
#include "LevelPassed.h"
#include <math.h>
#include <cmath>
using namespace std;



Scene_Zelda::Scene_Zelda(GameEngine* game, const std::string& levelPath)
    : Scene(game)
    , m_levelPath(levelPath)
{
    init(m_levelPath);
}



void Scene_Zelda::init(const std::string& levelPath)
{
    loadLevel(levelPath);



   


    

    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Y, "TOGGLE_FOLLOW");      // Toggle follow camera
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");      // Toggle drawing (T)extures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION");

	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Space, "ATTACK");
	registerAction(sf::Keyboard::I, "INVENTORY");
	registerAction(sf::Keyboard::E, "INTERACT");
	registerAction(sf::Keyboard::F, "SHOOT");
	registerAction(sf::Keyboard::Num1, "WEAPON1");
	registerAction(sf::Keyboard::Num2, "WEAPON2");
	registerAction(sf::Keyboard::Num3, "WEAPON3");

    // Toggle drawing (C)ollision Boxes

    m_mouseShape.setRadius(8);
    m_mouseShape.setOrigin(8, 8);
    m_mouseShape.setPointCount(32);
    m_mouseShape.setFillColor(sf::Color(0, 255, 0, 185));


    InventoryText.setFont(m_game->assets().getFont("Megaman"));
    InventoryText.setCharacterSize(64);

    m_levelPaths.push_back("gameover.txt");
    
    m_levelPaths.push_back("overworld.txt");

    m_levelPaths.push_back("levelpassed.txt");

    ScoreText.setFont(m_game->assets().getFont("Megaman"));
    ScoreText.setCharacterSize(32);

    ShaderFiles.push_back("Shaders/red.frag");

    if (!RedTintShader.loadFromFile(ShaderFiles[0], sf::Shader::Fragment)) {

        print("error, file not found");

    }
}




void Scene_Zelda::loadLevel(const std::string& filename)
{
    m_entityManager = EntityManager();


    std::ifstream stream(filename);

    std::string entityName, name, aibehaviour;
    int rx, ry, px, py, movement, vision, health, damage, speed, numOfPoints, point, rotatingSpeed,Num ;
    int tileSpeed = 2;
    int maxDistance = 3;
    //black square blocks movement and vision.
    //red block vision but not movement.
    //white does not block vision neither movement.
    //blue blocks movement but not vision

    while (stream.good()) {

        stream >> entityName;


        if (entityName == "Player") {

            stream >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX >> m_playerConfig.CY
                >> m_playerConfig.SPEED >> m_playerConfig.HEALTH;

        }

        if (entityName == "Tile") {

            stream >> name >> rx >> ry >> px >> py >> movement >> vision;

            bool move = false;
            bool vis = false;


            if (movement == 1) { move = true; }
            if (vision == 1) { vis = true; }

            BaseTileInitialization("Tile", name, rx, ry, px, py, move, vis);

        }

        if (entityName == BoxName) {

            stream >> name >> rx >> ry >> px >> py >> movement >> vision;

            bool move = false;
            bool vis = false;

            if (movement == 1) { move = true; }
            if (vision == 1) { vis = true; }

            auto boxTile = BaseTileInitialization(BoxName, name, rx, ry, px, py, move, vis);

            boxTile->addComponent<CGrabbable>();

            

        }

       

        

        if (entityName == RobotName) {

            stream >> name >> rx >> ry >> px >> py >> movement >> vision;

            bool move = false;
            bool vis = false;

            if (movement == 1) { move = true; }
            if (vision == 1) { vis = true; }

            BaseTileInitialization(RobotName, name, rx, ry, px, py, move, vis);

        }

       

        if (entityName == CollectibleName) {

            stream >> name >> rx >> ry >> px >> py >> movement >> vision >> Num;

            bool move = false;
            bool vis = false;

            if (movement == 1) { move = true; }
            if (vision == 1) { vis = true; }

            auto coll = BaseTileInitialization(CollectibleName, name, rx, ry, px, py, move, vis);
            
            CTransform& collT = coll->getComponent<CTransform>();
            
                
            if (name == ArrowAnim) {
                collT.angle = -90;
            }
            

            coll->addComponent<CCollectible>(Num);

        }


        if (entityName == InventoryName) {

            stream >> name >> rx >> ry >> px >> py;

            std::shared_ptr<Entity> tile = m_entityManager.addEntity(InventoryName);

            auto& anim = m_game->assets().getAnimation(name);

            tile->addComponent<CAnimation>(anim, true);

            tile->addComponent<CTransform>(getPosition(rx, ry, px, py));

            tile->getComponent<CTransform>().animName = name;

            

            if (name == ArrowIAnim) {
                tile->addComponent<CInventory>(false, true, false, false);
                tile->addComponent<CSelectable>();
                InventoryTextList.push_back(KnifeString);
                

            }

            else if (name == FireBallIAnim) {
                tile->addComponent<CInventory>(false, false, false, true);
                tile->addComponent<CSelectable>();

                InventoryTextList.push_back(ElectricBallString);
                
            }

            else if (name == StoneIAnim) {
                tile->addComponent<CInventory>(false, false, true, false);
                tile->addComponent<CSelectable>();

                InventoryTextList.push_back(HammerString);
                
            }
            else {
                tile->addComponent<CInventory>(true, false, false, false);
                
            }

            
        }

        

        if (entityName == "NPC") {

            stream >> name >> rx >> ry >> px >> py >> movement >> vision >> health >> damage >> aibehaviour;

            bool move = false;
            bool vis = false;

            if (movement == 1) { move = true; }
            if (vision == 1) { vis = true; }

            auto npc = m_entityManager.addEntity("NPC");

            auto& anim = m_game->assets().getAnimation(name);

            npc->addComponent<CAnimation>(anim, true);

            npc->addComponent<CTransform>(getPosition(rx, ry, px, py));

            npc->addComponent<CBoundingBox>(anim.getSize(), move, vis);

            npc->addComponent<CDamage>(damage);

            npc->addComponent<CHealth>(health, health);

            npc->getComponent<CTransform>().roomPos = Vec2(rx, ry);

            npc->getComponent<CTransform>().tilePos = Vec2(px, py);

            if (aibehaviour == "Follow") {

                stream >> speed;

                npc->addComponent<CFollowPlayer>(getPosition(rx, ry, px, py), speed);

            }
            else if (aibehaviour == "Patrol") {

                stream >> speed >> numOfPoints;

                vector<Vec2> locs;

                for (int i = 0; i < numOfPoints; i++) {

                    Vec2 vec;

                    for (int k = 0; k < 2; k++) {

                        stream >> point;


                        if (k == 0) {
                            vec.x = (rx * 20 * 64) + (point * 64) + 32;
                        }

                        if (k == 1) {
                            vec.y = (ry * 12 * 64) + (point * 64) + 32;
                        }

                    }

                    locs.push_back(vec);

                }

                npc->addComponent<CPatrol>(locs, speed);

                npc->getComponent<CPatrol>().LastShotFrame = m_currentFrame;



            }

        }

        if (entityName == Blade) {

            stream >> name >> rx >> ry >> px >> py >> movement >> vision >> rotatingSpeed;
            bool move = false;
            bool vis = false;

            if (movement == 1) { move = true; }
            if (vision == 1) { vis = true; }

            auto tile = BaseTileInitialization(Blade, name, rx, ry, px, py, move, vis);

            tile->addComponent<CRotator>(rotatingSpeed);

        }



        if (entityName == MovingTile) {

            stream >> name >> rx >> ry >> px >> py >> movement >> vision >> maxDistance >> tileSpeed;

            bool move = false;
            bool vis = false;

            if (movement == 1) { move = true; }
            if (vision == 1) { vis = true; }

            auto tile = BaseTileInitialization(MovingTile, name, rx, ry, px, py, move, vis);

            tile->addComponent<CPingPongMovement>(maxDistance * 64, tileSpeed);

            CPingPongMovement& ping = tile->getComponent<CPingPongMovement>();

            ping.centerPos = tile->getComponent<CTransform>().pos;

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

       

        if (entityName == Flag) {

            stream >> name >> rx >> ry >> px >> py >> movement >> vision;

            bool move = false;
            bool vis = false;

            if (movement == 1) { move = true; }
            if (vision == 1) { vis = true; }

            auto flag = BaseTileInitialization(Flag, name, rx, ry, px, py, move, vis);

            

        }

        

        if (entityName == Heart) {

            stream >> name >> rx >> ry >> px >> py >> movement >> vision;

            bool move = false;
            bool vis = false;

            if (movement == 1) { move = true; }
            if (vision == 1) { vis = true; }

            auto heart = BaseTileInitialization(Heart, name, rx, ry, px, py, move, vis);

        }

      


            if (entityName == Boss) {

                stream >> name >> rx >> ry >> px >> py >> movement >> vision >> health >> damage;

                bool move = false;
                bool vis = false;

                if (movement == 1) { move = true; }
                if (vision == 1) { vis = true; }

                auto npc = m_entityManager.addEntity(Boss);

                auto& anim = m_game->assets().getAnimation(name);

                npc->addComponent<CAnimation>(anim, true);

                npc->addComponent<CTransform>(getPosition(rx, ry, px, py));

                npc->addComponent<CBoundingBox>(anim.getSize(), move, vis);

                npc->addComponent<CDamage>(damage);

                npc->addComponent<CHealth>(health, health);

                npc->addComponent<CSmart>();


            }

    }

    LoadGame();
    
    spawnPlayer();

    m_game->playSound("Music");
}






std::shared_ptr<Entity> Scene_Zelda::BaseTileInitialization(string entityName, string name, int rx, int ry, int px, int py, bool move, bool vis) {

    std::shared_ptr<Entity> tile = m_entityManager.addEntity(entityName);

    auto& anim = m_game->assets().getAnimation(name);

    tile->addComponent<CAnimation>(anim, true);

    tile->addComponent<CTransform>(getPosition(rx, ry, px, py));

    tile->addComponent<CBoundingBox>(anim.getSize(), move, vis);

    tile->getComponent<CTransform>().animName = name;

    tile->getComponent<CTransform>().iniPos = tile->getComponent<CTransform>().pos;

    tile->getComponent<CTransform>().roomPos = Vec2(rx, ry);

    tile->getComponent<CTransform>().tilePos = Vec2(px, py);

    return tile;

}



Vec2 Scene_Zelda::getPosition(int rx, int ry, int tx, int ty) const
{
    //width of room 20 tiles
    //height of the room 12 tiles.

    //P(x,y) of R(1,0)

    //Px = 1 * 20 * 64(size of each tile);
    //Py = 0 * 12 * 64

    int X = (rx * 20 * 64) + (tx * 64) + 32;
    int Y = (ry * 12 * 64) + (ty * 64) + 32;




    return Vec2(X, Y);
}

Vec2 Scene_Zelda::GetTilePos(Vec2 pixelPos)
{
    int tx = (int)(pixelPos.x - 32 - (OffsetX * 20 * 64)) / 64;
    int ty = (int)(pixelPos.y - 32) / 64;

    Vec2 vec(tx, ty);

    return vec;
}

void Scene_Zelda::spawnPlayer()
{
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CTransform>(Vec2(m_playerConfig.X, m_playerConfig.Y));
    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("playerball"), true);
    m_player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY), true, false);
    m_player->addComponent<CHealth>(m_playerConfig.HEALTH, m_playerConfig.HEALTH);
    m_player->addComponent<CState>();

    CTransform& pTra = m_player->getComponent<CTransform>();

    if (CurrentLevel == HighestLevel) {

        pTra.pos = Vec2((HighestOffsetX * 20 * 64) + 64 , (12*64)/2 );

    }


}

void Scene_Zelda::BossRoll()
{
    if (!BossRolling)return;

    auto entities = m_entityManager.getEntities(Boss);
    
    for (auto e : entities) {
        CTransform& tran = e->getComponent<CTransform>();
        CTransform& pTran = m_player->getComponent<CTransform>();

        if ((pTran.pos.x - tran.pos.x) > 0) {

            tran.velocity.x = 4;

            tran.angle += 5;

        }
        else {

            tran.velocity.x = -4;

            tran.angle -= 5;
        }

    }

    

}

void Scene_Zelda::BossShoot()
{
    if (!BossShooting)return;

    auto entities = m_entityManager.getEntities(Boss);

    for (auto e : entities) {
        CSmart& smart = e->getComponent<CSmart>();
        CTransform& tran = e->getComponent<CTransform>();
        CTransform& pTran = m_player->getComponent<CTransform>();

        int random_integer = 30 + rand() % (100 - 30);

        if (m_currentFrame - smart.FrameShot > random_integer) {

            EnemyBulletsSpawn(GlowingBall, e);
            smart.FrameShot = m_currentFrame;
        }
    }


}

void Scene_Zelda::BossController()
{
    auto entities = m_entityManager.getEntities(Boss);
    
    for (auto e : entities) {

        CSmart& smart = e->getComponent<CSmart>();
        CTransform& tran = e->getComponent<CTransform>();
        CTransform& pTran = m_player->getComponent<CTransform>();

        if (m_currentFrame - smart.BehaviourChange > 120) {

            smart.BehaviourChange = m_currentFrame;

            float val = 0 + rand() % (100 - 0);


            if (val < 50) {

                BossRolling = true;
                BossShooting = false;

            }
            else {
                BossRolling = false;
                BossShooting = true;
                tran.velocity.x = 0;
            }

        }

    }


}

void Scene_Zelda::ChangeLevel()
{
    if (CurrentLevel >= 3)return;

    CurrentLevel += 1;

    std::ofstream stream("SavedGame.txt");

    stream << std::to_string(CurrentLevel) + " " + "0 1";

    m_game->changeScene("OVERWORLD", std::make_shared<Overworld>(m_game, m_levelPaths[1]));

}



void Scene_Zelda::spawnSword(std::shared_ptr<Entity> entity)
{
    
    CTransform& transform = entity->getComponent<CTransform>();

    auto sword = m_entityManager.addEntity("Sword");
    sword->addComponent<CTransform>(transform.pos);

    CTransform& swordTransform = sword->getComponent<CTransform>();

    swordTransform.pos = transform.pos + transform.facing * 64;
    std::string animName = "";

    if (transform.facing.x == 1 && transform.facing.y == 0) {
        animName = "SwordRight";
        sword->addComponent<CAnimation>(m_game->assets().getAnimation(animName), false);

    }

    if (transform.facing.x == -1 && transform.facing.y == 0) {
        animName = "SwordRight";
        sword->addComponent<CAnimation>(m_game->assets().getAnimation(animName), false);
        swordTransform.scale.x = -1;
    }

    if (transform.facing.x == 0 && transform.facing.y == -1) {
        animName = "SwordUp";
        sword->addComponent<CAnimation>(m_game->assets().getAnimation(animName), false);
    }

    if (transform.facing.x == 0 && transform.facing.y == 1) {
        animName = "SwordUp";
        sword->addComponent<CAnimation>(m_game->assets().getAnimation(animName), false);
        swordTransform.scale.y = -1;
    }

    sword->addComponent<CBoundingBox>(m_game->assets().getAnimation(animName).getSize(), 1, 0);
    m_game->playSound("Slash");
}

void Scene_Zelda::update()
{
    m_entityManager.update();

    // STUDENT TODO:
    // Implement pause functionality

    sMovement();
    
    BulletShootController();
    BulletsMovement();
    sAI();
    sStatus();
    sCollision();
    sAnimation();
    
    InventoryScreen();
    SoundControl();
    
    MoveTiles();
    RotateTiles();
    sCamera();
    GameOverCheck();
    LevelChange();
    AutoGameSave();
    m_game->assets().getSound("Music").setVolume(MasterVolume);

    m_currentFrame++;
}

void Scene_Zelda::sMovement()
{
    CInput& input = m_player->getComponent<CInput>();

    CTransform& transform = m_player->getComponent<CTransform>();

    auto boxes = m_entityManager.getEntities(BoxName);
    
    

    if (!m_OnGround)
    {
        transform.velocity.y += 1;
    }
    else
    {
        transform.velocity.y = 0;

    }

    for (auto box : boxes) {

        CGrabbable& grab = box->getComponent<CGrabbable>();
        CTransform& boxTrans = box->getComponent<CTransform>();

        if (!grab.OnGround) {

            boxTrans.velocity.y += 1;
        }
        else {
            boxTrans.velocity.y += 0;
        }


    }

    if (input.attack && !swordHit && !attackPressed) {

        swordFrame = m_currentFrame;
        swordHit = true;
        attackPressed = true;

    }

    if (swordHit) {

        if ((m_currentFrame - swordFrame) <= 10) {

           // spawnSword(m_player);

        }
        else {

            swordFrame = 0;
            swordHit = false;
        }


    }


    for (auto t : m_entityManager.getEntities())
    {
        auto& transform = t->getComponent<CTransform>();
        transform.prevPos = transform.pos;
        transform.pos += transform.velocity;
    }

    if (input.right) {

        transform.pos = transform.pos + Vec2(m_playerConfig.SPEED, 0);

        //Animation

        transform.facing.x = 1;
        transform.facing.y = 0;
       
        

    }


    if (input.left) {

        transform.pos = transform.pos - Vec2(m_playerConfig.SPEED, 0);

        transform.facing.x = -1;
        transform.facing.y = 0;
        
        
    }


    if (input.up) {

        m_jumping = true;
        if (!JumpPlaying) {
            m_game->playSound("Jump");
            JumpPlaying = true;
            JumpFrame = m_currentFrame;
        }
        
        
    }

    if (input.down) {

        transform.pos = transform.pos + Vec2(0, m_playerConfig.SPEED);

        transform.facing.x = 0;
        transform.facing.y = 1;

    }

    if (m_jumping) {

        transform.pos.y += -22;
        
    }

    CState& state = m_player->getComponent<CState>();
    if (m_OnGround && !input.left && !input.right)
    {
        state.state = "Stand";
    }


}

void Scene_Zelda::SoundControl()
{
    if (m_currentFrame - JumpFrame > 45) {
        JumpPlaying = false;

    }
    
}

void Scene_Zelda::sDoAction(const Action& action)
{
    Vec2 worldPos(action.pos().x + (OffsetX * 20 * 64), action.pos().y);

    CInput& playerInput = m_player->getComponent<CInput>();

    if (action.type() == "START")
    {
        if (action.name() == "PAUSE") { setPaused(!m_paused); }
        else if (action.name() == "QUIT") { onEnd(); }
        else if (action.name() == "TOGGLE_FOLLOW") { m_follow = !m_follow; }
        else if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
        else if (action.name() == "LEFT") { playerInput.left = true; }
        else if (action.name() == "RIGHT") { playerInput.right = true; }
        else if (action.name() == "UP") { playerInput.up = true; }
        else if (action.name() == "DOWN") { playerInput.down = true; }
        else if (action.name() == "ATTACK") { playerInput.attack = true; }
        else if (action.name() == "INVENTORY") { OpenInventory = !OpenInventory; }
        else if (action.name() == "INTERACT") { Interact = !Interact; }
        else if (action.name() == "SHOOT") { if(AllowShoot)Shoot = true; AllowShoot = false; }
        else if (action.name() == "LEFT_CLICK") { LeftMouseClick(worldPos); }
        else if (action.name() == "RIGHT_CLICK") { RightMouseClick(worldPos); }
        else if (action.name() == "WEAPON1") { CurrentWeapon = ArrowBullet; }
        else if (action.name() == "WEAPON2") { CurrentWeapon = StoneBullet; }
        else if (action.name() == "WEAPON3") { CurrentWeapon = FireBallBullet; }




    }
    else if (action.type() == "END")
    {
        if (action.name() == "RIGHT") { playerInput.right = false; }
        if (action.name() == "LEFT") { playerInput.left = false; }
        if (action.name() == "UP") { playerInput.up = false; }
        if (action.name() == "DOWN") { playerInput.down = false; }
        if (action.name() == "SHOOT") { Shoot = false; AllowShoot = true; }

        if (action.name() == "ATTACK") { playerInput.attack = false; attackPressed = false; }

    }

    if (action.name() == "MOUSE_MOVE") {

        m_mouseShape.setPosition(action.pos().x + (OffsetX * 20 * 64), action.pos().y);

    }
}



void Scene_Zelda::PlayerBoxCollision()
{
    auto Boxes = m_entityManager.getEntities(BoxName);


    auto& transform = m_player->getComponent<CTransform>();

    for (auto box : Boxes) {

        if (!Interact) {

            CGrabbable& grab = box->getComponent<CGrabbable>();
            grab.PlayerCarrying = false;
            m_player->getComponent<CHealth>().playerCarrying = false;
            MovingBox = false;
            
        }
        else {

            

            Vec2 currentoverlap = Physics::GetOverlap(box, m_player);

            Vec2 prev = Physics::GetPreviousOverlap(box, m_player);

            if (currentoverlap.x > 0 && currentoverlap.y > 0) {

                if (!m_player->getComponent<CHealth>().playerCarrying) {
                    //MoveBoxWithPlayer(box);
                    CurrentBox = box;
                    MovingBox = true;
                }
                

            }
        }

        

    }

}

void Scene_Zelda::MoveControlledEntity(std::shared_ptr<Entity> e, bool Open)
{
    CTransform& connectedTrans = e->getComponent<CTransform>();

    if (Open) {
        if (connectedTrans.pos.y >= connectedTrans.iniPos.y - 256) {

            connectedTrans.pos.y = connectedTrans.pos.y - 5;

        }

        return;
    }

    if (connectedTrans.pos.y <= connectedTrans.iniPos.y) {

        connectedTrans.pos.y = connectedTrans.pos.y + 2;
        
    }
    

}

void Scene_Zelda::EnemyBulletsCleanup()
{
    auto enemybullets = m_entityManager.getEntities("EBullets");

    

    for (auto b : enemybullets) {

        CBullet& bulletComponent = b->getComponent<CBullet>();

        if (m_currentFrame - bulletComponent.FrameActive > 300) {

            b->destroy();

        }

    }
}



void Scene_Zelda::EnemyBulletsSpawn(std::string bulletName, std::shared_ptr<Entity> entity)
{
    CTransform& Etransform = entity->getComponent<CTransform>();

    auto weapon = m_entityManager.addEntity("EBullets");

    weapon->addComponent<CTransform>(Etransform.pos);

    CTransform& bulletTransform = weapon->getComponent<CTransform>();

    if (bulletName == GlowingBall) {
        weapon->addComponent<CAnimation>(m_game->assets().getAnimation(GlowingBall), true);
    }

    
    
    int speed = 10;

    int damage = 1;

    weapon->addComponent<CBullet>(damage, speed);

    weapon->getComponent<CBullet>().FrameActive = m_currentFrame;

    bulletTransform.pos = Etransform.pos + Etransform.facing * 64;

    Vec2 pPos = m_player->getComponent<CTransform>().pos;

    Vec2 npcPos = Etransform.pos;

    Vec2 direction = pPos - npcPos;

    float squaredX = pow(direction.x, 2);
    float squaredY = pow(direction.y, 2);

    float val = sqrt(squaredX + squaredY);

    Vec2 normalizedDir = direction / val;

    bulletTransform.velocity = normalizedDir * speed;
    

}



void Scene_Zelda::SpawnBullet(std::string bulletName, std::shared_ptr<Entity> entity)
{
    

    CTransform& Etransform = entity->getComponent<CTransform>();

    auto weapon = m_entityManager.addEntity("Bullets");


    weapon->addComponent<CTransform>(Etransform.pos);

    CTransform& bulletTransform = weapon->getComponent<CTransform>();

    int speed = 10;
    
    int damage = 1;

    bool posOffset = true;

    if (Etransform.facing.x == 1 && Etransform.facing.y == 0) {

        bulletTransform.angle = 0;

    }
    else if ((Etransform.facing.x == -1 && Etransform.facing.y == 0)) {

        bulletTransform.angle = 180;
        speed = -speed;
        posOffset = false;
    }

    if (bulletName == ArrowBullet) {

        if (ArrowNum > 0) {
            weapon->addComponent<CAnimation>(m_game->assets().getAnimation(ArrowAnim), true);
            weapon->addComponent<CBullet>(damage, speed);
            bulletTransform.pos = Etransform.pos + Etransform.facing * 64;
            ArrowNum -= 1;
            m_game->playSound("Arrow");
        }
        else {
            weapon->destroy();
        }
        
    }
    else if (bulletName == FireBallBullet) {

        if (FireBallNum > 0) {
            weapon->addComponent<CAnimation>(m_game->assets().getAnimation(FireBallAnim), true);
            weapon->addComponent<CBullet>(damage, speed);
            bulletTransform.pos = Etransform.pos + Etransform.facing * 64;
            FireBallNum -= 1;
            m_game->playSound("Fire");
        }
        else {
            weapon->destroy();
        }
        
    }

    else if (bulletName == StoneBullet) {

        if (StoneNum > 0) {
            weapon->addComponent<CAnimation>(m_game->assets().getAnimation(StoneAnim), true);
            weapon->addComponent<CBullet>(damage, speed, true, posOffset);

            bulletTransform.pos = Etransform.pos + Etransform.facing * 64 + Vec2(0, -64);
            StoneNum -= 1;
            m_game->playSound("Stone");

        }
        else {
            weapon->destroy();
        }
        
    }


    Shoot = false;



    print("shooting");

}

void Scene_Zelda::BulletShootController()
{
    if(Shoot){ SpawnBullet(CurrentWeapon, m_player); }
    

}

void Scene_Zelda::BulletsMovement()
{
    auto bullets = m_entityManager.getEntities("Bullets");

    for (auto b : bullets) {

        if (b->hasComponent<CBullet>()) {


            CBullet& bullet = b->getComponent<CBullet>();
            CTransform& bTrans = b->getComponent<CTransform>();

            if (!bullet.projectile) {

                int BSpeed = bullet.speed;

                CTransform& bTrans = b->getComponent<CTransform>();

                bTrans.velocity.x = BSpeed;
            }
            else {

                if (bullet.offset) {
                    
                    int BSpeed = bullet.speed - 2;
                    bullet.speedOffset += 1;

                    bTrans.velocity.x = BSpeed;

                    bTrans.velocity.y = -BSpeed + 0.5 * bullet.speedOffset;
                }
                else {

                    int BSpeed = bullet.speed+2;
                    bullet.speedOffset += 1;

                    bTrans.velocity.x = BSpeed;

                    bTrans.velocity.y = BSpeed + 0.5 * bullet.speedOffset;

                }
                
                

            }
            

        }

    }



}

void Scene_Zelda::NPCwithBullets()
{
    auto entities = m_entityManager.getEntities("NPC");
    auto boss = m_entityManager.getEntities(Boss);

    vector<std::shared_ptr<Entity>> npcs;

    for (auto e : entities) {

        npcs.push_back(e);

    }

    for (auto e : boss) {

        npcs.push_back(e);

    }

    auto playerBullets = m_entityManager.getEntities("Bullets");

    for (auto e : npcs) {

        CHealth& health = e->getComponent<CHealth>();

        if (health.Hit) {
            if (m_currentFrame - health.frameHit > 5) {
                health.Hit = false;
            }
        }

        for (auto b : playerBullets) {

            CTransform& bulletT = b->getComponent<CTransform>();
            CTransform& npcT = e->getComponent<CTransform>();
            

            Vec2 currentoverlap = Physics::GetOverlap(b, e);

            if (currentoverlap.x > 0 && currentoverlap.y > 0) {

                b->destroy();
                health.current -= 1;
                health.Hit = true;
                health.frameHit = m_currentFrame;
            }

        }

    }
}

void Scene_Zelda::BulletsCollision()
{
    auto entities = m_entityManager.getEntities();

    auto bullets = m_entityManager.getEntities("Bullets");

    auto EBullets = m_entityManager.getEntities("EBullets");

   

    for (auto e : entities) {

        for (auto b : bullets) {

            Vec2 currentoverlap = Physics::GetOverlap(b, e);

            //Vec2 prev = Physics::GetPreviousOverlap(b, e);

            if (currentoverlap.x > 0 && currentoverlap.y > 0 && 
                e!=b && !e->hasComponent<CCollectible>()) {

                CBullet& bulletB = b->getComponent<CBullet>();
                CTransform& bulletT = b->getComponent<CTransform>();

                CTransform& eTransform = e->getComponent<CTransform>();


                if (e->hasComponent<CBullet>()) {

                    Score += 20;
                    e->destroy();
                    b->destroy();
                    
                }

                else if (e == m_player) {

                    CHealth& health = m_player->getComponent<CHealth>();
                    health.current = health.current - bulletB.damage;
                    b->destroy();


                }
                else {

                    b->destroy();
                }
                //if e is npc


            }

            
        }
    }

     
    for (auto b : EBullets) {

        Vec2 currentoverlap = Physics::GetOverlap(b, m_player);

        CTransform& PTransform = m_player->getComponent<CTransform>();

        CBullet& bulletB = b->getComponent<CBullet>();
        CTransform& bulletT = b->getComponent<CTransform>();

        if (currentoverlap.x > 0 && currentoverlap.y > 0) {
            
            CHealth& health = m_player->getComponent<CHealth>();
            health.current = health.current - bulletB.damage;
            b->destroy();

        }

    }
        

    


}

void Scene_Zelda::CollectibleCollision()
{
    auto collectibles = m_entityManager.getEntities(CollectibleName);

    for (auto c : collectibles) {

        Vec2 currentoverlap = Physics::GetOverlap(c, m_player);

        if (currentoverlap.x > 0 && currentoverlap.y > 0) {

            CCollectible& coll = c->getComponent<CCollectible>();
            CTransform& cTrans = c->getComponent<CTransform>();

            std::string anim = cTrans.animName;

            m_game->playSound("Collect");

            if (anim == ArrowAnim) {
                ArrowNum += coll.Num;

            }
            if (anim == FireBallAnim) {
                FireBallNum += coll.Num;
            }
            if (anim == StoneAnim) {
                StoneNum += coll.Num;
            }

            c->destroy();
            c->destroy();
        }

    }

}

void Scene_Zelda::LeftMouseClick(Vec2 worldPos)
{
    for (auto e : m_entityManager.getEntities()) {

        if (IsInside(worldPos, e)) {

            if (e->hasComponent<CSelectable>() && e->hasComponent<CInventory>()) {
                CSelectable& selectable = e->getComponent<CSelectable>();
                CInventory& in = e->getComponent<CInventory>();
                selectable.Selected = true;
                if (in.Arrow) {
                    CurrentWeapon = ArrowBullet;
                }
                if (in.Fireball) {
                    CurrentWeapon = FireBallBullet;
                }
                if (in.Stone) {
                    CurrentWeapon = StoneBullet;
                }

            }

        }
        else {

            if (e->hasComponent<CSelectable>() && e->hasComponent<CInventory>()) {

                CSelectable& selectable = e->getComponent<CSelectable>();
                selectable.Selected = false;

            }

        }

    }
    
}

void Scene_Zelda::RightMouseClick(Vec2 worldPos)
{
    print("right clic");
}

void Scene_Zelda::OpenDoor()
{
    auto doors = m_entityManager.getEntities(Door);



    auto door = doors.at(0);
    CTransform& tran = door->getComponent<CTransform>();

    auto opendoor = BaseTileInitialization(Door, "dooropen", tran.roomPos.x, tran.roomPos.y, tran.tilePos.x, tran.tilePos.y, 1, 1);

    door->destroy();

    opendoor->removeComponent<CBoundingBox>();

  
}

bool Scene_Zelda::IsInside(Vec2 pos, std::shared_ptr<Entity> e)
{
    if (!e->hasComponent<CAnimation>()) {
        return false;

    }

    auto& halfSize = e->getComponent<CAnimation>().animation.getSize() / 2;

    Vec2 delta = (e->getComponent<CTransform>().pos - pos).abs();

    return (delta.x <= halfSize.x) && (delta.y <= halfSize.y);
}







void Scene_Zelda::print(std::string value)
{
    std::cout << value << "\n";
}

void Scene_Zelda::BoxWithEntities()
{
    vector<std::shared_ptr<Entity>> tiles;

    auto sceneObjects = m_entityManager.getEntities("Tile");

    auto otherTiles = m_entityManager.getEntities("MovingTile");

    auto otherBoxes = m_entityManager.getEntities(BoxName);


    auto boxes = m_entityManager.getEntities(BoxName);


    for (int i = 0; i < sceneObjects.size(); i++) {

        tiles.push_back(sceneObjects[i]);

    }

    for (int i = 0; i < otherTiles.size(); i++) {

        tiles.push_back(otherTiles[i]);

    }

    for (int i = 0; i < otherBoxes.size(); i++) {

       // tiles.push_back(otherBoxes[i]);

    }

    for (auto tile : tiles) {

        for (auto box : boxes) {
            
            if (box != tile) 
            {
                CTransform& boxTrans = box->getComponent<CTransform>();
                CGrabbable& grab = box->getComponent<CGrabbable>();

                Vec2 currentoverlap = Physics::GetOverlap(box, tile);

                Vec2 prev = Physics::GetPreviousOverlap(box, tile);

                if (currentoverlap.x > 0 && currentoverlap.y > 0) {

                    if (grab.PlayerCarrying) {
                        
                    }

                    ActualImpact(boxTrans, currentoverlap, prev);


                    if (prev.x > 0) {
                        box->getComponent<CGrabbable>().OnGround = true;
                    }

                }
                else {

                    box->getComponent<CGrabbable>().OnGround = false;

                }
            }

            

        }

    }
    
}

void Scene_Zelda::sAI()
{
    CTransform& playerT = m_player->getComponent<CTransform>();

    vector< std::shared_ptr<Entity>> tiles = m_entityManager.getEntities("Tile");
    auto otherNPCs = m_entityManager.getEntities("NPC");

    for (int i = 0; i < otherNPCs.size(); i++) {

        tiles.push_back(otherNPCs[i]);

    }

    for (auto entity : m_entityManager.getEntities("NPC")) {

        CTransform& npcT = entity->getComponent<CTransform>();

        CTransform& playerT = m_player->getComponent<CTransform>();

        if (playerT.pos.x > npcT.pos.x) {
            npcT.facing.x = 1;
            npcT.facing.y = 0;
        }
        else {
            npcT.facing.x = -1;
            npcT.facing.y = 0;

        }

        if (entity->hasComponent<CFollowPlayer>()) {

            CHealth& npcH = entity->getComponent<CHealth>();

            

            bool intersecting = false;

            for (auto t : tiles) {

                if (entity == t) {

                    intersecting = false;
                }

                else if (Physics::EntityIntersect(playerT.pos, npcT.pos, t)) {

                    intersecting = true;
                    break;

                }

            }

            if (!intersecting) {

                CFollowPlayer followNPC = entity->getComponent<CFollowPlayer>();

                Vec2 desired = GetDesiredDirection(playerT.pos, npcT);

                desired = desired * followNPC.speed;

                npcT.pos = npcT.pos + desired;


            }
            else {

                CFollowPlayer followNPC = entity->getComponent<CFollowPlayer>();

                if (abs(npcT.pos.x - followNPC.home.x > 1) || abs(npcT.pos.y - followNPC.home.y) > 1) {

                    Vec2 desired = GetDesiredDirection(followNPC.home, npcT);

                    desired = desired * followNPC.speed;

                    npcT.pos = npcT.pos + desired;
                }


            }

            if (npcH.current <= 0) {
                entity->destroy();
            }

        }



        if (entity->hasComponent<CPatrol>()) {

            CPatrol& npcPatrol = entity->getComponent<CPatrol>();

            CTransform& npcT = entity->getComponent<CTransform>();

            CHealth& npcHealth = entity->getComponent<CHealth>();

            vector<Vec2> allPoints = npcPatrol.positions;

            int index = GetNextIndex(npcT, allPoints, npcPatrol.currentPosition);

            //std::cout << index << "\n";

            npcPatrol.currentPosition = index;

            Vec2 targetPos = allPoints[index];

            Vec2 desired = GetDesiredDirection(targetPos, npcT);

            desired = desired * npcPatrol.speed;

            npcT.pos = npcT.pos + desired;

            int val = m_currentFrame - npcPatrol.LastShotFrame;

            

            if (val > 150) {

                if (npcT.roomPos.x == OffsetX) {
                    npcPatrol.LastShotFrame = m_currentFrame;
                    EnemyBulletsSpawn(GlowingBall, entity);
                }
                

            }

            if (npcHealth.current < 0) {
                entity->destroy();
                
            }

        }



    }


}

int Scene_Zelda::GetNextIndex(CTransform& transform, vector<Vec2> allPoint, int currentIndex) {

    int nextIndex = currentIndex;

    Vec2 currentPoint = allPoint[currentIndex];

    Vec2 transformPos = transform.pos;



    if (transformPos.dist(currentPoint) <= 1) {

        nextIndex++;

        if (nextIndex > allPoint.size() - 1) {

            nextIndex = 0;
        }

    }


    return nextIndex;

}

Vec2 Scene_Zelda::GetDesiredDirection(Vec2 target, CTransform& transform) {

    Vec2 npcCurrentPos = transform.pos;

    Vec2 desired = target - npcCurrentPos;

    desired = desired / sqrt(pow(desired.x, 2) + pow(desired.y, 2));

    return desired;
}

void Scene_Zelda::sStatus()
{
    //THIS HAS BEEN DONE ENTITYHEART() AND PLAYER ENEMYCOLLISION();

}

void Scene_Zelda::PlayerEnemyCollision() {

    auto sceneObjects = m_entityManager.getEntities("NPC");

    auto& transform = m_player->getComponent<CTransform>();

    for (auto npc : sceneObjects) {

        Vec2 currentoverlap = Physics::GetOverlap(npc, m_player);

        Vec2 prev = Physics::GetPreviousOverlap(npc, m_player);

        if (m_player->hasComponent<CInvincibility>()) {

            CInvincibility& in = m_player->getComponent<CInvincibility>();

            in.iframes -= 1;

            if (in.iframes <= 0) {

                m_player->removeComponent<CInvincibility>();
                return;
            }

        }

        if (currentoverlap.x > 0 && currentoverlap.y > 0) {

            if (!m_player->hasComponent<CInvincibility>()) {

                CHealth& health = m_player->getComponent<CHealth>();
                CDamage& EDamage = npc->getComponent<CDamage>();

                //ActualImpact(transform, currentoverlap, prev);


                health.current -= EDamage.damage;

                m_game->playSound("LinkHurt");

                m_player->addComponent<CInvincibility>(360);

               /* if (health.current <= 0) {

                    m_game->playSound("LinkDie");
                    CTransform& tra = m_player->getComponent<CTransform>();
                    tra.pos = Vec2(m_playerConfig.X, m_playerConfig.Y);
                    health.current = health.max;


                }*/
            }


        }



    }


}

void Scene_Zelda::SwordNPC() {

    auto sceneObjects = m_entityManager.getEntities("NPC");

    auto swords = m_entityManager.getEntities("Sword");

    for (auto sword : swords) {

        for (auto npc : sceneObjects) {

            Vec2 currentoverlap = Physics::GetOverlap(npc, sword);

            if (currentoverlap.x > 0 && currentoverlap.y > 0) {

                CHealth& npcHealth = npc->getComponent<CHealth>();

                npcHealth.current -= 0.001f;

                m_game->playSound("EnemyHit");

                if (npcHealth.current <= 0) {

                    npc->getComponent<CAnimation>().animation.update();

                    npc->addComponent<CAnimation>(m_game->assets().getAnimation("Explosion"), true);
                    npc->destroy();
                }

            }

        }

    }

}

void Scene_Zelda::EntityHeart() {

    auto sceneObjects = m_entityManager.getEntities(Heart);
    CHealth& health = m_player->getComponent<CHealth>();

    for (auto heart : sceneObjects) {

        CAnimation& anim = heart->getComponent<CAnimation>();

        Vec2 currentoverlap = Physics::GetOverlap(heart, m_player);

        if (currentoverlap.x > 0 && currentoverlap.y > 0) {

            health.current = health.max;
            heart->destroy();
            m_game->playSound("GetItem");

        }



    }


}

void Scene_Zelda::BlackTile() {

    auto sceneObjects = m_entityManager.getEntities("Tile");
    CTransform& transform = m_player->getComponent<CTransform>();
    vector<Vec2> blackTiles;

    for (auto black : sceneObjects) {

        CAnimation& anim = black->getComponent<CAnimation>();



        if (anim.animation.getName() == "Black") {

            CTransform& trans = black->getComponent<CTransform>();

            blackTiles.push_back(trans.pos);

        }

    }

    for (auto black : sceneObjects) {

        CAnimation& anim = black->getComponent<CAnimation>();

        Vec2 currentoverlap = Physics::GetOverlap(black, m_player);

        if (currentoverlap.x > 0 && currentoverlap.y > 0) {

            if (anim.animation.getName() == "Black") {

                int random_integer = 0 + rand() % (blackTiles.size());

                Vec2 tile = blackTiles[random_integer];

                transform.pos = tile + Vec2(0, 64);

            }

        }



    }

}

void Scene_Zelda::EntityTile() {

    auto sceneObjects = m_entityManager.getEntities("Tile");

    auto npcS = m_entityManager.getEntities("NPC");

    for (auto tile : sceneObjects) {


        for (auto npc : npcS) {

            Vec2 currentoverlap = Physics::GetOverlap(npc, tile);

            Vec2 prev = Physics::GetPreviousOverlap(npc, tile);

            CTransform& npcT = npc->getComponent<CTransform>();

            if (currentoverlap.x > 0 && currentoverlap.y > 0) {

                if (prev.x > 0) {


                    if (npcT.prevPos.y > npcT.pos.y) {

                        npcT.pos = npcT.pos + Vec2(0, currentoverlap.y);


                    }
                    else {

                        npcT.pos = npcT.pos + Vec2(0, -currentoverlap.y);

                    }

                }
                else if (prev.y > 0) {

                    if (npcT.prevPos.x > npcT.pos.x) {

                        npcT.pos = npcT.pos + Vec2(currentoverlap.x, 0);

                    }
                    else {

                        npcT.pos = npcT.pos + Vec2(-currentoverlap.x, 0);

                    }

                }

            }

        }

    }
}

void Scene_Zelda::PlayerBladeCollision() {

    auto sceneObjects = m_entityManager.getEntities(Blade);

    auto& transform = m_player->getComponent<CTransform>();

    for (auto blade : sceneObjects) {

        Vec2 currentoverlap = Physics::GetOverlap(blade, m_player);
        Vec2 prev = Physics::GetPreviousOverlap(blade, m_player);

        if (currentoverlap.x > 0 && currentoverlap.y > 0) {

            ActualImpact(transform, currentoverlap, prev);

            CHealth& health = m_player->getComponent<CHealth>();

            health.current -= 1;

            m_game->playSound("LinkHurt");


        }

    }

}



void Scene_Zelda::ActualImpact(CTransform& transform, Vec2& currentoverlap, Vec2& prev) {



    if (prev.x > 0) {


        if (transform.prevPos.y > transform.pos.y) {

            transform.pos = transform.pos + Vec2(0, currentoverlap.y);

            
            transform.velocity = Vec2(0, 0);




        }
        else {

            transform.pos = transform.pos + Vec2(0, -currentoverlap.y);
            transform.velocity = Vec2(0, 0);

           
        }

    }
    else if (prev.y > 0) {

        if (transform.prevPos.x > transform.pos.x) {

            transform.pos = transform.pos + Vec2(currentoverlap.x, 0);

        }
        else {

            transform.pos = transform.pos + Vec2(-currentoverlap.x, 0);

        }

    }

}

void Scene_Zelda::sCollision()
{

    PlayerEnemyCollision();

    SwordNPC();
    EntityHeart();
    BlackTile();
    EntityTile();
    PlayerBladeCollision();
    PlayerBoxCollision();
    BoxWithEntities();
   
    BulletsCollision();
    CollectibleCollision();
    
    NPCwithBullets();
    
   
        
    vector<std::shared_ptr<Entity>> tiles;

    auto sceneObjects = m_entityManager.getEntities("Tile");

    auto otherTiles = m_entityManager.getEntities("MovingTile");

    auto gates = m_entityManager.getEntities(GateName);

    auto doors = m_entityManager.getEntities(Door);


    for (int i = 0; i < sceneObjects.size(); i++) {

        tiles.push_back(sceneObjects[i]);

    }

    for (int i = 0; i < otherTiles.size(); i++) {

        tiles.push_back(otherTiles[i]);

    }

    for (int i = 0; i < gates.size(); i++) {

        tiles.push_back(gates[i]);

    }

    for (int i = 0; i < doors.size(); i++) {

        tiles.push_back(doors[i]);

    }


    auto& transform = m_player->getComponent<CTransform>();




    /*for (auto t : sceneObjects) {

        CTransform& objectsT = t->getComponent<CTransform>();

        CBoundingBox& objectsB = t->getComponent<CBoundingBox>();

        if (transform.pos.dist(objectsT.pos) <= (playerB.halfSize.x + objectsB.halfSize.x)) {

            if (transform.facing.x == 1 && transform.facing.y == 0) {

                collisionRight = true;

            }
            if (transform.facing.x == -1 && transform.facing.y == 0) {

                collisionLeft = true;

            }

            if (transform.facing.x == 0 && transform.facing.y == -1) {

                collisionUp = true;

            }

            if (transform.facing.x == 0 && transform.facing.y == 1) {

                collisionDown = true;

            }


        }
        else {

            collisionRight = false;
            collisionLeft = false;
            collisionUp = false;
            collisionDown = false;
        }

    }*/



    for (auto t : tiles) {

        Vec2 currentoverlap = Physics::GetOverlap(t, m_player);



        Vec2 prev = Physics::GetPreviousOverlap(t, m_player);

        //collision happened
        if (currentoverlap.x > 0 && currentoverlap.y > 0) {


            ActualImpact(transform, currentoverlap, prev);

            if (prev.x > 0) {
                m_OnGround = true;
                m_jumping = false;
            }

        }
        else {

            m_OnGround = false;

        }

    }



}

void Scene_Zelda::RotateTiles() {

    auto entities = m_entityManager.getEntities(Blade);

    for (auto e : entities) {

        CRotator& rotator = e->getComponent<CRotator>();
        CTransform& trans = e->getComponent<CTransform>();

        trans.angle += rotator.rotatingSpeed;


    }

}

void Scene_Zelda::MoveTiles() {

    auto entities = m_entityManager.getEntities(MovingTile);

    for (auto e : entities) {

        CPingPongMovement& move = e->getComponent<CPingPongMovement>();

        CTransform& trans = e->getComponent<CTransform>();

        float maxPos = move.centerPos.y + (move.maxDistance);
        float minPos = move.centerPos.y - (move.maxDistance);



        if (move.m_tileup) {

            trans.velocity = Vec2(0,move.speed);

        }
        else {
            trans.velocity = Vec2(0, -move.speed);

        }



        if (trans.pos.y >= maxPos) {

            move.m_tileup = false;

        }

        if (trans.pos.y <= minPos) {

            move.m_tileup = true;

        }

    }



}

void Scene_Zelda::sAnimation()
{
    for (auto t : m_entityManager.getEntities())
    {
        auto& anime = t->getComponent<CAnimation>();
        anime.animation.update();
        if (anime.repeat == false)
        {
            if (anime.animation.hasEnded())
            {
                t->destroy();
            }

        }
    }

    CTransform& transform = m_player->getComponent<CTransform>();

    CInput& input = m_player->getComponent<CInput>();

    transform.scale.y = 1;

    if (transform.facing.x == 1 && transform.facing.y == 0) {

        if (input.right) {
            //AnimationSet("RunRight", true);
        }

        else {
            //AnimationSet("StandRight", true);
        }

        if (input.attack) {
            //AnimationSet("AtkRight", true);
        }

        transform.scale.x = 1;
    }

    if (transform.facing.x == -1 && transform.facing.y == 0) {

        if (input.left) {
            //AnimationSet("RunRight", true);
        }


        else {
            //AnimationSet("StandRight", true);
        }

        if (input.attack) {
            //AnimationSet("AtkRight", true);
        }

        transform.scale.x = -1;

    }

    if (transform.facing.x == 0 && transform.facing.y == -1) {

        if (input.up) {
            //AnimationSet("RunUp", true);
        }


        else {
            //AnimationSet("StandUp", true);
        }

        if (input.attack) {
            //AnimationSet("AtkUp", true);
        }

    }

    if (transform.facing.x == 0 && transform.facing.y == 1) {

        if (input.down) {
            //AnimationSet("RunDown", true);
        }

        else {
            //AnimationSet("StandDown", true);
        }

        if (input.attack) {
            //AnimationSet("AtkDown", true);
        }

    }


}

void Scene_Zelda::AnimationSet(std::string anim, bool repeat) {

    m_player->addComponent<CAnimation>(m_game->assets().getAnimation(anim), repeat);
}

void Scene_Zelda::LevelChange()
{
    auto entities = m_entityManager.getEntities(Flag);

    for (auto e : entities) {

        Vec2 currentoverlap = Physics::GetOverlap(e, m_player);

        if (currentoverlap.x > 0 && currentoverlap.y > 0) {

            ResetCamera();

            std::ofstream file("SavedGame.txt");

            CurrentLevel += 1;

            file << std::to_string(CurrentLevel) + " " + "0 1" << endl;

            file.close();

            m_game->changeScene("LEVELCHANGE", std::make_shared<LevelPassed>(m_game, m_levelPaths[2]));

        }

    }
}

void Scene_Zelda::LoadGame()
{
    std::ifstream stream("SavedGame.txt");

    std::string maxlevel, maxoffset, currentL;

    while (stream.good())
    {
        stream >> maxlevel >> maxoffset >> currentL;

        HighestLevel = stoi(maxlevel);

        HighestOffsetX = stoi(maxoffset);

        CurrentLevel = stoi(currentL);

    }


}

void Scene_Zelda::ResetCamera()
{
    sf::View view = m_game->window().getView();

    view.setCenter((20*64)/2, (12*64)/2);

    m_game->window().setView(view);

}

void Scene_Zelda::GameOverScene()
{

    ResetCamera();

    m_game->changeScene("GAMEOVER", std::make_shared<GameOverScreen>(m_game, m_levelPaths[0]));

}

void Scene_Zelda::AutoGameSave()
{
    

    
    if (OffsetX > HighestOffsetX) {

        HighestOffsetX = OffsetX;

        std::ofstream file("SavedGame.txt");

       

        std::string newData = std::to_string(HighestLevel) + " " + std::to_string(OffsetX) + " " + std::to_string(CurrentLevel);

        file << newData << endl;



        //CurrentLevel
        //OffsetX
        //Amount of weapons etc..

    }



    



}

void Scene_Zelda::GameOverCheck()
{
    CHealth& health = m_player->getComponent<CHealth>();

    CTransform& pTrans = m_player->getComponent<CTransform>();

    if (pTrans.pos.y >= 800 || health.current <= 0) {

        GameOverScene();
        print("game over");
    }

}

void Scene_Zelda::sCamera()
{
    

    sf::View view = m_game->window().getView();

    CTransform& transform = m_player->getComponent<CTransform>();

    Vec2 playerPos = transform.pos;

    OffsetX = (int)playerPos.x / (20 * 64);
    int offsetY = 0;

    if (OffsetX > CurrentMaxOffset) {

        CurrentMaxOffset = OffsetX;
        Score += 20;

    }

    if (playerPos.x < 0) {
        OffsetX -= 1;
    }

    int centerPixelX = (OffsetX * 20 * 64) + ((20 * 64) / 2);
    int centerPixelY = (offsetY * 12 * 64) + ((12 * 64) / 2);

    view.setCenter(centerPixelX, centerPixelY);
    
    // then set the window view
    m_game->window().setView(view);
}

void Scene_Zelda::onEnd()
{
    //m_game->playSound("MusicTitle");

    //m_game->changeScene("MENU", nullptr, true);

    GameOverScene();

}

void Scene_Zelda::InventoryScreen()
{
    CTransform& pTrans = m_player->getComponent<CTransform>();
    Vec2 PTilePos = GetTilePos(pTrans.pos);
    
    auto allInventories = m_entityManager.getEntities(InventoryName);

    for (auto i : allInventories) {

        CInventory& inventory = i->getComponent<CInventory>();
        CTransform& InventoryTrans = i->getComponent<CTransform>();

        if (OpenInventory) {

            if (!inventory.Visible) {
                
                EnableInventory(true, i);
                
                auto& anim = m_game->assets().getAnimation(InventoryTrans.animName);
                
                if (!inventory.MainInventory) {
                    i->addComponent<CBoundingBox>(anim.getSize());
                }
                
                
            }

        }
        else {

            if (inventory.Visible) {
                EnableInventory(false, i);
                i->removeComponent<CBoundingBox>();
            }

        }

        if (inventory.MainInventory) {
           
            InventoryTrans.pos = InventoryPositioning(Vec2(PTilePos.x, 5));
           
        }

        if (inventory.Arrow) {

             InventoryTrans.pos = InventoryPositioning(Vec2(PTilePos.x - 4, 5));

        }

        if (inventory.Stone) {

            InventoryTrans.pos = InventoryPositioning(Vec2(PTilePos.x, 5));

        }

        if (inventory.Fireball) {

            InventoryTrans.pos = InventoryPositioning(Vec2(PTilePos.x + 4, 5));

        }

    }

}

void Scene_Zelda::EnableInventory(bool enable, std::shared_ptr<Entity> i)
{
    CInventory& inventory = i->getComponent<CInventory>();

    if (enable) {

        inventory.Visible = true;
        return;
    }
    

    inventory.Visible = false;

}

void Scene_Zelda::Renderer(std::shared_ptr<Entity> e, sf::Color c)
{
    auto& transform = e->getComponent<CTransform>();

    auto& animation = e->getComponent<CAnimation>().animation;

    if (animation.getName() == "flag") { return; }
    animation.getSprite().setRotation(transform.angle);
    animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
    animation.getSprite().setScale(transform.scale.x, transform.scale.y);
    animation.getSprite().setColor(c);
    
    if (e->hasComponent<CHealth>()) {

        CHealth& heal = e->getComponent<CHealth>();
        if (heal.Hit) {
            m_game->window().draw(animation.getSprite(), &RedTintShader);
            return;
        }

    }

    m_game->window().draw(animation.getSprite());

    
}



Vec2 Scene_Zelda::InventoryPositioning(Vec2 position)
{
    Vec2 TilePos = position;
    Vec2 Pos = getPosition(OffsetX, 0, TilePos.x, TilePos.y);
    return Pos;
}

void Scene_Zelda::UpdateWeaponCount(std::shared_ptr<Entity> e, bool add, int value)
{
    CTransform& weaponTrans = e->getComponent<CTransform>();

    std::string animname = weaponTrans.animName;

    if (animname == ArrowIAnim) {

        if (add) {
            ArrowNum += value;
            
        }
        else {
            ArrowNum -= value;
        }

    }

    if (animname == StoneIAnim) {

        if (add) {
            StoneNum += value;

        }
        else {
            StoneNum -= value;
        }

    }

    if (animname == FireBallIAnim) {

        if (add) {
            FireBallNum += value;

        }
        else {
            FireBallNum -= value;
        }

    }


}

void Scene_Zelda::ScoreTextRendering() {

    ScoreText.setFillColor(sf::Color::White);
    std::string val = "Score: " + std::to_string(Score);
    ScoreText.setString(val);
    Vec2 scorePos = getPosition(OffsetX, 0, 0, 0);
    ScoreText.setPosition(scorePos.x, scorePos.y);
    m_game->window().draw(ScoreText);

}

void Scene_Zelda::InventoryTextRender()
{
    if (!OpenInventory)return;
    CTransform& pTrans = m_player->getComponent<CTransform>();
    Vec2 PTilePos = GetTilePos(pTrans.pos);
    InventoryText.setCharacterSize(15);
    

    for (int i = 0; i < InventoryTextList.size(); i++)
    {
        std::string text = InventoryTextList[i];
        InventoryText.setFillColor(sf::Color::White);
       
        if (text.find(KnifeString) != string::npos) 
		{
            
            InventoryText.setString(text + ": " + std::to_string(ArrowNum));

            Vec2 pos = InventoryPositioning(Vec2(PTilePos.x - 5, 7));
            InventoryText.setPosition(sf::Vector2f(pos.x,pos.y));
            
        }

        if (text.find(HammerString) != string::npos) {

            InventoryText.setString(text + ": " + std::to_string(StoneNum));

            Vec2 pos = InventoryPositioning(Vec2(PTilePos.x-1, 7));
            InventoryText.setPosition(sf::Vector2f(pos.x, pos.y));
            
        }

        if (text.find(ElectricBallString) != string::npos) {

            InventoryText.setString(text + ": " + std::to_string(FireBallNum));
            Vec2 pos = InventoryPositioning(Vec2(PTilePos.x +3, 7));
            InventoryText.setPosition(sf::Vector2f(pos.x, pos.y));
           
        }

        
        m_game->window().draw(InventoryText);
    }

    
   
}

void Scene_Zelda::sRender()
{
    // RENDERING DONE FOR YOU

    m_game->window().clear(sf::Color(0, 0, 0));
    sf::RectangleShape tick({ 1.0f, 6.0f });
    tick.setFillColor(sf::Color::Black);

    

    // draw all Entity textures / animations
    if (m_drawTextures)
    {
        sf::Color c = sf::Color::White;
        for (auto e : m_entityManager.getEntities()) {

            if (e->tag() == BackgroundTile) {
                Renderer(e, c);
            }

        }

        // draw entity animations
        for (auto e : m_entityManager.getEntities())
        {
            auto& transform = e->getComponent<CTransform>();
            
            if (e->hasComponent<CInvincibility>())
            {
                c = sf::Color(255, 255, 255, 128);
            }

            if (e->hasComponent<CAnimation>() && e->tag() != BackgroundTile)
            {
                if (e->hasComponent<CInventory>()) {

                    CInventory& in = e->getComponent<CInventory>();
                    if (in.Visible) {
                        Renderer(e, c);
                    }

                }
                else {
                    Renderer(e, c);
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

                    rect.setOutlineColor(sf::Color::Green);
                    rect.setOutlineThickness(1);
                    m_game->window().draw(rect);
                }

            }


        }



        // draw entity health bars
        for (auto e : m_entityManager.getEntities())
        {
            auto& transform = e->getComponent<CTransform>();
            if (e->hasComponent<CHealth>())
            {
                auto& h = e->getComponent<CHealth>();
                Vec2 size(64, 6);
                sf::RectangleShape rect({ size.x, size.y });
                rect.setPosition(transform.pos.x - 32, transform.pos.y - 48);
                rect.setFillColor(sf::Color(96, 96, 96));
                rect.setOutlineColor(sf::Color::Black);
                rect.setOutlineThickness(2);
                m_game->window().draw(rect);

                float ratio = (float)(h.current) / h.max;
                size.x *= ratio;
                rect.setSize({ size.x, size.y });
                rect.setFillColor(sf::Color(255, 0, 0));
                rect.setOutlineThickness(0);
                m_game->window().draw(rect);

                for (int i = 0; i < h.max; i++)
                {
                    tick.setPosition(rect.getPosition() + sf::Vector2f(i * 64 * 1 / h.max, 0));
                    m_game->window().draw(tick);
                }
            }
        }
    }

    // draw all Entity collision bounding boxes with a rectangleshape
    if (m_drawCollision)
    {
        sf::CircleShape dot(4);
        dot.setFillColor(sf::Color::Black);
        for (auto e : m_entityManager.getEntities())
        {
            if (e->hasComponent<CBoundingBox>())
            {
                auto& box = e->getComponent<CBoundingBox>();
                auto& transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));

                if (box.blockMove && box.blockVision) { rect.setOutlineColor(sf::Color::Black); }
                if (box.blockMove && !box.blockVision) { rect.setOutlineColor(sf::Color::Blue); }
                if (!box.blockMove && box.blockVision) { rect.setOutlineColor(sf::Color::Red); }
                if (!box.blockMove && !box.blockVision) { rect.setOutlineColor(sf::Color::White); }
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }

            if (e->hasComponent<CPatrol>())
            {
                auto& patrol = e->getComponent<CPatrol>().positions;
                for (size_t p = 0; p < patrol.size(); p++)
                {
                    dot.setPosition(patrol[p].x, patrol[p].y);
                    m_game->window().draw(dot);
                }
            }

            if (e->hasComponent<CFollowPlayer>())
            {
                sf::VertexArray lines(sf::LinesStrip, 2);
                lines[0].position.x = e->getComponent<CTransform>().pos.x;
                lines[0].position.y = e->getComponent<CTransform>().pos.y;
                lines[0].color = sf::Color::Black;
                lines[1].position.x = m_player->getComponent<CTransform>().pos.x;
                lines[1].position.y = m_player->getComponent<CTransform>().pos.y;
                lines[1].color = sf::Color::Black;
                m_game->window().draw(lines);
                dot.setPosition(e->getComponent<CFollowPlayer>().home.x, e->getComponent<CFollowPlayer>().home.y);
                m_game->window().draw(dot);
            }
        }    
    }

    InventoryTextRender();
    ScoreTextRendering();
    m_game->window().draw(m_mouseShape);
    
}