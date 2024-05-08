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
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_left.png")));
        }else if(d->position == DockingPaneManager::S_Top){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_top.png")));
        }else if(d->position == DockingPaneManager::S_Right){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_right.png")));
        }else if(d->position == DockingPaneManager::S_Bottom){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_bottom.png")));
        }else if(d->position == DockingPaneManager::Left){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_b_left.png")));
        }else if(d->position == DockingPaneManager::Top){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_b_top.png")));
        }else if(d->position == DockingPaneManager::Right){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_b_right.png")));
        }else if(d->position == DockingPaneManager::Bottom){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_b_bottom.png")));
        }else if(d->position == DockingPaneManager::Center){
            this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_center.png")));
        }
    }

    void DockingGuideDiamond::setActive(bool state)
    {
        d->active = state;
        if(d->active){
            if(d->position == DockingPaneManager::S_Left){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_left_act.png")));
            }else if(d->position == DockingPaneManager::S_Top){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_top_act.png")));
            }else if(d->position == DockingPaneManager::S_Right){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_right_act.png")));
            }else if(d->position == DockingPaneManager::S_Bottom){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_bottom_act.png")));
            }else if(d->position == DockingPaneManager::Left){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_b_left_act.png")));
            }else if(d->position == DockingPaneManager::Top){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_b_top_act.png")));
            }else if(d->position == DockingPaneManager::Right){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_b_right_act.png")));
            }else if(d->position == DockingPaneManager::Bottom){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_b_bottom_act.png")));
            }else if(d->position == DockingPaneManager::Center){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_center_act.png")));
            }
        }else{
            if(d->position == DockingPaneManager::S_Left){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_left.png")));
            }else if(d->position == DockingPaneManager::S_Top){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_top.png")));
            }else if(d->position == DockingPaneManager::S_Right){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_right.png")));
            }else if(d->position == DockingPaneManager::S_Bottom){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_bottom.png")));
            }else if(d->position == DockingPaneManager::Left){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_b_left.png")));
            }else if(d->position == DockingPaneManager::Top){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_b_top.png")));
            }else if(d->position == DockingPaneManager::Right){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_b_right.png")));
            }else if(d->position == DockingPaneManager::Bottom){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_b_bottom.png")));
            }else if(d->position == DockingPaneManager::Center){
                this->setPixmap(QPixmap(QString::fromUtf8(":/images/docking_center.png")));
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
