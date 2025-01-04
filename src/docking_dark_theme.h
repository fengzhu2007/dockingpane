#ifndef DOCKING_DARK_THEME_H
#define DOCKING_DARK_THEME_H

#include "docking_theme.h"

namespace ady{
class DockingDarkThemePrivate;
class DockingDarkTheme : public DockingTheme
{
public:
    virtual ~DockingDarkTheme() override;
    virtual QString name()override;
    virtual QColor color()override;//default color
    virtual QColor primaryColor()override;
    virtual QColor secondaryColor()override;
    virtual QColor backgroundColor()override;

    virtual QColor textColor()override;
    virtual QColor primaryTextColor()override;
    virtual QColor secondaryTextColor()override;

    virtual QColor borderColor()override;
    virtual QColor primaryBorderColor()override;

    virtual QColor tabColor() override;
    virtual QColor primaryTabColor() override;
    virtual QColor secondaryTabColor() override;

    virtual QColor clientTabColor() override;
    virtual QColor primaryClientTabColor() override;
    virtual QColor secondaryClientTabColor() override;

    virtual QColor buttonColor() override;
    virtual QColor primaryButtonColor() override;
    virtual QColor secondaryButtonColor() override;

    virtual QString* guides()override;//guide diamond image path array
    virtual QString guide(int position,bool state) override;
    virtual QString icon(int type,int state) override;

    virtual QString background(int type) override;

private:
    DockingDarkTheme();

private:
    DockingDarkThemePrivate* d;

    friend class DockingTheme;
};

}

#endif // DOCKING_DARK_THEME_H
