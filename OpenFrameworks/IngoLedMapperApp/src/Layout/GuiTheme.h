/*
 *  GuiTheme.h
 *  IngoLedMapperApp
 *
 *  Created by Imanol Gomez on 13/08/18.
 *
 */



#pragma once

#include "ofxDatGuiTheme.h"

//========================== class GuiTheme ==============================
//============================================================================
/** \class GuiTheme GuiTheme.h
 *	\brief Costume GUI theme
 *	\details Based on ofxDatGuiTheme
 */


class GuiTheme : public ofxDatGuiTheme{
    
public:
    
    GuiTheme()
    {
        layout.width = 300;
        layout.upperCaseLabels = false;
        //font.file =  AssetPath + "fonts/open-sans/OpenSans-Regular.ttf";
        font.file = LayoutManager::LAYOUT_FONT;
        font.size = 5;
        color.matrix.normal.button = hex(0x6E6E6E);
        color.matrix.hover.button = hex(0x9C9DA1);
        color.matrix.selected.button = hex(0x2FA1D6);
        
        init();
    }
};
