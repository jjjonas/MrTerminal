#ifndef GAMERUNNER_H
#define GAMERUNNER_H

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <functional>

#include "config.h"
// #include "StateMachineLib.h"
#include "OneButton.h"

#ifdef ESP32
#define BOX_LED_ON 0x1
#define BOX_LED_OFF 0x0
#elif defined(ESP8266)
#define BOX_LED_ON 0x0
#define BOX_LED_OFF 0x1
#endif

#define NUM_BUTTONS 3

#define WIN_FLASH_CYCLE_CNT 3
#define WIN_FLASH_CYCLE_MS  250

class GameRunner
{

    
    typedef bool (*GameMain)();
	typedef void (*GamePrePost)();

	// typedef struct {
	// 	void (*OnEnteringGame)();
    //     bool (*RunGame)();
	// 	void (*OnLeavingGame)();
	// } Game;

    typedef struct 
    {
        GamePrePost OnEnteringGame;
        GameMain RunGame;
        GamePrePost OnLeavingGame;
    } Game;

    public:

       
        
        enum GameID
        {
            NONE = 0,
            GAME1 = 1,
            GAME2 = 2
        };

        enum GameState
        {
            ENTER,
            RUN,
            WIN,
            LEAVE
        };

        static const GameRunner::GameID DEFAULT_GAME = GameID::GAME1;
        static const int NUM_GAMES = 2;
        static const uint16_t DEFAULT_SPEED = 10;
        
        // StateMachine stateMachine;

        // ----- Constructor ----------------------
        // GameRunner();

        GameRunner(uint pinNextGame, uint pinLED1, uint pinButton1, uint pinLED2, uint pinButton2, uint pinLED3, uint pinButton3);

        
        // init game statemachine. call in setup()
        void Init();

        // run statemachine. call in loop()
        void Run();

        // set game to DEFAULT_GAME an reset gamespeed
        void Reset();

        void SetSpeed(uint16_t speed);
        uint16_t GetSpeed() const;

        void SetRunningGame(GameID gameid, bool launchLeaving = true, bool launchEntering = true);
        // GameID GetRunningGame() const;

        // uint8_t AddGame(bool (*game)(), void  (*pre)(), void (*post)());
        uint8_t AddGame(GameMain game, GamePrePost pre, GamePrePost post);


    private:

        OneButton *_buttons[NUM_BUTTONS];

        uint _gameCycleMS = 500;
        long _lastRunTick = 0;

        uint _buttonPins[NUM_BUTTONS];
        uint _ledPins[NUM_BUTTONS];

        bool _clickedStates[NUM_BUTTONS] = {0, 0, 0};

        uint _pinNextGameButton = -1;

        Game* _games;

        uint8_t _numGames;
        GameID _currentGameID;
        GameID _requestedGameID;

        GameState _gameState;

        void _setupPins();
        
        // add game methods to gamerunner
        void _setupGames();
        
        void _attachButtonCalls();

        void _buttonClicked(uint8_t i);
        void _buttonClicked0();
        void _buttonClicked1();
        void _buttonClicked2();

        void _buttonClicked(uint i);

        // void (*_buttonClicked[NUM_BUTTONS])() = { _buttonClicked0 , _buttonClicked1 , _buttonClicked2 };
        
        void _ledON(uint i);
        void _ledOFF(uint i);

        void _allLedOFF();
        void _allLedON();

        
        void Enter_GAME1();
        bool Run_GAME1();
        void Leave_GAME1();

        void Enter_GAME2();
        bool Run_GAME2();
        void Leave_GAME2();

        void Win_GAME();

};

#endif
