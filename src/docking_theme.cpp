#include "docking_theme.h"
#include "docking_light_theme.h"
namespace ady{
DockingTheme* DockingTheme::instance=nullptr;

DockingTheme::DockingTheme(){

}

DockingTheme::~DockingTheme(){

}

DockingTheme* DockingTheme::init(Style style){
    if(style==Dark){
        return DockingTheme::init<DockingLightTheme>();
    }else{
        return DockingTheme::init<DockingLightTheme>();
    }
}

void DockingTheme::destory(){
    if(instance!=nullptr){
        delete instance;
    }
}

DockingTheme* DockingTheme::getInstance(){
    return instance;
}

}
