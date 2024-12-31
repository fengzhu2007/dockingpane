#include "docking_light_theme.h"
#include <QDebug>

namespace ady{
class DockingLightThemePrivate{
public:
    QColor color;
    QColor primaryColor;
    QColor secondaryColor;
    QColor backgroundColor;
    QColor textColor;
    QColor primaryTextColor;
    QColor secondaryTextColor;
    QColor borderColor;
    QColor primaryBorderColor;
    QColor secondaryClientTabColor;
    QColor secondaryTabColor;
    QColor primaryTabColor;
    QColor secondaryButtonColor;
    QColor primaryButtonColor;
    QString guides[26];
    QString icons[16];



    DockingLightThemePrivate():
        color("#EEEEF2"),
        primaryColor("#007acc"),
        secondaryColor("#c9def5"),
        backgroundColor(Qt::white),
        textColor(Qt::black),
        primaryTextColor(Qt::white),
        secondaryTextColor("#444"),
        borderColor("#ccc"),
        primaryBorderColor("#007acc"),
        secondaryClientTabColor("#1c97ea"),
        secondaryTabColor("#c9DCF5"),//c9DCF5
        primaryTabColor("#F5F5F5"),
        secondaryButtonColor("#e6e6e6"),
        primaryButtonColor("#52b0ef"),

        guides{
            ":/images/vs2019/dock_border_sider_left.png",":/images/vs2019/dock_border_sider_top.png",":/images/vs2019/dock_border_sider_right.png",
            ":/images/vs2019/dock_border_sider_bottom.png",":/images/vs2019/dock_sider_left.png",":/images/vs2019/dock_sider_top.png",
            ":/images/vs2019/dock_sider_right.png",":/images/vs2019/dock_sider_bottom.png",":/images/vs2019/dock_center.png",
            ":/images/vs2019/dock_left.png",":/images/vs2019/dock_top.png",":/images/vs2019/dock_right.png",
            ":/images/vs2019/dock_bottom.png",

            ":/images/vs2019/dock_border_sider_left_active.png",":/images/vs2019/dock_border_sider_top_active.png",":/images/vs2019/dock_border_sider_right_active.png",
            ":/images/vs2019/dock_border_sider_bottom_active.png",":/images/vs2019/dock_sider_left_active.png",":/images/vs2019/dock_sider_top_active.png",
            ":/images/vs2019/dock_sider_right_active.png",":/images/vs2019/dock_sider_bottom_active.png",":/images/vs2019/dock_center_active.png",
            ":/images/vs2019/dock_left_active.png",":/images/vs2019/dock_top_active.png",":/images/vs2019/dock_right_active.png",
            ":/images/vs2019/dock_bottom_active.png"},


        icons{":/images/vs2019/dock_drow_gray.png",":/images/vs2019/dock_pin_gray.png",":/images/vs2019/dock_pin_fixed_gray.png",
              ":/images/vs2019/dock_restore_gray.png",":/images/vs2019/dock_max_gray.png",":/images/vs2019/dock_close_gray.png",
              ":/images/vs2019/dock_head_gray.png",":/images/vs2019/dock_close_light_gray.png",


              ":/images/vs2019/dock_drow_white.png",":/images/vs2019/dock_pin_white.png",":/images/vs2019/dock_pin_fixed_white.png",
              ":/images/vs2019/dock_restore_white.png",":/images/vs2019/dock_max_white.png",":/images/vs2019/dock_close_white.png",
              ":/images/vs2019/dock_head_white.png",":/images/vs2019/dock_close_light_gray.png"}
    {


    }
};

DockingLightTheme::DockingLightTheme():DockingTheme() {
    d = new DockingLightThemePrivate;

}

DockingLightTheme::~DockingLightTheme(){
    delete d;
}


QString DockingLightTheme::name(){
    return QString::fromUtf8("Light");
}

//
QColor DockingLightTheme::color(){
    return d->color;
}

QColor DockingLightTheme::primaryColor(){
    return d->primaryColor;
}

QColor DockingLightTheme::secondaryColor(){
    return d->secondaryColor;
}

QColor DockingLightTheme::backgroundColor(){
    return d->backgroundColor;
}

QColor DockingLightTheme::textColor(){
    return d->textColor;
}

QColor DockingLightTheme::primaryTextColor(){
    return d->primaryTextColor;
}

QColor DockingLightTheme::secondaryTextColor(){
    return d->secondaryTextColor;
}

QColor DockingLightTheme::borderColor(){
    return d->borderColor;
}

QColor DockingLightTheme::primaryBorderColor(){
    return d->primaryBorderColor;
}

QColor DockingLightTheme::tabColor(){
    return color();
}

QColor DockingLightTheme::primaryTabColor(){
    return d->primaryTabColor;
}

QColor DockingLightTheme::secondaryTabColor(){
    return d->secondaryTabColor;
}

QColor DockingLightTheme::clientTabColor(){
    return color();
}

QColor DockingLightTheme::primaryClientTabColor(){
    return d->primaryColor;
}

QColor DockingLightTheme::secondaryClientTabColor(){
    //client tab hover
    return d->secondaryClientTabColor;
}


QColor DockingLightTheme::buttonColor(){
    return color();
}

QColor DockingLightTheme::primaryButtonColor(){
    return d->primaryButtonColor;
}

QColor DockingLightTheme::secondaryButtonColor(){
    //
    return d->secondaryButtonColor;
}

QString* DockingLightTheme::guides(){
    return d->guides;
}

QString DockingLightTheme::guide(int position,bool state){
    if(state){
        position += 13;
    }
    return d->guides[position];
}

QString DockingLightTheme::icon(int type,int state){
    if(state==DockingTheme::Active){
        type += 8;
    }
    return d->icons[type];
}

}
