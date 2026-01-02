#include "GameManager.h"
#include "Monster.h"
#include "NormalMonster.h"
#include "BossMonster.h"
#include "Shop.h"
#include <string>
#include <vector>
#include <iostream>

void GameManager::play() {
    isRunning = true;
    currentState = GameState::MAIN_MENU;

    while (isRunning) {
        switch (currentState) {
        case GameState::MAIN_MENU:
            showMainMenu();
            break;

        case GameState::CHARACTER_CREATION:
            createCharacter();
            currentState = GameState::PHASE_1;
            break;

        case GameState::PHASE_1:
            startPhase(PhaseType::PHASE_1);
            break;

        case GameState::PHASE_2:
            startPhase(PhaseType::PHASE_2);
            break;

        case GameState::PHASE_3:
            startPhase(PhaseType::PHASE_3);
            break;

        case GameState::SHOP:
            runShop();
            currentState = GameState::BATTLE;
            break;

        case GameState::BATTLE:
            runBattle();
            break;

        case GameState::BOSS_BATTLE:
            runBossBattle();
            break;

        case GameState::PHASE_CLEAR:
            showPhaseClearScreen();
            break;

        case GameState::GAME_OVER:
            showGameOverScreen();
            break;

        case GameState::ENDING:
            showEndingScreen();
            isRunning = false;
            break;
        }
    }
}

void GameManager::showMainMenu() {
     std::cout << "메인메뉴 1시작 2종료" << std::endl;
     std::string input;
     std::getline(std::cin, input);
     if (input == "1") {
         currentState = GameState::CHARACTER_CREATION;
     } else if (input == "") {
         std::cout << "게임을 종료합니다.\n";
         isRunning = false;
         currentState = GameState::ENDING;
     } else {
         std::cout << "입력이 잘못되었다." << std::endl;
     }
}

void GameManager::createCharacter() {
    std::cout << "\n=== 캐릭터 생성 ===\n";
    std::string InpName;
    std::getline(std::cin, InpName);
    player = Character::getInstance(InpName);
    std::cout << "환영합니다 용사 " << InpName <<  "님" << std::endl;
}

void GameManager::startPhase(PhaseType phase) {
    if (currentPhase != phase) {
        currentPhase = phase;
        currentRound = 0;

        switch (phase) {
        case PhaseType::PHASE_1:
             std::cout << "변수의 숲" << std::endl;
            phase1Monsters = { 
                { "VariableSlime", "값이 변하는 물렁물렁한 슬라임"}, 
                {"ConstantGolem", "변하지 않는 단단한 바위 골렘"}, 
                {"TypeSpirit", "데이터의 형태를 결정하는 정령"},
                {"InitWisp", "시작할 때 빛을 내는 위스프"}, 
                {"DeclareShadow", "실체 없이 이름만 먼저 보이는 그림자"}
            };
            totalRoundsInPhase = 5;
            break;
        case PhaseType::PHASE_2:
             std::cout << "클래스의 성" << std::endl;
            phase2Monsters = { 
                {"ClassMimic", "무엇이든 찍어낼 수 있는 상자 괴물"}, 
                {"ObjectOrc","클래스에서 실체화된 강력한 오크"}, 
                {"MemberSpider", "객체 내부의 데이터를 지키는 거미"},
                {"FunctionMage", "기능을 수행하고 값을 반환하는 마법사"},
                {"ConstructGargoyle", "객체가 생성될 때 깨어나는 가고일"},
                {"DestructReaper", "객체가 사라질 때 나타나는 사신"}
            };
            totalRoundsInPhase = 6;
            break;
        case PhaseType::PHASE_3:
             std::cout << "디자인 패턴의 탑" << std::endl;
            phase3Monsters = { 
                {"SingletonKing", "오직 하나만 존재하는 왕"}, 
                {"DecoratorKnight", "장비를 덧붙여 강해지는 기사"}, 
                {"ObserverBeholder", "상태 변화를 감시하는 눈 괴물(비홀더)"},
                {"StrategyGeneral", "상황에 따라 전술을 바꾸는 장군"}
            };
            totalRoundsInPhase = 4;
            break;
        }
    }
    // 상점 방문 여부
    if (askShopVisit()) {
        currentState = GameState::SHOP;
    } else {
        currentState = GameState::BATTLE;
    }
}

void GameManager::runBattle() {
    Monster* monster = generateMonster();

    // std::cout << 전투관련 내용들 << std::endl;

    // 전투 전 버프적용, 자동전투한다면 구현
    //applyBuffItems();

    // 실제 전투
    // battleService->battle();

    // 전투정보 받아와서 처리
    if (player->isAlive()) {
        //mob킬수저장
        mobKillCounts[monster->getName()]++;
        
        // reward 받는거 처리

        // 몬스터 삭제
        delete monster;

        //다음라운드 실행
        currentRound++;
        if (currentRound >= totalRoundsInPhase) {
            currentState = GameState::BOSS_BATTLE;
        } else {
             //상점에 방문하는가?
            if (askShopVisit()) {
                currentState = GameState::SHOP;
            } else {
                currentState = GameState::BATTLE;
            }
        }
    } else {
        delete monster;
        handlePlayerDeath();
    }
}

void GameManager::handlePlayerDeath() {
    std::cout << "캐릭터 사망" << std::endl;
    
    // 인풋받아서 처리
    // 추후에 세이브를 통해서 싸우던 라운드 혹은 페이즈로 넘어가게
    isRunning = false;
}

Monster* GameManager::generateMonster() {
    std::vector<MonsterData>* monsterInfo = nullptr;

    switch (currentPhase) {
    case PhaseType::PHASE_1:
        monsterInfo = &phase1Monsters;
        break;
    case PhaseType::PHASE_2:
        monsterInfo = &phase2Monsters;
        break;
    case PhaseType::PHASE_3:
        monsterInfo = &phase3Monsters;
        break;
    default:
        monsterInfo = &phase1Monsters;
        break;
    }

    // 현재 라운드에 맞는 몬스터
    MonsterData Info = (*monsterInfo)[currentRound % monsterInfo->size()];
    

    // 스텟은 추후에 변경
    int baseHealth = (currentRound + 1) * 20;
    int baseAttack = (currentRound + 1) * 5;
    
    // 추후에 Monster 추상클래스를 상속하는 NormalMonster클래스 추가 
    return new NormalMonster(Info.name, Info.info, baseHealth, baseAttack);
}

// 추후에 수정
Monster* GameManager::generateBoss() {
    std::string bossName;
    std::string bossInfo;
    float multiplier = 0.0f;
    bool isFinalBoss = false;

    switch (currentPhase) {
    case PhaseType::PHASE_1:
        bossName = "PointerLich";
        bossInfo = "주소를 가리키며 조종하는 강력한 리치";
        multiplier = 1.2f;
        break;
    case PhaseType::PHASE_2:
        bossName = "PolyDragon";
        bossInfo = "여러 형태(타입)로 변신하는 드래곤";
        multiplier = 1.2f;
        break;
    case PhaseType::PHASE_3:
        bossName = "TeamProjectDevil";
        bossInfo = "모든 것을 통합하고 시험하는 최종 마왕";
        multiplier = 1.5f;
        isFinalBoss = true;
        break;
    }

    // 스텟은 우선 요구사항 따라서
    int baseHealth = (currentRound + 1) * 20;
    int baseAttack = (currentRound + 1) * 5;

    int health = static_cast<int>(baseHealth * multiplier);
    int attack = static_cast<int>(baseAttack * multiplier);

    return new BossMonster(bossName, bossInfo, health, attack);
}

void GameManager::runShop() {
    Shop* shop = Shop::getInstance();
    bool shopping = true;

    while (shopping) {
        std::cout << "╔════════════════════════════════════════╗\n";
        std::cout << "║              🏪 상점 🏪               ║\n";
        std::cout << "╚════════════════════════════════════════╝\n";

        std::cout << player->getGold() << "G\n";

        // 상점 아이템 목록 표시
        // shop->displayItems();

        std::cout << "\n[메뉴]\n";
        std::cout << "1. 아이템 구매\n";
        std::cout << "2. 아이템 판매\n";
        std::cout << "3. 상점 나가기\n";
        std::cout << "선택 : ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');

            std::cout << "\n❌ 숫자를 입력하세요!\n";
            waitForInput();
            continue;  // 다시 메뉴 표시
        }

        switch (choice) {
            case 1:
                // shop->buyItem(index)
            case 2:
                // shop->sellItem(index)
            case 3:
                std::cout << "상점을 나갑니다\n";
                shopping = false;
                break;
            default:
                std::cout << "입력이 잘못됨\n";
                break;
        }
    }
    currentState = GameState::BATTLE;
}

// 보스전, 클리어시 다음 페이즈로 넘겨주는 역할
void GameManager::runBossBattle() {}

void GameManager::showPhaseClearScreen() {}

void GameManager::showGameOverScreen() {}

void GameManager::showEndingScreen() {}


bool GameManager::askShopVisit() { 
    std::cout << "상점 방문 1, 아니면 2 " << std::endl;
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        std::cout << "상점 방문" << std::endl;
        return true;
    } else {
        std::cout << "즉시 전투" << std::endl;
        return false;
    }
}

void GameManager::checkLevelUp() {}