#include "docking_pane_fixed_window.h"
#include "docking_pane_window_resizer.h"
#include "docking_pane_manager.h"
#include "docking_pane_container.h"

#include <QGraphicsDropShadowEffect>
namespace ady {
    class DockingPaneFixedWindowPrivate {
    public:
        DockingPaneWindowResizer* region;
        int resizer_size = 0;
        DockingPaneManager::Position position = DockingPaneManager::S_Left;
        DockingPaneContainer* container = nullptr;
    };

    DockingPaneFixedWindow::DockingPaneFixedWindow(QWidget* parent,int margin)
        :QWidget(parent){
        //setWindowFlags(/*Qt::Tool | */Qt::FramelessWindowHint);
        d = new DockingPaneFixedWindowPrivate;
        d->resizer_size = margin;
        initResizer();
    }

    DockingPaneFixedWindow::~DockingPaneFixedWindow()
    {
        delete d;
    }

    void DockingPaneFixedWindow::setCenterWidget(DockingPaneContainer* container)
    {
        d->container = container;
        d->container->setParent(this);
        d->container->setState(DockingPaneContainer::Fixed);
        updateResizer();


        QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
        shadow_effect->setOffset(0, 0);
        shadow_effect->setColor(Qt::black);
        shadow_effect->setBlurRadius(10);
        container->setGraphicsEffect(shadow_effect);
    }

    DockingPaneContainer* DockingPaneFixedWindow::centerWidget()
    {
        return d->container;
    }


    void DockingPaneFixedWindow::updateResizer()
    {
        //QRect rect = this->parentWidget()->geometry();
        QRect rc = geometry();
        DockingPaneWindowResizer::Region region = d->region->region();
        if(region==DockingPaneWindowResizer::Left){
            d->region->setGeometry(QRect(0,0,d->resizer_size,rc.height()));
            if(d->container!=nullptr){
                d->container->setGeometry(d->resizer_size,0,rc.width() - d->resizer_size,rc.height());
            }
        }else if(region==DockingPaneWindowResizer::Top){
             d->region->setGeometry(QRect(0,rc.height() - d->resizer_size,rc.width(),d->resizer_size));
             if(d->container!=nullptr){
                 d->container->setGeometry(0,0,rc.width() ,rc.height() - d->resizer_size);
             }
        }else if(region==DockingPaneWindowResizer::Right){


            //to right
            d->region->setGeometry(QRect(rc.width() - d->resizer_size,0,d->resizer_size,rc.height()));
            if(d->container!=nullptr){
                d->container->setGeometry(0,0,rc.width() - d->resizer_size,rc.height());
            }
        }else if(region==DockingPaneWindowResizer::Bottom){
            d->region->setGeometry(QRect(0,0,rc.width(),d->resizer_size));
            if(d->container!=nullptr){
                d->container->setGeometry(0,d->resizer_size,rc.width() ,rc.height() - d->resizer_size);
            }
        }
    }

    void DockingPaneFixedWindow::initResizer()
    {
        d->region = new DockingPaneWindowResizer(this,DockingPaneWindowResizer::Left);
        updateResizer();
    }

    void DockingPaneFixedWindow::setFixedPosition(int position)
    {
        d->position = (DockingPaneManager::Position)position;
        if(d->position==DockingPaneManager::S_Left){
            d->region->setRegion(DockingPaneWindowResizer::Right);
        }else if(d->position==DockingPaneManager::S_Top){
            d->region->setRegion(DockingPaneWindowResizer::Bottom);
        }else if(d->position==DockingPaneManager::S_Right){
            d->region->setRegion(DockingPaneWindowResizer::Left);
        }else if(d->position==DockingPaneManager::S_Bottom){
            d->region->setRegion(DockingPaneWindowResizer::Top);
        }

        updateResizer();
    }

    int DockingPaneFixedWindow::fixedPosition()
    {
        return d->position;
    }

    void DockingPaneFixedWindow::resizeEvent(QResizeEvent *event)
    {
        QWidget::resizeEvent(event);
        updateResizer();
    }
}
