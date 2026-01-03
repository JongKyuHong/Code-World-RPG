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
	// 1. 콘솔 입력 버퍼 비우기 (_getch 계열)
	while (_kbhit()) {
		_getch();
	}

	// 2. 표준 입력 스트림 완전히 비우기 (cin 계열)
	std::cin.clear();
	std::cin.sync();  // 버퍼 동기화 및 비우기

	// 3. 남아있을 수 있는 모든 입력 제거
	while (std::cin.rdbuf()->in_avail() > 0) {
		std::cin.ignore();
	}
	std::cout << "\n";
	std::cout << "╔════════════════════════════════════════════════════════════╗\n";
	std::cout << "║                                                            ║\n";
	std::cout << "║                  ⚔️  캐 릭 터  생 성  ⚔️                   ║\n";
	std::cout << "║                                                            ║\n";
	std::cout << "║                  Character Creation                        ║\n";
	std::cout << "║                                                            ║\n";
	std::cout << "╚════════════════════════════════════════════════════════════╝\n";
	std::cout << "\n";
	std::cout << "  당신의 이름을 입력하여 모험을 시작하세요.\n";
	std::cout << "\n";
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "  📝 캐릭터 이름: ";

	
	std::string playerName;
	if (std::cin.peek() == '\n') {
		std::cin.ignore();
	}
	std::getline(std::cin, playerName);

	// 빈 이름 방지
	if (playerName.empty()) {
		std::cout << "\n이름을 입력하지 않았습니다. 기본 이름 'Hero'로 설정합니다.\n";
		playerName = "Hero";
	}

	player = Character::getInstance(playerName);

	std::cout << "\n환영합니다, 용사 " << playerName << "님!\n";
	std::cout << "\n초기 스탯:\n";
	std::cout << "체력: " << player->getHealth() << " / " << player->getMaxHealth() << "\n";
	std::cout << "공격력: " << player->getAttack() << "\n";
	std::cout << "레벨: " << player->getLevel() << "\n";
	std::cout << "골드: " << player->getGold() << " G\n";

	std::cout << "\n아무 키나 눌러 모험을 시작하세요...\n";
	_getch();
}

void GameManager::startPhase(PhaseType phase) {
	if (currentPhase != phase) {
		currentPhase = phase;
		currentRound = 0;

		switch (phase) {
		case PhaseType::PHASE_1:
			std::cout << "╔════════════════════════════════════════════════════════════╗\n";
			std::cout << "║                                                            ║\n";
			std::cout << "║        🌲    변 수 의  숲    🌲                           ║\n";
			std::cout << "║                                                            ║\n";
			std::cout << "║              The Forest of Variables                       ║\n";
			std::cout << "║                                                            ║\n";
			std::cout << "╚════════════════════════════════════════════════════════════╝\n";
			std::cout << "\n";
			std::cout << "    🌳                  🌳                  🌳\n";
			std::cout << "         🌲      🌲           🌲      🌲\n";
			std::cout << "\n";
			std::cout << "  \"이곳은 모든 데이터가 시작되는 곳...\"\n";
			std::cout << "  \"변하는 것과 변하지 않는 것을 구분하라.\"\n";
			std::cout << "\n";
			std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
			std::cout << "  📊 총 라운드: 5 | 난이도: ★☆☆☆☆\n";
			std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
			phase1Monsters = {{ "VariableSlime", "값이 변하는 물렁물렁한 슬라임"},
				{"ConstantGolem", "변하지 않는 단단한 바위 골렘"},
				{"TypeSpirit", "데이터의 형태를 결정하는 정령"},
				{"InitWisp", "시작할 때 빛을 내는 위스프"},
				{"DeclareShadow", "실체 없이 이름만 먼저 보이는 그림자"}
			};
			totalRoundsInPhase = 5;
			break;
		case PhaseType::PHASE_2:
			std::cout << "╔════════════════════════════════════════════════════════════╗\n";
			std::cout << "║                                                            ║\n";
			std::cout << "║        🏰    클 래 스 의  성    🏰                        ║\n";
			std::cout << "║                                                            ║\n";
			std::cout << "║              The Castle of Classes                         ║\n";
			std::cout << "║                                                            ║\n";
			std::cout << "╚════════════════════════════════════════════════════════════╝\n";
			std::cout << "\n";
			std::cout << "          🏰\n";
			std::cout << "         /||\\\n";
			std::cout << "        /_||_\\\n";
			std::cout << "       /  ||  \\\n";
			std::cout << "      |___||___|\n";
			std::cout << "\n";
			std::cout << "  \"객체들이 태어나고 소멸하는 신비로운 성...\"\n";
			std::cout << "  \"클래스는 설계도, 객체는 실체. 그 경계를 넘어라.\"\n";
			std::cout << "\n";
			std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
			std::cout << "  📊 총 라운드: 6 | 난이도: ★★★☆☆\n";
			std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
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
			std::cout << "╔════════════════════════════════════════════════════════════╗\n";
			std::cout << "║                                                            ║\n";
			std::cout << "║      ⚡  디 자 인  패 턴 의  탑  ⚡                       ║\n";
			std::cout << "║                                                            ║\n";
			std::cout << "║           The Tower of Design Patterns                     ║\n";
			std::cout << "║                                                            ║\n";
			std::cout << "╚════════════════════════════════════════════════════════════╝\n";
			std::cout << "\n";
			std::cout << "            ⚡\n";
			std::cout << "           /^\\\n";
			std::cout << "          /   \\\n";
			std::cout << "         /  ⚡  \\\n";
			std::cout << "        /_______\\\n";
			std::cout << "        |   |   |\n";
			std::cout << "        |   |   |\n";
			std::cout << "\n";
			std::cout << "  \"고대 마법사들이 남긴 설계의 정수...\"\n";
			std::cout << "  \"패턴을 이해하는 자만이 탑의 정상에 오를 수 있다.\"\n";
			std::cout << "\n";
			std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
			std::cout << "  📊 총 라운드: 4 | 난이도: ★★★★★\n";
			std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
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
	}
	else {
		currentState = GameState::BATTLE;
	}
}

void GameManager::runBattle() {
	Monster* monster = generateMonster();

	std::cout << monster->getName() << "가 나타났다." << std::endl;

	// 전투 전 버프적용, 자동전투한다면 구현
	//applyBuffItems();

	// 실제 전투
	BattleService battleService;
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
			if (askShopVisit()) {
				currentState = GameState::SHOP;
			}
			else {
				currentState = GameState::BATTLE;
			}
		}
	}
	else {
		handlePlayerDeath();
	}
}

void GameManager::runBossBattle() {
	Monster* bossMonster = generateBoss();

	std::string bossName = bossMonster->getName();

	std::cout << "보스 " << bossName << "가 나타났다." << std::endl;
	// 전투 전 버프적용, 자동전투한다면 구현
	//applyBuffItems();

	// 실제 전투
	// battleService->battle();
	std::cout << "⚔️ " << bossName << "와 전투 중 ⚔️\n";

	// 전투정보 받아와서 처리
	if (player->isAlive()) {
		std::cout << "🏆" << bossName << "와의 전투 승리!🏆\n";
		//mob킬수저장
		mobKillCounts[bossName]++;

		// reward 받는거 처리

		// 몬스터 삭제
		delete bossMonster;

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
		std::cout << "\n";
		std::cout << "╔════════════════════════════════════════════════════════════╗\n";
		std::cout << "║                                                            ║\n";
		std::cout << "║                    🏪  상 점  🏪                          ║\n";
		std::cout << "║                                                            ║\n";
		std::cout << "║                    Item Shop                               ║\n";
		std::cout << "║                                                            ║\n";
		std::cout << "╚════════════════════════════════════════════════════════════╝\n";
		std::cout << "\n";
		std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
		std::cout << "  💰 보유 골드: " << player->getGold() << " G\n";
		std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

		// 상점 아이템 목록 표시
		// shop->displayItems();

		std::cout << "\n";
		std::cout << "  [메뉴]\n";
		std::cout << "  ┌────────────────────────────────────────────────────────┐\n";
		std::cout << "  │  1. 🛒 아이템 구매                                    │\n";
		std::cout << "  │  2. 💸 아이템 판매                                    │\n";
		std::cout << "  │  3. 🚪 상점 나가기                                    │\n";
		std::cout << "  └────────────────────────────────────────────────────────┘\n";
		std::cout << "\n  선택: ";

		int choice;
		std::cin >> choice;

		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');

			std::cout << "\n❌ 숫자를 입력하세요!\n";
			continue;  // 다시 메뉴 표시
		}

		switch (choice) {
		case 1:
			// shop->buyItem(index)
			break;
		case 2:
			// shop->sellItem(index)
			break;
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


void GameManager::showPhaseClearScreen() {}

void GameManager::showEndingScreen() {
	std::cout << "\n\n";
	std::cout << "╔════════════════════════════════════════════════════════════╗\n";
	std::cout << "║                                                            ║\n";
	std::cout << "║              🎉  게 임  클 리 어!  🎉                     ║\n";
	std::cout << "║                                                            ║\n";
	std::cout << "║                   Congratulations!                         ║\n";
	std::cout << "║                                                            ║\n";
	std::cout << "╚════════════════════════════════════════════════════════════╝\n";
	std::cout << "\n";
	std::cout << "                    ⭐️\n";
	std::cout << "                   ⭐️⭐️\n";
	std::cout << "                  ⭐️⭐️⭐️\n";
	std::cout << "                 ⭐️  🏆  ⭐️\n";
	std::cout << "                ⭐️⭐️⭐️⭐️⭐️\n";
	std::cout << "\n";
	std::cout << "  당신은 디자인 패턴의 탑을 정복했습니다!\n";
	std::cout << "  모든 페이즈를 클리어하고 진정한 C++ 마스터가 되었습니다.\n";
	std::cout << "\n";
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "  📊 최종 통계\n";
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "  🎮 플레이어: " << player->getName() << "\n";
	std::cout << "  ⭐ 최종 레벨: " << player->getLevel() << "\n";
	std::cout << "  💰 보유 골드: " << player->getGold() << " G\n";
	std::cout << "  ❤️  최종 체력: " << player->getHealth() << " / " << player->getMaxHealth() << "\n";
	std::cout << "  ⚔️  공격력: " << player->getAttack() << "\n";
	std::cout << "\n";
	std::cout << "  🏆 처치한 몬스터\n";

	// 몬스터 처치 기록 출력
	for (const auto& mobKillCount : mobKillCounts) {
		std::cout << "     - " << mobKillCount.first << ": " << mobKillCount.second << "마리\n";
	}

	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "\n";
	std::cout << "  \"변수의 숲에서 시작한 여정이 디자인 패턴의 정상에서 끝났다.\"\n";
	std::cout << "  \"이제 당신은 C++의 세계를 자유롭게 탐험할 수 있다.\"\n";
	std::cout << "\n";
	std::cout << "  \"하지만 진정한 모험은 이제부터 시작이다...\"\n";
	std::cout << "\n";
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "\n";
	std::cout << "              제작: Num 1조\n";
	std::cout << "              감사합니다!\n";
	std::cout << "\n";
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "\n  계속하려면 아무 키나 누르세요...";
	_getch();

	isRunning = false;  // 게임 종료
}



bool GameManager::askShopVisit() {
	std::cout << "\n";
	std::cout << "╔════════════════════════════════════════════════════════════╗\n";
	std::cout << "║                                                            ║\n";
	std::cout << "║                  🛤️  갈 림 길  🛤️                        ║\n";
	std::cout << "║                                                            ║\n";
	std::cout << "╚════════════════════════════════════════════════════════════╝\n";
	std::cout << "\n";
	std::cout << "       🏪  ←  [당신]  →  ⚔️\n";
	std::cout << "\n";
	std::cout << "  전투를 계속하기 전에 상점을 방문하시겠습니까?\n";
	std::cout << "\n";
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "  [선택지]\n";
	std::cout << "  1. 🏪 상점 방문 (아이템 구매/판매)\n";
	std::cout << "  2. ⚔️  즉시 전투\n";
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "\n  선택: ";

	int choice;
	std::cin >> choice;

	if (choice == 1) {
		return true;
	} else {
		return false;
	}
}

void GameManager::handlePlayerDeath() {
	std::cout << "\n";
	std::cout << "╔════════════════════════════════════════════════════════════╗\n";
	std::cout << "║                                                            ║\n";
	std::cout << "║                    💀  사 망  💀                          ║\n";
	std::cout << "║                                                            ║\n";
	std::cout << "║                      You Died                              ║\n";
	std::cout << "║                                                            ║\n";
	std::cout << "╚════════════════════════════════════════════════════════════╝\n";
	std::cout << "\n";
	std::cout << "                    ___\n";
	std::cout << "                   /   \\\n";
	std::cout << "                  | x x |\n";
	std::cout << "                  |  ^  |\n";
	std::cout << "                   \\___/\n";
	std::cout << "\n";
	std::cout << "  당신은 쓰러졌습니다...\n";
	std::cout << "  하지만 모험은 여기서 끝나지 않습니다.\n";
	std::cout << "\n";
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "  [선택지]\n";
	std::cout << "  1. ♻️  재도전 (현재 전투 다시 시작)\n";
	std::cout << "  2. 🚪 게임 종료\n";
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "\n  선택: ";

	char choice;
	while (true) {
		choice = _getch();
		std::cout << choice << std::endl;

		if (choice == '1' || choice == '2') {
			break;
		}
		else {
			std::cout << "잘못된 입력입니다. 1 또는 2를 입력하세요: ";
		}
	}

	switch (choice) {
	case '1':
		retryCurrentBattle();
		break;
	case '2':
		std::cout << "게임을 종료합니다.\n";
		isRunning = false;
		currentState = GameState::ENDING;
		break;
	default:
		std::cout << "입력이 잘못되었다." << std::endl;
		break;
	}
}

void GameManager::retryCurrentBattle() {
	std::cout << "\n";
	std::cout << "╔════════════════════════════════════════════════════════════╗\n";
	std::cout << "║                                                            ║\n";
	std::cout << "║                ♻️  전 투  재 시 작  ♻️                    ║\n";
	std::cout << "║                                                            ║\n";
	std::cout << "╚════════════════════════════════════════════════════════════╝\n";
	std::cout << "\n";
	std::cout << "        ✨ 체력이 완전히 회복되었습니다! ✨\n";
	std::cout << "\n";

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

	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "  📊 현재 상태\n";
	std::cout << "  ├─ 라운드: " << currentRound << " / " << totalRoundsInPhase << "\n";
	std::cout << "  └─ 페이즈: " << phaseName << "\n";
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
	std::cout << "\n";

	if (currentRound >= totalRoundsInPhase) {
		std::cout << "  🔥 보스전으로 복귀합니다...\n";
		currentState = GameState::BOSS_BATTLE;
	} else {
		std::cout << "  ⚔️  일반 전투로 복귀합니다...\n";
		currentState = GameState::BATTLE;
	}

	std::cout << "\n  계속하려면 아무 키나 누르세요...\n";
	_getch();
}
