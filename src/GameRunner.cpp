#include "GameRunner.h"

typedef bool (GameRunner::*NonStaticMemberFunctionPointer)(void);

// --- Constructors ---
GameRunner::GameRunner(uint pinNextGame, uint pinLED1, uint pinButton1, uint pinLED2, uint pinButton2, uint pinLED3, uint pinButton3)
{

	_numGames = NUM_GAMES;

	_games = new Game[NUM_GAMES];

	Init();	

	_pinNextGameButton = pinNextGame;

	_ledPins[0] = pinLED1;
	_ledPins[1] = pinLED2;
	_ledPins[2] = pinLED3;

	_buttonPins[0] = pinButton1;
	_buttonPins[1] = pinButton2;
	_buttonPins[2] = pinButton3;

	_setupPins();
}

void GameRunner::_setupPins()
{
	pinMode(_pinNextGameButton, INPUT_PULLUP);
	
	for(uint i : _ledPins)
    {
    	pinMode(i, OUTPUT);
		digitalWrite(i, BOX_LED_OFF);
		
    }	
	
	int _b = 0;
	for(uint i : _buttonPins)
    {
		_buttons[i] = new OneButton(i, true, true);
		_b++;
    }
}

void GameRunner::_attachButtonCalls()
{
	//TODO: attatch OneButton to member methods
	// _buttons[0]->attachClick(&this._buttonClicked0);
	// _buttons[1]->attachClick(&GameRunner::_buttonClicked1);
	// _buttons[2]->attachClick(&GameRunner::_buttonClicked2);

}

void GameRunner::Init()
{
	_currentGameIndex = 0;
	for (int gameIndex = 0; gameIndex < _numGames; gameIndex++)
	{
		_games[gameIndex].OnEnteringGame = nullptr;
		_games[gameIndex].RunGame = nullptr;
		_games[gameIndex].OnLeavingGame = nullptr;
	}

	return;
}

void GameRunner::Run()
{
	if (millis() - _lastRunTick > _gameCycleMS )
	{
		if (_games[_currentGameIndex].RunGame != nullptr) { _games[_currentGameIndex].RunGame(); }
		_lastRunTick = millis();
	}
}

void GameRunner::Reset()
{
	this->SetGame(GameRunner::DEFAULT_GAME);
	this->SetSpeed(GameRunner::DEFAULT_SPEED);
}

// uint8_t GameRunner::AddGame( bool (*game)(), void (*pre)() = nullptr,  void (*post)() = nullptr)
uint8_t GameRunner::AddGame(GameMain game, GamePrePost pre= nullptr,  GamePrePost post = nullptr)
{
	for (uint8_t gameIndex = 0; gameIndex < _numGames; gameIndex++)
	{
		// add game to first empty slot
		if (_games[gameIndex].RunGame != nullptr)
		{
			_games[gameIndex].OnEnteringGame = pre;
			_games[gameIndex].RunGame = game;
			_games[gameIndex].OnLeavingGame = post;
			return gameIndex;
		}
	}
	return _numGames;
}

void GameRunner::SetGame(uint8_t gameid, bool launchLeaving, bool launchEntering)
{
	if (launchLeaving && _games[_currentGameIndex].OnLeavingGame != nullptr) _games[_currentGameIndex].OnLeavingGame();
	if (launchEntering && _games[gameid].OnEnteringGame != nullptr) _games[gameid].OnEnteringGame();

	_currentGameIndex = gameid;
}

uint8_t GameRunner::GetGame() const
{
	return _currentGameIndex;
}

void GameRunner::_buttonClicked0() { _clickedStates[0] != _clickedStates[0]; }
void GameRunner::_buttonClicked1() { _clickedStates[1] != _clickedStates[1]; }
void GameRunner::_buttonClicked2() { _clickedStates[2] != _clickedStates[2]; }

void GameRunner::_allLedOFF() {	for(uint i : _ledPins) { digitalWrite(i, BOX_LED_OFF); } }
void GameRunner::_allLedON()  { for(uint i : _ledPins) { digitalWrite(i, BOX_LED_ON);  } }

/*************************************
* GAME	1                  			 *
*************************************/

void GameRunner::Enter_GAME1()
{
	// D_LOG("--> GAME1");
	return;
}

bool GameRunner::Run_GAME1()
{
	// p.count++;

	// D_LOG("P1: |" +String(tB[0]) + String(tB[1]) + String(tB[2]) + String(tB[3]) + String(tB[4]) + String(tB[5]) + String(tB[6]) + String(tB[7]) + "|"); 
	bool _checked = false;
	return 	_checked;
}

void GameRunner::Leave_GAME1()
{

	// D_LOG("    GAME1 --> " )
	return;
}

/*************************************
* GAME	2                             *
*************************************/

void GameRunner::Enter_GAME2()
{
	// D_LOG("--> GAME2");
	return;
}

bool GameRunner::Run_GAME2()
{
	// p.count++;

	// D_LOG("P2: |" +String(tB[0]) + String(tB[1]) + String(tB[2]) + String(tB[3]) + String(tB[4]) + String(tB[5]) + String(tB[6]) + String(tB[7]) + "|"); 
	bool _checked = false;
	return 	_checked;
}

void GameRunner::Leave_GAME2()
{

	// D_LOG("    GAME2 --> " )
	return;
}

void GameRunner::Win_GAME()
{
	for (uint i = 0; i < WIN_FLASH_CYCLE_CNT; i++)
	{
		// _allLedON();
		delay(WIN_FLASH_CYCLE_MS);
		// _allLedOFF();
		delay(WIN_FLASH_CYCLE_MS);
	}
	
	return;
}

