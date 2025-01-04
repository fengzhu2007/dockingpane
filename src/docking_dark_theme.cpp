#include "docking_dark_theme.h"
#include <QDebug>

namespace ady{
class DockingDarkThemePrivate{
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
    QString backgrounds[2];



    DockingDarkThemePrivate():
        color("#2d2d30"),
        primaryColor("#007acc"),
        secondaryColor("#c9def5"),
        backgroundColor(Qt::white),
        textColor(Qt::white),
        primaryTextColor(Qt::white),
        secondaryTextColor(Qt::white),
        borderColor("#494949"),
        primaryBorderColor("#007acc"),
        secondaryClientTabColor("#1c97ea"),
        secondaryTabColor("#494949"),//c9DCF5
        primaryTabColor("#252526"),
        secondaryButtonColor("#494949"),
        primaryButtonColor("#52b0ef"),

        guides{
               ":/images/vs2019dark/dock_border_sider_left.png",":/images/vs2019dark/dock_border_sider_top.png",":/images/vs2019dark/dock_border_sider_right.png",
               ":/images/vs2019dark/dock_border_sider_bottom.png",":/images/vs2019dark/dock_sider_left.png",":/images/vs2019dark/dock_sider_top.png",
               ":/images/vs2019dark/dock_sider_right.png",":/images/vs2019dark/dock_sider_bottom.png",":/images/vs2019dark/dock_center.png",
               ":/images/vs2019dark/dock_left.png",":/images/vs2019dark/dock_top.png",":/images/vs2019dark/dock_right.png",
               ":/images/vs2019dark/dock_bottom.png",

               ":/images/vs2019dark/dock_border_sider_left_active.png",":/images/vs2019dark/dock_border_sider_top_active.png",":/images/vs2019dark/dock_border_sider_right_active.png",
               ":/images/vs2019dark/dock_border_sider_bottom_active.png",":/images/vs2019dark/dock_sider_left_active.png",":/images/vs2019dark/dock_sider_top_active.png",
               ":/images/vs2019dark/dock_sider_right_active.png",":/images/vs2019dark/dock_sider_bottom_active.png",":/images/vs2019dark/dock_center_active.png",
               ":/images/vs2019dark/dock_left_active.png",":/images/vs2019dark/dock_top_active.png",":/images/vs2019dark/dock_right_active.png",
               ":/images/vs2019dark/dock_bottom_active.png"


        },


        icons{":/images/vs2019/dock_drow_white.png",":/images/vs2019/dock_pin_white.png",":/images/vs2019/dock_pin_fixed_white.png",
              ":/images/vs2019/dock_restore_white.png",":/images/vs2019/dock_max_white.png",":/images/vs2019/dock_close_white.png",
              ":/images/vs2019/dock_head_white.png",":/images/vs2019/dock_close_light_gray.png",


              ":/images/vs2019/dock_drow_white.png",":/images/vs2019/dock_pin_white.png",":/images/vs2019/dock_pin_fixed_white.png",
              ":/images/vs2019/dock_restore_white.png",":/images/vs2019/dock_max_white.png",":/images/vs2019/dock_close_white.png",
              ":/images/vs2019/dock_head_white.png",":/images/vs2019/dock_close_light_gray.png"},
        backgrounds{":/images/vs2019dark/docking.png",":/images/vs2019dark/docking_all.png"}
    {


    }
};

DockingDarkTheme::DockingDarkTheme():DockingTheme() {
    d = new DockingDarkThemePrivate;

}

DockingDarkTheme::~DockingDarkTheme(){
    delete d;
}


QString DockingDarkTheme::name(){
    return QString::fromUtf8("Dark");
}

//
QColor DockingDarkTheme::color(){
    return d->color;
}

QColor DockingDarkTheme::primaryColor(){
    return d->primaryColor;
}

QColor DockingDarkTheme::secondaryColor(){
    return d->secondaryColor;
}

QColor DockingDarkTheme::backgroundColor(){
    return d->backgroundColor;
}

QColor DockingDarkTheme::textColor(){
    return d->textColor;
}

QColor DockingDarkTheme::primaryTextColor(){
    return d->primaryTextColor;
}

QColor DockingDarkTheme::secondaryTextColor(){
    return d->secondaryTextColor;
}

QColor DockingDarkTheme::borderColor(){
    return d->borderColor;
}

QColor DockingDarkTheme::primaryBorderColor(){
    return d->primaryBorderColor;
}

QColor DockingDarkTheme::tabColor(){
    return color();
}

QColor DockingDarkTheme::primaryTabColor(){
    return d->primaryTabColor;
}

QColor DockingDarkTheme::secondaryTabColor(){
    return d->secondaryTabColor;
}

QColor DockingDarkTheme::clientTabColor(){
    return Qt::white;
}

QColor DockingDarkTheme::primaryClientTabColor(){
    return d->primaryColor;
}

QColor DockingDarkTheme::secondaryClientTabColor(){
    //client tab hover
    return d->secondaryClientTabColor;
}


QColor DockingDarkTheme::buttonColor(){
    return color();
}

QColor DockingDarkTheme::primaryButtonColor(){
    return d->primaryButtonColor;
}

QColor DockingDarkTheme::secondaryButtonColor(){
    //
    return d->secondaryButtonColor;
}

QString* DockingDarkTheme::guides(){
    return d->guides;
}

QString DockingDarkTheme::guide(int position,bool state){

    if(state){
        position += 13;
    }
    //qDebug()<<"guide"<<position<<state<<d->guides[position];
    return d->guides[position];
}

QString DockingDarkTheme::icon(int type,int state){
    if(state==DockingTheme::Active){
        type += 8;
    }
    return d->icons[type];
}

QString DockingDarkTheme::background(int type){
    if(type==0){
        //default
        return d->backgrounds[0];
    }else{
        //all
        return d->backgrounds[1];
    }
}
}
