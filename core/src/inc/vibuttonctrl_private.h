/**
 * @file vibuttonctrl_private.h
 * @author Ilia Voronin (www.linkedin.com/in/ilia-voronin-7a169122a)
 * @brief Private header file of button control driver
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

#ifndef VIBUTTCTRL_PRIVATE_H
#define VIBUTTCTRL_PRIVATE_H

#include "vibuttonctrl.h"

#define STATE_DELAY 50

/**
 * @brief Button state
 * 
 */
typedef enum {
  BUTTON_RELEASED = 0,
  BUTTON_PRESSED,
  BUTTON_SWITCHING_ON,
  BUTTON_SWITCHING_OFF,
} ButtonState_t;

/**
 * @brief Button control type
 * 
 */
typedef struct
{
  char name[VIBTCTR_NAME_LEN]; /*!< name of button */
  void* port;                  /*!< pointer to the physical port, to witch the button is connected*/
  uint16_t pin;                /*!< pin number, to witch the button is connected*/
  ButtonState_t state;         /*!< button logical state*/
  uint32_t risingTime;         /*!< rising anti-rattle filter*/
  uint32_t failTime;           /*!< falling anti-rattle filter*/
} Button_t;

/**
 * @brief Initialize a button handler
 * 
 * @param button Pointer to a button control structur
 * @param name Unique name of a new button
 * @param port Pointer to a GPIO structur
 * @param pin A pin number, to which is button connected
 * @return true if initialization is successful
 * @return false if initialization is failed
 */
static bool Init(Button_t* button, const char* name, 
  void* port, uint16_t pin);

/**
 * @brief Find button by name in array of initialized buttons
 * 
 * @param name Button name to search
 * @return int32_t index of found button, if no button found returns -1
 */
static int32_t FindButtonByName(const char* name);

/**
 * @brief Button runtime
 * 
 * @param button Pointer to a button control structur
 */
static void Runtime(Button_t* button);

#endif // VIBUTTCTRL_PRIVATE_H
