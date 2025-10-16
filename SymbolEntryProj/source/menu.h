#ifndef MENU_H
#define MENU_H

#include "MCUType.h"   /* For INT8U type */

#define SYMBOL_COUNT 5

/* Updates the menu display based on the selected index */
void UpdateMenu(INT8U selected_index);

/* Returns the current symbol index */
INT8U GetCurrentSymbolIndex(void);

/* Sets a new symbol index and updates the menu */
void SetCurrentSymbolIndex(INT8U index);

/* Returns the current symbol as a character */
const INT8C* GetCurrentSymbol(void);

#endif
