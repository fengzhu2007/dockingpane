#include "docking_guide_diamond.h"

#include <QDebug>

namespace ady {

    class DockingGuideDiamondPrivate {
    public:
        DockingPaneManager::Position position;
        bool active = false;
    };

    DockingGuideDiamond::DockingGuideDiamond(QWidget* parent)
        :QLabel(parent)
    {
        d = new DockingGuideDiamondPrivate;
        setAttribute(Qt::WA_TranslucentBackground, true);
    }

    DockingGuideDiamond::DockingGuideDiamond(DockingPaneManager::Position position,QWidget* parent)
        :QLabel(parent){
            d = new DockingGuideDiamondPrivate;
            setPosition(position);
            setAttribute(Qt::WA_TranslucentBackground, true);

    }

    DockingGuideDiamond::~DockingGuideDiamond(){
        delete d;
    }

    DockingPaneManager::Position DockingGuideDiamond::position()
    {
        return d->position;
    }

    void DockingGuideDiamond::setPosition(DockingPaneManager::Position position)
    {
        d->position = position;
        if(d->position == DockingPaneManager::S_Left){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_border_sider_left.png")));
        }else if(d->position == DockingPaneManager::S_Top){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_border_sider_top.png")));
        }else if(d->position == DockingPaneManager::S_Right){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_border_sider_right.png")));
        }else if(d->position == DockingPaneManager::S_Bottom){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_border_sider_bottom.png")));
        }else if(d->position == DockingPaneManager::Left){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_sider_left.png")));
        }else if(d->position == DockingPaneManager::Top){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_sider_top.png")));
        }else if(d->position == DockingPaneManager::Right){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_sider_right.png")));
        }else if(d->position == DockingPaneManager::Bottom){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_sider_bottom.png")));
        }else if(d->position == DockingPaneManager::Center){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_center.png")));
        }
    }

    void DockingGuideDiamond::setActive(bool state)
    {
        d->active = state;
        if(d->active){
            if(d->position == DockingPaneManager::S_Left){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_border_sider_left_active.png")));
            }else if(d->position == DockingPaneManager::S_Top){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_border_sider_top_active.png")));
            }else if(d->position == DockingPaneManager::S_Right){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_border_sider_right_active.png")));
            }else if(d->position == DockingPaneManager::S_Bottom){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_border_sider_bottom_active.png")));
            }else if(d->position == DockingPaneManager::Left){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_sider_left_active.png")));
            }else if(d->position == DockingPaneManager::Top){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_sider_top_active.png")));
            }else if(d->position == DockingPaneManager::Right){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_sider_right_active.png")));
            }else if(d->position == DockingPaneManager::Bottom){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_sider_bottom_active.png")));
            }else if(d->position == DockingPaneManager::Center){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_center_active.png")));
            }
        }else{
            if(d->position == DockingPaneManager::S_Left){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_border_sider_left.png")));
            }else if(d->position == DockingPaneManager::S_Top){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_border_sider_top.png")));
            }else if(d->position == DockingPaneManager::S_Right){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_border_sider_right.png")));
            }else if(d->position == DockingPaneManager::S_Bottom){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_border_sider_bottom.png")));
            }else if(d->position == DockingPaneManager::Left){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_sider_left.png")));
            }else if(d->position == DockingPaneManager::Top){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_sider_top.png")));
            }else if(d->position == DockingPaneManager::Right){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_sider_right.png")));
            }else if(d->position == DockingPaneManager::Bottom){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_sider_bottom.png")));
            }else if(d->position == DockingPaneManager::Center){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/vs2019/dock_center.png")));
            }
        }
    }

    bool DockingGuideDiamond::active()
    {
        return d->active;
    }

    bool DockingGuideDiamond::updateActive(const QPoint& pos){
        bool ret  =  geometry().contains(pos);
        if(ret!=d->active){
            setActive(ret);
        }
        return ret;
    }


}
