/* xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
   JobDialogAppend.c
   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
*/
#include "Printing.h"

/*  global variables */

TPPrDlg		gTPrDlgStructurePtr;
SInt32		gFirstAppendedItemNo;
PDlgInitUPP 	gInitialisationFunctionUPP;
PItemUPP	gOldItemEvaluateFunctionUPP;
PItemUPP	gNewItemEvaluateFunctionUPP;
ModalFilterUPP	gEventFilterUPP;


/*   initialisationFunction */
#ifdef    FFFFF
pascal TPPrDlg  initialisationFunction(THPrint hPrint)
{
    ControlHandle	controlHdl;
    MenuHandle		menuHdl;
    SInt16		numberOfItems, a, fontNumber;
    Str255		fontName;
		
    /* append the DITL */

    doAppendTheDITL(gTPrDlgStructurePtr);

    /*  make pop-up menu WYSIWYG, 
	check second radio button, set fractional widths off */

    GetDialogItemAsControl((DialogPtr) gTPrDlgStructurePtr,
			   gFirstAppendedItemNo, &controlHdl);
#ifdef nomore
    GetControlData(controlHdl,kControlNoPart,
		   kControlPopupButtonMenuHandleTag,
		   sizeof(menuHdl),(Ptr) &menuHdl,NULL);
#endif
    numberOfItems = CountMenuItems(menuHdl);
    for(a=1; a<=numberOfItems; a++) {
	GetMenuItemText(menuHdl,a,fontName);
	GetFNum(fontName,&fontNumber);
	/* SetMenuItemFontID(menuHdl,a,fontNumber); */
    }

    GetDialogItemAsControl((DialogPtr) gTPrDlgStructurePtr,
			   gFirstAppendedItemNo + 2, &controlHdl);
    SetControlValue(controlHdl,1);

		
    GetDialogItemAsControl((DialogPtr) gTPrDlgStructurePtr,
			   gFirstAppendedItemNo + 4, &controlHdl);
    SetControlValue(controlHdl,0);
    SetFractEnable(false);

    /* save old evaluation function and assign new evaluation function */

    gOldItemEvaluateFunctionUPP = gTPrDlgStructurePtr->pItemProc;
    gTPrDlgStructurePtr->pItemProc = gNewItemEvaluateFunctionUPP;

    /* assign new event filter function */

    gTPrDlgStructurePtr->pFltrProc = gEventFilterUPP;

    /* PrDlgMain expects a pointer to the modified dialog to be returned */

    return gTPrDlgStructurePtr;
}
#endif


void  doAppendTheDITL(TPPrDlg theDialog)
{
    Handle	ditlHdl;
    SInt16	numberOfExistingItems;

    ditlHdl = GetResource('DITL',rJobDialogAppendDITL);
    numberOfExistingItems = CountDITL((DialogPtr) theDialog);
    AppendDITL((DialogPtr) theDialog,ditlHdl,appendDITLBottom);
    gFirstAppendedItemNo = numberOfExistingItems + 1;
}


#ifdef FFFFF
pascal void  itemEvaluationFunction(TPPrDlg theDialog,short itemHit)
{
    SInt16		localizedItemNo, controlValue;
    ControlHandle	controlHdl;
    MenuHandle		menuHdl;
    Str255		itemName;
	
    localizedItemNo = itemHit - gFirstAppendedItemNo + 1;

    if(localizedItemNo > 0) {
	if(localizedItemNo == iPopupButton) {
	    GetDialogItemAsControl((DialogPtr) theDialog,
				   gFirstAppendedItemNo,&controlHdl);
	    controlValue = GetControlValue(controlHdl);
	    GetControlData(controlHdl,kControlNoPart,
			   kControlPopupButtonMenuHandleTag,
			   sizeof(menuHdl),(Ptr) &menuHdl,NULL);
	    GetMenuItemText(menuHdl,controlValue,itemName);
	    /* GetFNum(itemName,&gFontNumber); */
	}
	else if(localizedItemNo >= iRadioButton10pt && 
		localizedItemNo <= iRadioButton14pt) {
	    GetDialogItemAsControl((DialogPtr)theDialog,gFirstAppendedItemNo +1,
				   &controlHdl);
	    SetControlValue(controlHdl,0);
	    GetDialogItemAsControl((DialogPtr)theDialog,gFirstAppendedItemNo +2,
				   &controlHdl);
	    SetControlValue(controlHdl,0);
	    GetDialogItemAsControl((DialogPtr)theDialog,gFirstAppendedItemNo +3,
				   &controlHdl);
	    SetControlValue(controlHdl,0);

	    GetDialogItemAsControl((DialogPtr) theDialog,itemHit,&controlHdl);
	    SetControlValue(controlHdl,1);
 		
	}
	else if(localizedItemNo == iCheckboxFracWidths) {
	    GetDialogItemAsControl((DialogPtr)theDialog,gFirstAppendedItemNo +4,
				   &controlHdl);
	    SetControlValue(controlHdl,!GetControlValue(controlHdl));
	    SetFractEnable(GetControlValue(controlHdl));
	}
    }
    else {
	CallPItemProc(gOldItemEvaluateFunctionUPP,
		      (DialogPtr) theDialog,itemHit);
    }
}
#endif
/* eventFilter */

pascal Boolean eventFilter(DialogPtr dialogPtr,EventRecord *eventStrucPtr,
			   SInt16 *itemHit)
{
    Boolean	handledEvent;
    GrafPtr	oldPort;

    handledEvent = false;

    if((eventStrucPtr->what == updateEvt) && 
       ((WindowPtr) eventStrucPtr->message != dialogPtr)) {
	doUpdate(eventStrucPtr);
    }
    else {
	GetPort(&oldPort);
	SetPort(dialogPtr);

	handledEvent = StdFilterProc(dialogPtr,eventStrucPtr,itemHit);

	SetPort(oldPort);
    }

    return(handledEvent);
}
