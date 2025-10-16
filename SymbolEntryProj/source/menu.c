/* menu.c */
#include "menu.h"
#include "MCUType.h"     /* Defines MCU-specific data types */
#include "BasicIO.h"     /* Basic IO functions for console output */

/* Symbol table: ordered list of selectable symbols */
static const INT8C *symbols[SYMBOL_COUNT] = {"π", "∑", "µ", "Ω", "∫"};

/* Tracks which symbol is currently active */
static INT8U current_symbol_index = 0;  /* Default to first symbol */

/*****************************************************************************************
* UpdateMenu()
*   Prints the symbol library and highlights the selected symbol.
*****************************************************************************************/
void UpdateMenu(INT8U selected_index) {
    BIOPutStrg("\033[1;1H\033[KSymbol Library: "); /* Print this on line 1 */
    for (INT8U i = 0; i < SYMBOL_COUNT; i++) {
        if (i == selected_index) {
            BIOPutStrg("[");
            BIOPutStrg(symbols[i]);
            BIOPutStrg("] ");
        } else {
            BIOPutStrg(symbols[i]);
            BIOPutStrg(" ");
        }
    }
}


/*****************************************************************************************
* GetCurrentSymbolIndex()
*   Returns the currently selected symbol index.
*****************************************************************************************/
INT8U GetCurrentSymbolIndex(void) {
    return current_symbol_index;
}

/*****************************************************************************************
* SetCurrentSymbolIndex()
*   Updates the current symbol index (with bounds checking) and refreshes the menu.
*****************************************************************************************/
void SetCurrentSymbolIndex(INT8U index) {
    if (index < SYMBOL_COUNT) { /* Extra layer of protection */
        current_symbol_index = index; /* Match the current symbol to the index */
        UpdateMenu(current_symbol_index);
    }
}

/*****************************************************************************************
* GetCurrentSymbol()
*   Finds the current symbol to print to the terminal (not the index)
*****************************************************************************************/
const INT8C* GetCurrentSymbol(void) {
    return symbols[current_symbol_index];
}
