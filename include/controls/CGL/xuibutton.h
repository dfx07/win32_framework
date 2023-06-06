////////////////////////////////////////////////////////////////////////////////////
/*!*********************************************************************************
* @Copyright (C) 2021-2022 thuong.nv <thuong.nv.mta@gmail.com>
*            All rights reserved.
************************************************************************************
* @file     xuibuttom.h
* @create   june 15, 2023
* @brief    Create property panel
* @note     For conditions of distribution and use, see copyright notice in readme.txt
************************************************************************************/
#ifndef XUIBUTTOM_H
#define XUIBUTTOM_H

#include "xuicontrol.h"
#include "UIFrame/xUIButton.h"
#include "controls/xbuttonbase.h"

___BEGIN_NAMESPACE___

____BEGIN_SECTION____(gui)

class Dllexport UIButton : public UIControlElement, public XButtonBase
{

};

_____END_SECTION_____

____END_NAMESPACE____

#endif // XUIBUTTOM_H