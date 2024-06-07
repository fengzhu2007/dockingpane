#include "qss.h"
namespace ady{


QString QSS::global()
{

    QString qss = ".ady--DockingWorkbench{background:#eeeef2}"
                  ".ady--DockingGuideCover{background:#007acc;border:5px solid #aaa}"
                  ".ady--DockingPaneContainer{background:white;border:1px solid #ccc;}"
                  ".ady--DockingPaneClient{background:white;}"
                  ".ady--DockingPaneClient>QTabBar{background-color:#EEEEF2;border:0;border-bottom:2px solid #1c97ea}"
                  ".ady--DockingPaneClient>QTabBar::tab{background-color:#EEEEF2;border-bottom:2px solid #007acc;height:24px;padding:0 4px 0 6px;}"
                  ".ady--DockingPaneClient>QTabBar::tab:hover{background-color:#1c97ea;color:white;}"
                  ".ady--DockingPaneClient>QTabBar::tab:selected{background-color:#007acc;color:white;}"
                  ".ady--DockingPaneClient>QTabBar::close-button{image:url(':/images/vs2019/dock_close_light_gray.png')}"
                  ".ady--DockingPaneClient>QTabBar::close-button:hover{image:url(':/images/vs2019/dock_close_white.png')}"
                  ".ady--DockingPaneClient>QStackedWidget{border:1px solid #ccc;border-top:0;}"
                  ".ady--DockingPaneContainer>QTabBar{background-color:#EEEEF2;padding:0;border:0}"
                  ".ady--DockingPaneContainer>QTabBar::tab{border:0;background-color:#EEEEF2;height:22px;padding-left:5px;padding-right:5px;color:#444444;border-top:1px solid #ccc;}"
                  ".ady--DockingPaneContainer>QTabBar::tab:hover{background:#c9DCF5}"
                  ".ady--DockingPaneContainer>QTabBar::tab:selected{border:0;background-color:#F5F5F5;height:22px;color:#007acc;border-top:1px solid #F5F5F5;border-left:1px solid #ccc;border-right:1px solid #ccc;border-bottom:1px solid #EEEEF2}"
                  ".ady--DockingPaneContainer>QTabBar::tab:first{border-left:0}"
                  ".ady--DockingPaneContainer .ady--DockingPaneContainerNClient{background:#eeeef2}"
                  ".ady--DockingPaneContainer .ady--DockingPaneContainerNClient>QPushButton{background-color: transparent; border: none;}"
                  ".ady--DockingPaneContainer .ady--DockingPaneContainerNClient>QPushButton:hover{background:#e6e6e6;}"
                  ".ady--DockingPaneContainer .ady--DockingPaneContainerNClient>QLabel#title{background:#eeeef2;color:#444444}"
                  ".ady--DockingPaneContainer .ady--DockingPaneContainerNClient>QLabel#label{background: url(:/images/vs2019/dock_head_gray.png);}"

                  ".ady--DockingPaneContainer[activeState=true] .ady--DockingPaneContainerNClient{background:#007acc}"
                  ".ady--DockingPaneContainer[activeState=true] .ady--DockingPaneContainerNClient>QPushButton{background-color: transparent; border: none;}"
                  ".ady--DockingPaneContainer[activeState=true] .ady--DockingPaneContainerNClient>QPushButton:hover{background:#52b0ef;}"
                  ".ady--DockingPaneContainer[activeState=true] .ady--DockingPaneContainerNClient>QLabel#title{background:#007acc;color:white}"
                  ".ady--DockingPaneContainer[activeState=true] .ady--DockingPaneContainerNClient>QLabel#label{background: url(:/images/vs2019/dock_head_white.png);}"

                  ".ady--DockingPaneHandle{background:#eeeef2}";

    return qss;
}

}


