/**
 * @file vibuttonctrl.c
 * @author Ilia Voronin (www.linkedin.com/in/ilia-voronin-7a169122a)
 * @brief Source file of of button control driver
 *
 * @copyright Copyright (c) 2024 Ilia Voronin
 * 
 * This software is licensed under GNU GENERAL PUBLIC LICENSE 
 * The terms can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS,
 * Without warranty of any kind, express or implied, 
 * including but not limited to the warranties of merchantability, 
 * fitness for a particular purpose and noninfringement. 
 * In no event shall the authors or copyright holders be liable for any claim, 
 * damages or other liability, whether in an action of contract, tort or otherwise, 
 * arising from, out of or in connection with the software 
 * or the use or other dealings in the software.
 * 
 */

#include "vibuttonctrl_private.h"
#include "string.h"

/**
 * @brief array of all buttons
 * 
 */
static struct {
  Button_t bt[VIBTCTR_MAX_BUTTONS];              /*!< array of all buttons */
  uint32_t currentBt;                            /*!< current handled button */
  uint32_t initializedNumber;                    /*!< number of initialized buttons in array*/
  VIBTCTR_getPortState_t getPortSateCb;          /*!< call-back funktion to get input port state*/
  VIBTCTR_getTick_t getTickCb;                   /*!< call-back funktion to get system tick*/
  VIBTCTR_IsTickOverflowed_t isTickOverflowedCb; /*!< call-back funktion to define if tick was overflowed */
  VIBTCTR_Printf_t printfCb;                     /*!< printf callback */
  VIBTCTR_DebugLvl_t debugLvl;                   /*!< debug level */
} buttons;

bool VIBTCTR_Init(VIBTCTR_getPortState_t getPortStateCb,
  VIBTCTR_getTick_t getTickCb, VIBTCTR_IsTickOverflowed_t isTickOverflowedCb) {

  if(NULL == getPortStateCb || NULL == getTickCb ||
     NULL == isTickOverflowedCb)
    return false;
  
  buttons.getPortSateCb = getPortStateCb;
  buttons.getTickCb = getTickCb;
  buttons.isTickOverflowedCb = isTickOverflowedCb;
  buttons.currentBt = 0;
  buttons.initializedNumber = 0;

  for(uint32_t i = 0; i < VIBTCTR_MAX_BUTTONS; i++) {
    Init(&buttons.bt[i], NULL, NULL, 0);
  }
  return true;
}

bool VIBTCTR_Create(const char* name, void* port, uint16_t pin) {

  if(NULL == name || NULL == port)
    return false;

  if(VIBTCTR_MAX_BUTTONS <= buttons.initializedNumber)
    return false;

  int32_t buttonIndex = FindButtonByName(name);

  if(0 <= buttonIndex)
    return false;

  if(Init(&buttons.bt[buttons.initializedNumber], name, port, pin)) {
    buttons.initializedNumber++;
    return true;
  }
  return true;
}

void VIBTCTR_Runtime(void) {

  if(0 < buttons.initializedNumber)
    Runtime(&buttons.bt[buttons.currentBt]);

  buttons.currentBt++;

  if(buttons.currentBt >= buttons.initializedNumber) {
    buttons.currentBt = 0;
  }
}

bool VIBTCTR_IsPressed(const char* name) {
  if(NULL == name )
    return false;

  int32_t buttonIndex = FindButtonByName(name);

  if(0 <= buttonIndex) {
    if(BUTTON_PRESSED == buttons.bt[buttonIndex].state)
      return true;
  }
  return false;
}

bool VIBTCTR_IsReleased(const char* name) {
  if(NULL == name )
    return false;

  int32_t buttonIndex = FindButtonByName(name);

  if(0 <= buttonIndex) {
    if(BUTTON_RELEASED == buttons.bt[buttonIndex].state)
      return true;
  }
  return false;
}

void VIBTCTR_SetPrintfCb(VIBTCTR_Printf_t printfCb) {
  buttons.printfCb = printfCb;
}

void VIBTCTR_SetDebugLvl(VIBTCTR_DebugLvl_t lvl) {
  buttons.debugLvl = lvl;
}

static bool Init(Button_t* button, const char* name, 
  void* port, uint16_t pin) {
    
  if(NULL == button)
    return false;

  if(NULL == name)
    memset(button->name, 0, VIBTCTR_NAME_LEN);
  else
    strncpy(button->name, name, VIBTCTR_NAME_LEN);

  button->port = port;
  button->pin = pin;
  button->risingTime = 0;
  button->failTime = 0;

  if(NULL != button->port && 0 != button->pin && 
     buttons.getPortSateCb(button->port, button->pin))
    button->state = BUTTON_PRESSED;
  else
    button->state = BUTTON_RELEASED;

  if(VIBTCTR_DEBUG_INFO <= buttons.debugLvl && 
     NULL != buttons.printfCb)
    buttons.printfCb("Button \"%s\" is initialized\r\n");

  return true;
}

static int32_t FindButtonByName(const char* name) {

  int32_t result = -1;

  if(NULL == name)
    return result;

  for(uint32_t i = 0; ((i < VIBTCTR_MAX_BUTTONS) && 
      (i < buttons.initializedNumber)); i++) {
    if(0 == strncmp(buttons.bt[i].name, name, VIBTCTR_NAME_LEN)) {
      result = i;
    }
  }
  return result;
}

static void Runtime(Button_t* button) {

  if(NULL != buttons.getPortSateCb && NULL != buttons.getTickCb &&
     NULL != buttons.isTickOverflowedCb) {
    uint32_t currentTimestamp = buttons.getTickCb();
    // if physical pin is on
    if(buttons.getPortSateCb(button->port, button->pin)) {
      switch(button->state) {
        case BUTTON_RELEASED:
          if(VIBTCTR_DEBUG_LVL1 <= buttons.debugLvl && 
             NULL != buttons.printfCb)
            buttons.printfCb("Start to press button \"%s\", currentTimestamp: %d\r\n", 
              button->name, currentTimestamp);
          button->state = BUTTON_SWITCHING_ON;
          button->risingTime = currentTimestamp;
          break;
        case BUTTON_PRESSED:
          break;
        case BUTTON_SWITCHING_ON:
          if(buttons.isTickOverflowedCb()) {
            if(UINT32_MAX - button->risingTime - STATE_DELAY <= currentTimestamp){
              if(VIBTCTR_DEBUG_LVL1 <= buttons.debugLvl && 
                 NULL != buttons.printfCb)
                buttons.printfCb("Button \"%s\" is pressed, currentTimestamp: %d\r\n", 
                  button->name, currentTimestamp);
              else if(VIBTCTR_DEBUG_INFO <= buttons.debugLvl && 
                      NULL != buttons.printfCb)
                buttons.printfCb("Button \"%s\" is pressed\r\n", button->name);

              button->state = BUTTON_PRESSED;
            }

          } else {
            if(STATE_DELAY <= currentTimestamp - button->risingTime) {
              if(VIBTCTR_DEBUG_LVL1 <= buttons.debugLvl && 
                 NULL != buttons.printfCb)
                buttons.printfCb("Button \"%s\" is pressed, currentTimestamp: %d\r\n", 
                  button->name, currentTimestamp);
              else if(VIBTCTR_DEBUG_INFO <= buttons.debugLvl && 
                      NULL != buttons.printfCb)
                buttons.printfCb("Button \"%s\" is pressed\r\n", button->name);
              button->state = BUTTON_PRESSED;
            }
          }
          break;
        case BUTTON_SWITCHING_OFF:
          button->state = BUTTON_PRESSED;
          button->failTime = currentTimestamp;
          break;
      }
    // if physical pin is off
    } else {
      switch(button->state) {
        case BUTTON_RELEASED:
          break;
        case BUTTON_PRESSED:
          if(VIBTCTR_DEBUG_LVL1 <= buttons.debugLvl && 
             NULL != buttons.printfCb)
            buttons.printfCb("Start to release button \"%s\", currentTimestamp: %d\r\n", 
              button->name, currentTimestamp);
          button->state = BUTTON_SWITCHING_OFF;
          button->failTime = currentTimestamp;
          break;
        case BUTTON_SWITCHING_ON:
          button->state = BUTTON_RELEASED;
          button->failTime = currentTimestamp;
          break;
        case BUTTON_SWITCHING_OFF:
          if(buttons.isTickOverflowedCb()) {
            if(UINT32_MAX - button->failTime - STATE_DELAY <= currentTimestamp) {
              if(VIBTCTR_DEBUG_LVL1 <= buttons.debugLvl && 
                 NULL != buttons.printfCb)
                buttons.printfCb("Button \"%s\" is released, currentTimestamp: %d\r\n", 
                  button->name, currentTimestamp);
              else if(VIBTCTR_DEBUG_INFO <= buttons.debugLvl && NULL != buttons.printfCb)
                buttons.printfCb("Button \"%s\" is released\r\n", button->name);
              button->state = BUTTON_RELEASED;
            }
          } else {
            if(STATE_DELAY <= currentTimestamp - button->failTime) {
              if(VIBTCTR_DEBUG_LVL1 <= buttons.debugLvl && 
                 NULL != buttons.printfCb)
                buttons.printfCb("Button \"%s\" is released, currentTimestamp: %d\r\n", 
                  button->name, currentTimestamp);
              else if(VIBTCTR_DEBUG_INFO <= buttons.debugLvl && NULL != buttons.printfCb)
                buttons.printfCb("Button \"%s\" is released\r\n", button->name);
              button->state = BUTTON_RELEASED;
            }

          }
          break;
      }
    }
  }
}
