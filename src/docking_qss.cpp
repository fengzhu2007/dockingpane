#include "docking_qss.h"
#include "docking_theme.h"
#include <QDebug>
namespace ady{


QString DockingQSS::global()
{
    auto instance = DockingTheme::getInstance();
    auto color = instance->color().name(QColor::HexRgb);//light #EEEEF2
    auto primaryColor = instance->primaryColor().name(QColor::HexRgb);//light #007acc
    auto secondaryClientTabColor = instance->secondaryClientTabColor().name(QColor::HexRgb);//light #1c97ea
    auto secondaryTabColor = instance->secondaryTabColor().name(QColor::HexRgb);//light #c9DCF5
    auto primaryTabColor = instance->primaryTabColor().name(QColor::HexRgb);//light #c9DCF5
    auto borderColor = instance->borderColor().name(QColor::HexRgb);//light #ccc
    auto clientTabColor = instance->clientTabColor().name(QColor::HexRgb);
    auto primaryTextColor = instance->primaryTextColor().name(QColor::HexRgb);
    auto secondaryTextColor = instance->secondaryTextColor().name(QColor::HexRgb);
    auto primaryButtonColor = instance->primaryButtonColor().name(QColor::HexRgb);
    auto secondaryButtonColor = instance->secondaryButtonColor().name(QColor::HexRgb);//light #e6e6e6
    auto closeIcon = instance->icon(DockingTheme::ClientTabClose,DockingTheme::Normal);
    auto closeActiveIcon = instance->icon(DockingTheme::ClientTabClose,DockingTheme::Active);
    auto titleBackground = instance->icon(DockingTheme::TitleBackground,DockingTheme::Normal);
    auto titleActiveBackground = instance->icon(DockingTheme::TitleBackground,DockingTheme::Active);

    //qDebug()<<"closeIcon"<<closeActiveIcon;
    QString qss = ".ady--DockingWorkbench{background:"+color+"}"
                  //".ady--DockingGuideCover{background:"+primaryColor+";border:5px solid "+borderColor+"}"
                  ".ady--DockingPaneContainer{background:"+color+";border:1px solid "+borderColor+";}"
                  ".ady--DockingPaneClient{background:"+color+";}"
                  ".ady--DockingPaneClient>QTabBar{background-color:"+color+";border:0;}"
                  ".ady--DockingPaneClient>QTabBar::tab{background-color:"+color+";border-bottom:0;height:24px;padding:0 16px 0 6px;text-align:left;color:"+clientTabColor+"}"
                  ".ady--DockingPaneClient>QTabBar::tab:hover{background-color:"+secondaryClientTabColor+";color:"+primaryTextColor+";}"
                  ".ady--DockingPaneClient>QTabBar::tab:selected{background-color:"+primaryColor+";color:"+primaryTextColor+";}"

#ifdef Q_OS_WIN

                  ".ady--DockingPaneClient>QTabBar::close-button{image:url('"+closeIcon+"');margin-left:2px;}"
                  ".ady--DockingPaneClient>QTabBar::close-button:hover{image:url('"+closeActiveIcon+"')}"
                  ".ady--DockingPaneClient>QTabBar QToolButton{background:"+color+";border:0;}"
                    ".ady--DockingPaneClient>QTabBar QToolButton:hover{background:"+secondaryButtonColor+";border:0;}"
#else
                  ".ady--DockingPaneClient>QTabBar::tab{padding-right:23px}"
                  ".ady--DockingPaneClient>QTabBar::close-button:hover{image:url('"+titleActiveBackground+"')}"
#endif


                  ".ady--DockingPaneClient>QStackedWidget{border:1px solid "+borderColor+";border-top:2px solid "+secondaryClientTabColor+"}"
                  ".ady--DockingPaneContainer>QTabBar{background-color:"+color+";padding:0;border:0;}"
                  ".ady--DockingPaneContainer>QTabBar::tab{border:1px solid "+color+";background-color:"+color+";height:22px;color:"+secondaryTextColor+";border-top:1px solid"+borderColor+";padding:0 8px;}"
                  ".ady--DockingPaneContainer>QTabBar::tab:hover{background:"+secondaryTabColor+";border:1px solid "+secondaryTabColor+";border-top:1px solid "+borderColor+"}"
                  ".ady--DockingPaneContainer>QTabBar::tab:selected{border:0;background-color:"+primaryTabColor+";height:22px;color:"+primaryColor+";border-top:1px solid "+primaryTabColor+";border-left:1px solid "+borderColor+";border-right:1px solid "+borderColor+";border-bottom:1px solid "+color+"}"
                  ".ady--DockingPaneContainer>QTabBar::tab:first{border-left:0}"
                  ".ady--DockingPaneContainer .ady--DockingPaneContainerNClient{background:"+color+"}"
                  ".ady--DockingPaneContainer .ady--DockingPaneContainerNClient>QPushButton{background-color: transparent; border: none;padding:0;margin:0;min-width:17px;}"
                  ".ady--DockingPaneContainer .ady--DockingPaneContainerNClient>QPushButton:hover{background:"+secondaryButtonColor+";}"
                  ".ady--DockingPaneContainer .ady--DockingPaneContainerNClient>QLabel#title{background:"+color+";color:"+secondaryTextColor+"}"
                  ".ady--DockingPaneContainer .ady--DockingPaneContainerNClient>QLabel#label{background: url("+titleBackground+");}"

                  ".ady--DockingPaneContainer[activeState=true] .ady--DockingPaneContainerNClient{background:"+primaryColor+"}"
                  ".ady--DockingPaneContainer[activeState=true] .ady--DockingPaneContainerNClient>QPushButton{background-color: transparent; border: none;}"
                  ".ady--DockingPaneContainer[activeState=true] .ady--DockingPaneContainerNClient>QPushButton:hover{background:"+primaryButtonColor+";}"
                  ".ady--DockingPaneContainer[activeState=true] .ady--DockingPaneContainerNClient>QLabel#title{background:"+primaryColor+";color:"+primaryTextColor+"}"
                  ".ady--DockingPaneContainer[activeState=true] .ady--DockingPaneContainerNClient>QLabel#label{background: url("+titleActiveBackground+");}"

                  ".ady--DockingPaneHandle{background:"+color+"}";

    return qss;
}

}


