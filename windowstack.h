#ifndef WINDOWSTACK_H
#define WINDOWSTACK_H
#include <gsKit.h>
#include "ui.h"
#define WINDOW_STACK_SIZE 16



/**
 * @brief PushWindow
 * @param func Window drawing function should be func(GSGLOBAL * gsGlobal)
 * @param int Topmost is window topmost and should consume input.
 *
 * Adds a window to the top of the stack.
 */
void PushWindow( void (*func)(int));

/**
 * @brief PopWindow
 * Removes the top most window from the stack
 */
void PopWindow();

/**
 * @brief DrawWindows
 * @param gsGlobal gsKit global variable
 * Okay, so stack is a horrible term for it because we draw from top down,
 * but only the top most window will actually handle input.
 */
void DrawWindows();

/* Window Draw Funcs */
void DrawCharSelectionScreen_Input();
void DrawCharSelectScreen_Window(int topMost);

void DrawMC0AccessDialog_Window(int topMost);
void DrawMC1AccessDialog_Window(int topMost);
void DrawMCAccesDialog_Window(int topMost);

void DrawSourceDestSame_Window(int topMost);

void DrawInventory_Input();
void DrawInventory_Window(int topMost);


void DrawTransferResult_Input();
void DrawTransferResult_Window(int topMost);

void DrawDiscardConfirmation_Window(int topMost);


void DrawTransferGP_Window(int topMost);
#endif // WINDOWSTACK_H
