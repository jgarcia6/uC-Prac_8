/* Reaction time game */
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "my_gpio.h"

#define LED_LINE0   eGpioNum23
#define LED_LINE1   eGpioNum22

#define BTN_GPIO    eGpioNum5

#define NUM_LED_PER_COLOR 9

// Enumerations
typedef enum LedColor_tag
{
  eRedLed = 0,
  eGreenLed = 1,
  eNumOfColors = 2
}eLedColor_t;

typedef enum ButtonState_tag
{
    eBtnUndefined = 0,
    eBtnShortKeyPress,
    eBtnDoubleKeyPress,
    eBtnLongKeyPress
} eButtonState_t;

typedef enum GameState_tag
{
    eGameRestart = 0,
    eOngoingGame, 
    eStalemate, 
    eRedPlayerWin, 
    eGreenPlayerWin
} eGameState_t;

typedef struct BoardState_tag
{
    bool gameBoard[eNumOfColors][NUM_LED_PER_COLOR];
    uint8_t cursor;
    eLedColor_t currentColor;
}sBoardState_t;

// Global variable
uint32_t _millis;

void delayMs(uint16_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

static void initIO(void)
{
    // FIXME:
    // Replace the following code and insert
    // code to initialize all IO pins for the assigment
    gpio_init(BTN_GPIO, eInput);

}

void displayBoard(sBoardState_t *boardState)
{
    // FIXME:
    // Display current board status specified in argument boardState.gameBoard[][]
    if (gpio_read(BTN_GPIO) == eLow)
    {
        gpio_init(LED_LINE0, eOutput);
        gpio_init(LED_LINE1, eOutput);
        gpio_write(LED_LINE0, eHigh);
        gpio_write(LED_LINE1, eHigh);
        delayMs(500); 
        // Hint: there is a line missing here
        gpio_write(LED_LINE0, eInput);
        gpio_write(LED_LINE1, eInput);
        delayMs(100); 
    }
}

eGameState_t checkBoard(sBoardState_t *boardState, eButtonState_t buttonState)
{
    // check player input
    switch (buttonState)
    {
        case eBtnShortKeyPress:
            boardState->cursor = (boardState->cursor + 1) % NUM_LED_PER_COLOR;
            break;
        case eBtnDoubleKeyPress:
            boardState->cursor = (boardState->cursor - 1) % NUM_LED_PER_COLOR;
            break;
        case eBtnLongKeyPress:
            boardState->gameBoard[boardState->currentColor][boardState->cursor] = true;
            boardState->currentColor = (boardState->currentColor == eRedLed)? eGreenLed : eRedLed;
            break;
        default:
            break;
    } 
    // FIXME
    // Check next available cursor position, if no available position is found, then stalmate condition has been met
    // Check if any vertical, horizontal or diagonal 3 adjacent spaces are filled by the same color
    // If so, then a winner is found and return the corresponding game state
    return eOngoingGame;
}

bool playSequence(eGameState_t gameState)
{
    // FIXME:
    // Playback the corresponding animation of the gameState parameter
    // Once playback has finished, return true, otherwise return false
    return true;
}

eButtonState_t checkButton(void)
{
    // FIXME:
    // Polls the button and returns the press state
    return eBtnUndefined;
}

int app_main(void)
{
    eGameState_t  currentGameState = eGameRestart;
    eButtonState_t buttonState;
    sBoardState_t boardState;
    initIO();

    while(1)
    {   
        buttonState = checkButton();
        
        switch(currentGameState)
        {
            case eGameRestart:
                for (uint8_t idx = 0; idx < NUM_LED_PER_COLOR; idx++ )
                {
                    boardState.gameBoard[eRedLed][idx] = false;
                    boardState.gameBoard[eGreenLed][idx] = false;
                    boardState.cursor = 0;
                    boardState.currentColor = eRedLed;
                }
                currentGameState = eOngoingGame;
                break;
            case eOngoingGame:
                if (buttonState != eBtnUndefined)
                    currentGameState = checkBoard(&boardState, buttonState);
                displayBoard(&boardState);
                break;
            case eStalemate:
            case eRedPlayerWin:
            case eGreenPlayerWin:
                if (!playSequence(currentGameState))
                    currentGameState = eGameRestart;
            break;
        }
        delayMs(1);
        _millis++;
    }
}