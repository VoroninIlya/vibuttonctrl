/**
 * @file vibuttonctrl.h
 * @author Ilia Voronin (www.linkedin.com/in/ilia-voronin-7a169122a)
 * @brief Header file of button control driver
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

#ifndef VIBUTTCTRL_H
#define VIBUTTCTRL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/** 
 * @brief Maximal length of a button name.
 *   All names longer this limit will be cutted until this limit
 * */ 
#if !defined(VIBTCTR_NAME_LEN)
  #define VIBTCTR_NAME_LEN 15     
#endif

/** 
 * @brief Maximal number of allowed to create buttons.
 *   For this number of buttons will be static memory reserved 
 * */
#if !defined(VIBTCTR_MAX_BUTTONS)
  #define VIBTCTR_MAX_BUTTONS 2   
#endif

/**
 * @brief Debug level
 * 
 */
typedef enum {
  VIBTCTR_DEBUG_DISABLED = 0,  
  VIBTCTR_DEBUG_INFO,
  VIBTCTR_DEBUG_LVL1,
} VIBTCTR_DebugLvl_t;

typedef uint8_t (*VIBTCTR_getPortState_t) (void* gpio, uint16_t port);
typedef uint32_t (*VIBTCTR_getTick_t)(void);
typedef uint8_t (*VIBTCTR_IsTickOverflowed_t)(void);
typedef int (*VIBTCTR_Printf_t) (const char *__format, ...);

/**
 * @brief Initialization of buttons control driver
 * 
 * @param getPortStateCb Call-back funktion to get input port state. Port should be configured accordinately
 * @param getTickCb Call-back funktion to get system tick, (for Example: HAL_GetTick)
 * @param isTickOverflowedCb Call-back funktion to define if tick was overflowed
 * @return true if driver initialized successfull
 * @return false if any error occurred through initialization
 */
bool VIBTCTR_Init(
  VIBTCTR_getPortState_t getPortStateCb,
  VIBTCTR_getTick_t getTickCb, 
  VIBTCTR_IsTickOverflowed_t isTickOverflowedCb);

/**
 * @brief Create/register a new button handler
 * 
 * @param name Unique name of button. Care about VIBTCTR_NAME_LEN
 * @param port Pointer to GPIO structur
 * @param pin A pin number, to which is button connected
 * @return true if button created successfull
 * @return false if any error occurred through creation
 */
bool VIBTCTR_Create(const char* name, void* port, uint16_t pin);

/**
 * @brief Driver runtime, should be placed in main loop
 * 
 */
void VIBTCTR_Runtime(void);

/**
 * @brief Define if button is pressed
 * 
 * @param name Unique name of button.
 * @return true if button is pressed
 * @return false if button not pressed
 */
bool VIBTCTR_IsPressed(const char* name);

/**
 * @brief Define if button is released
 * 
 * @param name Unique name of button.
 * @return true if button is released
 * @return false if button not released
 */
bool VIBTCTR_IsReleased(const char* name);

/**
 * @brief Set printf callback.
 *   This callback can be used to get debug info from driver
 * 
 * @param printfCb Callback of prontf fuktion
 */
void VIBTCTR_SetPrintfCb(VIBTCTR_Printf_t printfCb);

/**
 * @brief Set debug info level
 * 
 * @param lvl Debug info level
 */
void VIBTCTR_SetDebugLvl(VIBTCTR_DebugLvl_t lvl);

#endif // VIBUTTCTRL_H