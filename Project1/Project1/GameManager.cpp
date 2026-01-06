#include "GameManager.h"
#include "Monster.h"
#include "NormalMonster.h"
#include "BossMonster.h"
#include "Shop.h"
#include "BattleService.h"
#include "BattleResult.h"
#include <string>
#include <vector>
#include <iostream>
#include <conio.h>
#include "Engine.h"
#include "SceneManager.h"
#include "MainMenu.h"
#include <chrono>
#include <limits>
#include <cstdlib> 
#include "MonsterEncounter.h"

#include "BattleRewardService.h"   // ✅ 추가
// ✅ Effect
#include "EffectSystem.h"
#include "EffectManager.h"

// ✅ Item/Inventory
#include "Inventory.h"
#include "Item.h"
#include "Character.h"

struct StatSnapshot {
    int hp;
    int maxHp;
    int atk;
};

static StatSnapshot snap(Character* c) {
    return { c->getHealth(), c->getMaxHealth(), c->getAttack() };
}

static std::string diffToText(const StatSnapshot& before, const StatSnapshot& after) {
    std::string out;

    auto append = [&](const std::string& name, int b, int a) {
        int d = a - b;
        if (d == 0) return;
        if (!out.empty()) out += " | ";
        out += name + (d > 0 ? " +" : " ") + std::to_string(d);
        };

    append("공격력", before.atk, after.atk);
    append("최대 체력", before.maxHp, after.maxHp);
    append("체력", before.hp, after.hp);

    if (out.empty()) out = "변화 없음";
    return out;
}

// ✅ 생성자 시그니처 변경: rewardService 주입
GameManager::GameManager(ItemContext& ctx, BattleRewardService& rewardService)
    : ctx(ctx)
    , uiManager(ctx.artRepo)          // ✅ 기존 그대로
    , rewardService(rewardService)    // ✅ 추가
{
    effectSystem = new EffectSystem();
    effectManager = new EffectManager();
}

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
		
		case GameState::INVENTORY:
            runInventory();
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

		case GameState::ENDING:
			showEndingScreen();
			isRunning = false;
			break;
		}
	}
}

void GameManager::showMainMenu() {
    Engine engine(160, 50);

    SceneManager::GetInstance().Register("MainMenu", [&]() {
        return std::make_unique<MainMenu>();
        });

    using clock = std::chrono::steady_clock;
    auto prev = clock::now();

    SceneManager::GetInstance().LoadScene("MainMenu");

    while (engine.IsRunning())
    {
        auto now = clock::now();
        std::chrono::duration<float> delta = now - prev;
        prev = now;
        float dt = delta.count();

        engine.Update(dt);

        if (Engine::exitRequested == true) {
            break;
        }
    }

    if (Engine::isMainMenu == true) {
        currentState = GameState::CHARACTER_CREATION;
    }
    else {
        currentState = GameState::ENDING;
    }

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) return;

    DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written = 0;
    COORD home{ 0, 0 };

    // 문자 지우기
    FillConsoleOutputCharacterW(hOut, L' ', cellCount, home, &written);
    // 속성 지우기(색 포함)
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, cellCount, home, &written);

    SetConsoleCursorPosition(hOut, home);
}

void GameManager::createCharacter() {
	uiManager.showCharacterCreationScreen();
	std::string playerName = uiManager.getPlayerName();
player = Character::getInstance(playerName);

	uiManager.showPlayerStats(
		player->getName(),
		player->getHealth(),
		player->getMaxHealth(),
		player->getAttack(),
		player->getLevel(),
		player->getGold()
	);
}

void GameManager::startPhase(PhaseType phase) {
	if (currentPhase != phase) {
		currentPhase = phase;
		currentRound = 0;

		switch (phase) {
		case PhaseType::PHASE_1:
			uiManager.showPhase1Entry();
			phase1Monsters = {{ "VariableSlime", "값이 변하는 물렁물렁한 슬라임"},
				{"ConstantGolem", "변하지 않는 단단한 바위 골렘"},
				{"TypeSpirit", "데이터의 형태를 결정하는 정령"},
				{"InitWisp", "시작할 때 빛을 내는 위스프"},
				{"DeclareShadow", "실체 없이 이름만 먼저 보이는 그림자"}
			};
			totalRoundsInPhase = 5;
			break;
		case PhaseType::PHASE_2:
			uiManager.showPhase2Entry();
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
			uiManager.showPhase3Entry();
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
    int choice = uiManager.askShopVisit();
    if (choice == 1) {
        currentState = GameState::BATTLE;
    }
    else if (choice == 2) {
        currentState = GameState::SHOP;
    }
    else {
        currentState = GameState::INVENTORY;
    }
}

void GameManager::runBattle() {
	Monster* monster = generateMonster();
	uiManager.clearScreen();

	std::cin.clear();
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

	// 아스키아트
	//Sleep(100);
	//{
	//	// 몬스터 등장 애니메이션
	//	Engine encounterEngine(160, 50);

	//	SceneManager::GetInstance().Register("Encounter", [&]() {
	//		return std::make_unique<MonsterEncounter>(monster, player);
	//	});

	//	using clock = std::chrono::steady_clock;
	//	auto prev = clock::now();

	//	SceneManager::GetInstance().LoadScene("Encounter");

	//	// 등장 애니메이션 루프
	//	while (encounterEngine.IsRunning()) {
	//		auto now = clock::now();
	//		std::chrono::duration<float> delta = now - prev;
	//		prev = now;
	//		float dt = delta.count();

	//		encounterEngine.Update(dt);

	//		MonsterEncounter* scene = dynamic_cast<MonsterEncounter*>(
	//			SceneManager::GetInstance().GetCurrent()
	//			);

	//		if (scene && scene->IsFinished()) {
	//			break;  // 전투 시작
	//		}
	//	}
	//}

	uiManager.clearScreen();
	uiManager.showMonsterEncounter(monster->getName());

	// 전투 전 버프적용, 자동전투한다면 구현
	//applyBuffItems();

	// 실제 전투
    // ✅ BattleService 생성자 변경: UIManager + rewardService 주입
    BattleService battleService(uiManager, rewardService);
    battleService.setInventory(&ctx.inventory);

    battleService.setOpenInventoryCallback([this]() {
        this->runInventory();
        });
	BattleResult result = battleService.battle(player, monster);

	delete monster;

	// 전투정보 받아와서 처리
	if (result.playerWon) {
		// mob킬수저장
		mobKillCounts[result.monsterName]++;

		// 다음라운드 실행
		currentRound++;
		if (currentRound >= totalRoundsInPhase) {
			currentState = GameState::BOSS_BATTLE;
		}
		else {
			// 상점에 방문하는가?
           int choice = uiManager.askShopVisit();
            if (choice == 1) {
                currentState = GameState::BATTLE;
            }
            else if (choice == 2) {
                currentState = GameState::SHOP;
            }
            else {
                currentState = GameState::INVENTORY;
            }
		}
	}
	else {
		handlePlayerDeath();
	}
}

void GameManager::runBossBattle() {
	uiManager.clearScreen();
	Monster* bossMonster = generateBoss();

	std::string bossName = bossMonster->getName();

	// 전투 전 버프적용, 자동전투한다면 구현
	//applyBuffItems();

	// 실제 전투
	BattleService battleService(uiManager, rewardService);
	battleService.setInventory(&ctx.inventory);
    battleService.setOpenInventoryCallback([this]() {
        this->runInventory();
        });

	BattleResult result = battleService.battle(player, bossMonster);

	// 전투정보 받아와서 처리
	if (player->isAlive()) {
		//mob킬수저장
		mobKillCounts[bossName]++;

		// reward 받는거 처리

		// 몬스터 삭제
		delete bossMonster;

		uiManager.waitForKeyPress();

		//다음라운드 실행
		switch (currentPhase) {
		case PhaseType::PHASE_1:
			currentState = GameState::PHASE_2;
			break;
		case PhaseType::PHASE_2:
			currentState = GameState::PHASE_3;
			break;
		case PhaseType::PHASE_3:
			currentState = GameState::ENDING;
			break;
		}
	}
	else {
		delete bossMonster;
		handlePlayerDeath();
	}
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

	int phaseNum = static_cast<int>(currentPhase);

	// 추후에 Monster 추상클래스를 상속하는 NormalMonster클래스 추가 
	return new NormalMonster(Info.name, Info.info, baseHealth, baseAttack, phaseNum);
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
	int baseHealth = (currentRound + 1) * 1000;
	int baseAttack = (currentRound + 1) * 100;

	int health = static_cast<int>(baseHealth * multiplier);
	int attack = static_cast<int>(baseAttack * multiplier);

	int phaseNum = static_cast<int>(currentPhase);

	return new BossMonster(bossName, bossInfo, health, attack, phaseNum);
}

void GameManager::runShop() {
    Shop* shop = ctx.shop;
    uiManager.runShop(*shop, *player, ctx.inventory);
    currentState = GameState::BATTLE;
}

void GameManager::runInventory() {
    while (true) {
        InventoryAction act = uiManager.askInventoryAction(ctx.inventory);

        switch (act.type) {
        case InventoryAction::Exit:
            return;

        case InventoryAction::Equip: {
            Item* it = ctx.inventory.getItems()[act.index];
            if (!it) {
                std::cout << "잘못된 선택입니다.\n";
                uiManager.waitForKeyPress();
                break;
            }

            if (!it->isEquipItem()) {
                std::cout << "장비 아이템이 아닙니다.\n";
                uiManager.waitForKeyPress();
                break;
            }

            // ✅ 적용 전/후 스탯 비교
            StatSnapshot before = snap(player);

            ctx.inventory.equipItem(player, act.index, it->getEquipSlotHint());

            StatSnapshot after = snap(player);

            uiManager.showItemActionScreen(
                "💼 아이템 장착",
                it->getName(),
                diffToText(before, after)
            );

            uiManager.waitForKeyPress();
            break;
        }

        case InventoryAction::Unequip: {
            // ✅ 해제는 아이템명을 알기 어려우니(현재 API 상)
            // 변화량만 보여주는 방식으로 처리
            StatSnapshot before = snap(player);

            ctx.inventory.unequipItem(player, act.slot);

            StatSnapshot after = snap(player);

            uiManager.showItemActionScreen(
                "🧤 장비 해제",
                (act.slot == EquipSlot::Weapon ? "Weapon Slot" : "Armor Slot"),
                diffToText(before, after)
            );

            uiManager.waitForKeyPress();
            break;
        }

        case InventoryAction::Use: {
            Item* it = ctx.inventory.getItems()[act.index];
            if (!it) {
                std::cout << "잘못된 선택입니다.\n";
                uiManager.waitForKeyPress();
                break;
            }

            // ✅ useItem은 내부에서 delete/erase 되므로 이름을 먼저 저장
            std::string itemName = it->getName();

            StatSnapshot before = snap(player);

            ctx.inventory.useItem(player, act.index, *effectSystem, *effectManager);

            StatSnapshot after = snap(player);

            uiManager.showItemActionScreen(
                "💊 아이템 사용",
                itemName,
                diffToText(before, after)
            );

            uiManager.waitForKeyPress();
            break;
        }

        default:
            break;
        }
    }
}

void GameManager::showPhaseClearScreen() {}

void GameManager::showEndingScreen() {
	uiManager.showEndingScreen(
		player->getName(),
		player->getLevel(),
		player->getGold(),
		player->getHealth(),
		player->getMaxHealth(),
		player->getAttack(),
		mobKillCounts
	);

	isRunning = false;  // 게임 종료
}

void GameManager::handlePlayerDeath() {
	uiManager.showDeathScreen();

	char choice = uiManager.askRetryOrQuit();

	switch (choice) {
	case '1':
		retryCurrentBattle();
		break;
	case '2':
		std::cout << "게임을 종료합니다.\n";
		isRunning = false;
		currentState = GameState::ENDING;
		break;
	}
}

void GameManager::retryCurrentBattle() {
	// 체력 회복
	// player->heal();

	std::string phaseName;

	switch (currentPhase) {
	case PhaseType::PHASE_1:
		phaseName = "변 수 의  숲";
		break;
	case PhaseType::PHASE_2:
		phaseName = "클 래 스 의  성";
		break;
	case PhaseType::PHASE_3:
		phaseName = "디 자 인  패 턴 의  탑";
		break;
	default:
		phaseName = "None";
		break;
	}

	uiManager.showRetryScreen(phaseName, currentRound, totalRoundsInPhase);

	if (currentRound >= totalRoundsInPhase) {
		currentState = GameState::BOSS_BATTLE;
	} else {
		currentState = GameState::BATTLE;
	}

	uiManager.waitForKeyPress();
}
