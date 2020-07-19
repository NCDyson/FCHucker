#include "windowstack.h"
#include <tamtypes.h>
#include <kernel.h>
#include "controller.h"
#include <libpad.h>
#include "libfchucker.h"
#include "math.h"

static void (*WINDOWFUNCS[WINDOW_STACK_SIZE])(int isTopmost) = {0};
static int WindowCount = 0;



void PushWindow(void (*func)(int))
{
    if(WindowCount < (WINDOW_STACK_SIZE - 1))
    {
        WINDOWFUNCS[WindowCount] = func;
        WindowCount++;
    }
}

void PopWindow()
{
    WINDOWFUNCS[WindowCount - 1] = NULL;
    WindowCount--;
}

void DrawWindows()
{
    //First, Draw Help Text
    DrawHelpText(gsGlobal);
    int i = 0;
    for(i = 0; i < (WindowCount); i++)
    {
        void (*f)(int) = WINDOWFUNCS[i];
        //printf("DRAWING WINDOW %08X\n", f);
        (*f)(i == (WindowCount - 1));
    }
}

void DrawCharSelectionScreen_Input()
{
    u32 padd = GetPad();
    if(padd & PAD_DOWN)
    {
        CharacterSelectionIndex = CharacterSelect_MoveCursor(CharacterSelectionIndex, SELECTION_MOVE_DIRECTION_DOWN);
    }
    else if(padd & PAD_UP)
    {
        CharacterSelectionIndex = CharacterSelect_MoveCursor(CharacterSelectionIndex, SELECTION_MOVE_DIRECTION_UP);
    }

    if(padd & PAD_LEFT)
    {
        CharacterSelectionIndex = CharacterSelect_MoveCursor(CharacterSelectionIndex, SELECTION_MOVE_DIRECTION_LEFT);
    }
    else if(padd & PAD_RIGHT)
    {
        CharacterSelectionIndex = CharacterSelect_MoveCursor(CharacterSelectionIndex, SELECTION_MOVE_DIRECTION_RIGHT);
    }

    if(padd & PAD_CROSS)
    {
        ValidateCharacterSelection();
    }

    if(padd & PAD_SQUARE)
    {
        RefreshCharacters(gsGlobal);
    }

    if(padd & PAD_START)
    {
        int elk = 0;
        if(SourceMCSlot == DestMCSlot)
        {
            if(SourceCharacterSlot == DestCharacterSlot)
            {
                elk = 1;
            }
        }

        if(elk)
        {
            PushWindow(&DrawSourceDestSame_Window);
        }
        else
        {
            PopWindow();
            GoInventoryView();
        }
    }
}
void DrawCharSelectScreen_Window(int topMost)
{
    if(topMost)
    {
        SetHelpText(HELP_TEXT_CHARACTER_SELECT_SELECT);
        DrawCharSelectionScreen_Input();
    }
    DrawCharacterSelection();
}

void DrawMC0AccessDialog_Window(int topMost)
{
    UNUSED(topMost);
    DrawMCIODialog(0);
}

void DrawMC1AccessDialog_Window(int topMost)
{
    UNUSED(topMost);
    DrawMCIODialog(0);
}

void DrawSourceDestSame_Window(int topMost)
{
    if(topMost)
    {
        u32 btns = GetPad();
        if(btns & PAD_CROSS)
        {

            PopWindow();
        }
    }
    DrawCharacterDialog((unsigned char *)"Elk", (unsigned char *)"Um... okay.");
}


void DrawInventory_Input()
{
    u32 btns = GetPad();
    InventoryWindow_t * currentWindow = &InventoryWindows[CurrentInventoryWindow];

    if(btns & PAD_SELECT)
    {
        if(InventoriesDirty)
        {
            PushWindow(&DrawDiscardConfirmation_Window);
            return;
        }
        else
        {
            GoBackToCharacterSelect();
        }
    }

    if(btns & PAD_START)
    {
        SaveAndGoBackToCharacterSelect();
    }

    if(btns & PAD_L1)
    {
        currentWindow->CurrentTransferAmount = 0;
        currentWindow->CurrentItemRow = -1;
        if(currentWindow->CurrentPage == 0) currentWindow->CurrentPage = 7;
        else currentWindow->CurrentPage -= 1;
    }
    if(btns & PAD_R1)
    {
        currentWindow->CurrentTransferAmount = 0;
        currentWindow->CurrentItemRow = -1;
        if(currentWindow->CurrentPage == 7) currentWindow->CurrentPage = 0;
        else currentWindow->CurrentPage += 1;
    }


    if(btns & PAD_CROSS)
    {
        if(currentWindow->CurrentTransferAmount > 0 && currentWindow->CurrentItemRow != -1)
        {
            DoItemTransfer();
        }
    }

    if(btns & PAD_SQUARE)
    {
        CurrentInventoryWindow ^= 1;
    }

    int currentPage = currentWindow->CurrentPage;
    int * CurrentPageIndex = &currentWindow->Page0Index + currentPage;
    int * CurrentPageCount = &currentWindow->Page0Count + currentPage;
    int * CurrentScrollStep = &currentWindow->Page0ScrollStep + currentPage;
    if(btns & PAD_DOWN)
    {
        currentWindow->CurrentTransferAmount = 0;
        if(*CurrentPageCount > 0)
        {
            if(*CurrentPageIndex == (*CurrentPageCount - 1))
            {
                *CurrentPageIndex = 0;
                *CurrentScrollStep = 0;
            }
            else
            {
                *CurrentPageIndex += 1;
                if(*CurrentPageIndex >= (currentWindow->VisibleItemCount + *CurrentScrollStep))
                {
                    *CurrentScrollStep += 1;
                }
            }
        }
    }

    if(btns & PAD_TRIANGLE)
    {
        PushWindow(&DrawTransferGP_Window);
    }

    if(btns & PAD_UP)
    {
        currentWindow->CurrentTransferAmount = 0;
        if(*CurrentPageCount > 0)
        {
            if(*CurrentPageIndex == 0)
            {
                *CurrentPageIndex = (*CurrentPageCount - 1);
                if(*CurrentPageCount <= currentWindow->VisibleItemCount) *CurrentScrollStep = 0;
                else *CurrentScrollStep = (*CurrentPageCount - currentWindow->VisibleItemCount);
            }
            else
            {
                *CurrentPageIndex -= 1;
                if(*CurrentPageIndex < *CurrentScrollStep)
                {
                    *CurrentScrollStep -= 1;
                }
            }
        }
    }

    InventoryItem_t * t = &currentWindow->Inventory[currentWindow->CurrentItemRow];
    if(btns & PAD_LEFT)
    {
        if(currentWindow->CurrentItemRow != -1)
        {
            if(currentWindow->CurrentTransferAmount <= 0) currentWindow->CurrentTransferAmount = t->Count;
            else currentWindow->CurrentTransferAmount -= 1;
        }
    }

    if(btns & PAD_RIGHT)
    {
        if(currentWindow->CurrentItemRow != -1)
        {
            if(currentWindow->CurrentTransferAmount == t->Count) currentWindow->CurrentTransferAmount = 0;
            else currentWindow->CurrentTransferAmount += 1;
        }
    }

}

void DrawInventory_Window(int topMost)
{
    if(topMost)
    {
        SetHelpText(HELP_TEXT_SELECT_ITEMS);
        DrawInventory_Input();
    }
    DrawInventoryWindow(CHARACTER_SRC, 20, 112, INVENTORY_WINDOW_WIDTH, INVENTORY_WINDOW_HEIGHT);
    DrawInventoryWindow(CHARACTER_DEST, 320, 112, INVENTORY_WINDOW_WIDTH, INVENTORY_WINDOW_HEIGHT);

    if(CurrentInventoryWindow == 0) DrawNameWindow(CHARACTER_SRC, 26, 16);
    else DrawNameWindow(CHARACTER_DEST, 320, 16);
}


void DrawTransferResult_Input()
{
    u32 btns = GetPad();
    if(btns & PAD_CROSS)
    {
        CloseTransferResultWindow();
    }
}

void DrawTransferResult_Window(int topMost)
{
    if(topMost)
    {
        SetHelpText(HELP_TEXT_TRANSFER_ITEMS_RESULT);
        DrawTransferResult_Input();
    }
    DrawInventoryTransferResult();
}

void DrawDiscardConfirmation_Input()
{
    u32 btns = GetPad();
    if(btns & PAD_LEFT || btns & PAD_RIGHT) DiscardCurrentSelection ^= 1;

    if(btns & PAD_CROSS)
    {
        //This is backwards, by the way.
        if(DiscardCurrentSelection)
        {
            PopWindow();
        }
        else
        {
            PopWindow();
            GoBackToCharacterSelect();
        }
    }
}

void DrawDiscardConfirmation_Window(int topMost)
{
    if(topMost)
    {
        DrawDiscardConfirmation_Input();
    }
    DrawDiscardConfirmationDialog();
}


void DrawTransferGP_Input()
{
    u32 btns = GetPad();

    if(btns & PAD_CROSS)
    {
        DoGPTransfer();
    }

    if(btns & PAD_CIRCLE)
    {
        PopWindow();
        GPDigit = 0;
        GPTransferAmount = 0;
    }

    int currentGP = SourceGP;
    if(CurrentInventoryWindow)
    {
        currentGP = DestGP;
    }

    if(btns & PAD_LEFT)
    {
        if(GPDigit < 7) GPDigit += 1;
    }

    if(btns & PAD_RIGHT)
    {
        GPDigit -= 1;
        if(GPDigit < 0) GPDigit = 0;
    }

    if(btns & PAD_UP)
    {
        int addAmount = (int)pow(10, GPDigit);
        GPTransferAmount += addAmount;

        if(GPTransferAmount > currentGP) GPTransferAmount = currentGP;
    }

    if(btns & PAD_DOWN)
    {
        int subAmount = (int)pow(10, GPDigit);
        GPTransferAmount -= subAmount;

        if(GPTransferAmount < 0) GPTransferAmount = 0;
    }
}

void DrawTransferGP_Window(int topMost)
{
    if(topMost)
    {
        SetHelpText(HELP_TEXT_TRANSFER_GP);
        DrawTransferGP_Input();
    }
    DrawGPTransferDialog();
}
