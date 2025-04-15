#ifndef PANEL_H
#define PANEL_H

#pragma once

#include "entities/element.h"
#include "entities/ui_element.h"
#include <cstring>

struct Text;

struct UIPanelTitlebar :
    public UIElement
{
    UIPanelTitlebar( const char *title, float x, float y, float scale, float depth, const shared_ptr<Window> &container );

    const string &panel_title = m_panel_title;

private:
    string m_panel_title;
};
struct UIPanelBody :
    public UIElement
{

};

/*
struct UIPanel :
    public UIElement
{
    UIPanel( const char *title, float scale,  )
}
*/

#endif