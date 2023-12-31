#include "GameRunner.h"

#define SERIAL_DEBUG
#ifndef DEBUG
#define DEBUG
#endif

//--- DEBUG MACRO ------------------------------------------------------------//
String logMsg = "-";
bool flushLog_Enable = false;
#define D_LOG(msg) {	logMsg = msg;		\
                    	FlushLog();			\
                 	 }

// print log message from "String logMsg" with timestamp to serial console
void FlushLog()
{
	#ifdef SERIAL_CONSOLE
		#ifdef DEBUG
		if (flushLog_Enable)
		{
			logMsg = String(millis()) + "	::" + logMsg;
			Serial.println(logMsg);
		}
		#endif
	#endif
}

//--- Check DEFINE Errors ----------------------------------------------------//

typedef bool (GameRunner::*NonStaticMemberFunctionPointer)(void);

// --- Constructors ---
GameRunner::GameRunner(uint pinNextGame, uint pinLED1, uint pinButton1, uint pinLED2, uint pinButton2, uint pinLED3, uint pinButton3)
{


	_numGames = NUM_GAMES;

	_games = new Game[NUM_GAMES];

	_pinNextGameButton = pinNextGame;

	_ledPins[0] = pinLED1;
	_ledPins[1] = pinLED2;
	_ledPins[2] = pinLED3;

	_buttonPins[0] = pinButton1;
	_buttonPins[1] = pinButton2;
	_buttonPins[2] = pinButton3;

	_setupPins();

	Init();

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

void GameRunner::_setupGames()
{
	return;
}

void GameRunner::_attachButtonCalls()
{
	static GameRunner* self = this;
	//TODO: attatch OneButton to member methods
	_buttons[0]->attachClick([]() { self->_buttonClicked(0); });
	_buttons[1]->attachClick([]() { self->_buttonClicked1(); });
	_buttons[2]->attachClick([]() { self->_buttonClicked2(); });

}

void GameRunner::Init()
{
	_gameState = ENTER;
	_currentGameID = DEFAULT_GAME;

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

		switch (_currentGameID)
		{
		case GAME1:
			switch (_gameState)
			{
				case ENTER:
					Enter_GAME1();
					_gameState = RUN;
					break;
				case RUN:
					if (Run_GAME1()) { _gameState = WIN; }
					break;
				case WIN:
					Win_GAME();
					break;
				case LEAVE:
					Leave_GAME1();
					_currentGameID = _requestedGameID;
					_gameState = ENTER;
					break;
			}
			break;
		case GAME2:
			switch (_gameState)
			{
				case ENTER:
					Enter_GAME2();
					_gameState = RUN;
					break;
				case RUN:
					if (Run_GAME2()) { _gameState = WIN; }
					break;
				case WIN:
					Win_GAME();
					break;
				case LEAVE:
					Leave_GAME2();
					_currentGameID = _requestedGameID;
					_gameState = ENTER;
					break;
			}
			break;
		default:
			break;
		}

		
		if (_games[_currentGameID].RunGame != nullptr) { _games[_currentGameID].RunGame(); }
		_lastRunTick = millis();
	}
}

void GameRunner::Reset()
{
	this->SetRunningGame(GameRunner::DEFAULT_GAME);
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

void GameRunner::SetRunningGame(GameID gameid, bool launchLeaving, bool launchEntering)
{
	// if (launchLeaving && _games[_currentGameIndex].OnLeavingGame != nullptr) _games[_currentGameIndex].OnLeavingGame();
	// if (launchEntering && _games[gameid].OnEnteringGame != nullptr) _games[gameid].OnEnteringGame();

	if (gameid != _currentGameID)
	{
		_requestedGameID = gameid;
	}
}

// GameID GameRunner::GetRunningGame() const
// {
// 	return _currentGameID;
// }

void GameRunner::_buttonClicked(uint8_t i) { D_LOG("Button "+ String(i) +  " clicked"); _clickedStates[i] != _clickedStates[i]; digitalWrite(_ledPins[i], _clickedStates[i]); }
void GameRunner::_buttonClicked0() { D_LOG("Button 1 clicked"); this->_buttonClicked(0); }
void GameRunner::_buttonClicked1() { D_LOG("Button 2 clicked"); this->_buttonClicked(1); }
void GameRunner::_buttonClicked2() { D_LOG("Button 3 clicked"); this->_buttonClicked(2); }

void GameRunner::_allLedOFF() {	for(uint i : _ledPins) { digitalWrite(i, BOX_LED_OFF); } }
void GameRunner::_allLedON()  { for(uint i : _ledPins) { digitalWrite(i, BOX_LED_ON);  } }

/*************************************
* GAME	1 - Alle Tasten an           *
*************************************/

void GameRunner::Enter_GAME1()
{
	D_LOG("--> GAME1");
	_allLedOFF();
	return;
}

bool GameRunner::Run_GAME1()
{
	
	bool _checked = true;
	for (bool c : _clickedStates )
	{
		_checked = _checked && c;
	}
	
	return 	_checked;
}

void GameRunner::Leave_GAME1()
{
	D_LOG("    GAME1 --> " )
	return;
}

/*************************************
* GAME	2                             *
*************************************/

void GameRunner::Enter_GAME2()
{
	D_LOG("--> GAME2");
	return;
}

bool GameRunner::Run_GAME2()
{
	// p.count++;
	
	bool _checked = false;
	return 	_checked;
}

void GameRunner::Leave_GAME2()
{

	D_LOG("    GAME2 --> " )
	return;
}

void GameRunner::Win_GAME()
{
	D_LOG("    WIN     " )
	for (uint i = 0; i < WIN_FLASH_CYCLE_CNT; i++)
	{
		_allLedON();
		delay(WIN_FLASH_CYCLE_MS);
		_allLedOFF();
		delay(WIN_FLASH_CYCLE_MS);
	}
	return;
}

