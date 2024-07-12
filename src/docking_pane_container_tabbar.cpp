#include "docking_pane_container_tabbar.h"
#include "docking_pane_float_window.h"
#include "docking_pane_container.h"
#include "docking_workbench.h"
#include "docking_pane_layout_item_info.h"
//#include "docking_pane_handle.h"
#include "docking_pane.h"
#include <QMouseEvent>
#include <QEvent>
#include <QApplication>
#include <QStylePainter>
#include <QDebug>
namespace ady {
    class DockingPaneContainerTabBarPrivate {
    public:
        bool moving = false;
        bool fixed = true;
        bool del_parent = false;
        int offsetX = 0;
        int offsetY = 0;
        int position = -1;
        DockingPaneContainer* guide_container = nullptr;
        DockingPaneFloatWindow* current_window = nullptr;
        //int index = -1;

    };

    DockingPaneContainerTabBar::DockingPaneContainerTabBar(QWidget* parent)
        :QTabBar(parent){
        d = new DockingPaneContainerTabBarPrivate;
        setContextMenuPolicy(Qt::CustomContextMenu);
        connect(this, &QWidget::customContextMenuRequested, this, &DockingPaneContainerTabBar::showContextMenu);

    }

    void DockingPaneContainerTabBar::showContextMenu(const QPoint &pos){
        int i = this->tabAt(pos);
        if(i>=0){
            auto container = static_cast<DockingPaneContainer*>(parentWidget());
            if(container!=nullptr){
                container->pane(i)->contextMenu(pos);
            }
        }
    }

    void DockingPaneContainerTabBar::onFloat(int i,bool moving){
        DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
        if(container!=nullptr){
            if(d->fixed==true){
                setAutoHide(false);
                bool isClient = container->isClient();
                QRect parentRect = container->geometry();
                DockingPane* pane = container->takeAt(i);
                DockingPaneContainer* new_container = new DockingPaneContainer(container->parentWidget());
                new_container->setGeometry(parentRect);
                new_container->setObjectName(pane->id()+"_containter");
                pane->setParent(new_container);
                new_container->appendPane(pane);
                new_container->activeWidget(true);
                QPoint pos = QCursor::pos();
                int margin = 6;
                d->current_window = new DockingPaneFloatWindow(new_container->parentWidget(),margin);
                if(moving){
                    d->current_window->setGeometry(QRect(pos.x(),pos.y(),parentRect.width(),parentRect.height()));
                }else{
                    d->current_window->setGeometry(QRect(parentRect.x(),parentRect.y(),parentRect.width(),parentRect.height()));
                }
                d->current_window->setCenterWidget(new_container);
                d->current_window->updateResizer();
                d->current_window->show();
                if(isClient){
                    DockingPaneLayoutItemInfo* itemInfo = container->itemInfo();
                    DockingPaneLayoutItemInfo* parentItemInfo = itemInfo->parent();
                    if(parentItemInfo->clientChildren().size()>1){
                        if(container->paneCount()==0){
                            parentItemInfo->removeItem(itemInfo);
                            //delete itemInfo;
                            container->hide();
                            d->del_parent = true;
                        }
                    }
                }

                d->fixed=false;
            }
        }
    }

    void DockingPaneContainerTabBar::onFloatRelease(){
        d->moving = false;
        d->fixed = true;
        DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
        if(container->isClient()==false){
            setAutoHide(true);
        }
        if(d->current_window!=nullptr){
            DockingWorkbench* workbench = (DockingWorkbench*)container->parentWidget();
            workbench->hideGuide();
            workbench->hideSiderGuide();
            workbench->endLookup();
            if(d->position>=0){
                workbench->lockContainer(d->current_window,d->guide_container,d->position);
            }
        }
        d->current_window = nullptr;
        if(d->del_parent){
            d->del_parent = false;
            container->close();
            container->deleteLater();
        }
    }



    void DockingPaneContainerTabBar::mousePressEvent(QMouseEvent *e)
    {
        QTabBar::mousePressEvent(e);
        //d->moving = true;
        d->offsetX = e->x();
        d->offsetY = e->y();

        DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
        if(container!=nullptr && container->state()==DockingPaneContainer::Inner &&container->activeState()==false){
            DockingWorkbench* workbench = (DockingWorkbench*)container->parentWidget();
            workbench->unActiveAll();
            container->activeWidget(true);
        }
    }

    void DockingPaneContainerTabBar::mouseMoveEvent(QMouseEvent *e)
    {
        QTabBar::mouseMoveEvent(e);
        if(d->moving){
            DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
            if(d->fixed==true){
                int index = tabAt(e->pos());
                this->onFloat(index,true);
            }else{
                QWidget* window = d->current_window;
                QPoint cursorPos = e->pos();
                if(!container->isClient()){
                    int x = e->x() ;
                    int y = e->y();
                    x -= d->offsetX;
                    y -= d->offsetY;
                    d->offsetX = e->x();
                    d->offsetY = e->y();
                    QPoint pos = window->pos();
                    pos.rx() += x;
                    pos.ry() += y;
                    window->move(pos);
                }else{
                    QPoint pos = QCursor::pos();
                    pos.rx() -= d->offsetX;
                    pos.ry() -= d->offsetY;
                    window->move(pos);
                }
                DockingWorkbench* workbench = (DockingWorkbench*)window->parentWidget();
                QPoint globalPos = mapToGlobal(cursorPos);
                workbench->startLookup();
                QRect rc;
                bool guide_visibility = false;
                DockingPaneContainer* container = workbench->lookup(globalPos,rc,guide_visibility);

                int position = -1;
                if(guide_visibility){
                    workbench->showGuide(container,rc);
                    position = workbench->activeGuide(globalPos);
                }else{
                    workbench->hideGuide();
                    position = workbench->activeSiderGuide(globalPos);
                }
                workbench->showSiderGuide();

                if(position>=0){
                    workbench->showGuideCover(container,position,window->geometry());
                }else{
                    workbench->hideGuideCover();
                }
                d->position = position;
                d->guide_container = container;
            }
        }else{
            if(abs(d->offsetX - e->x())>3 || abs(d->offsetY-e->y())>3){
                d->moving = true;
            }
        }
    }

    void DockingPaneContainerTabBar::mouseReleaseEvent(QMouseEvent *e)
    {
        QTabBar::mouseReleaseEvent(e);
        this->onFloatRelease();
    }

    void DockingPaneContainerTabBar::paintEvent(QPaintEvent* event){
        QTabBar::paintEvent(event);
        if(!this->tabsClosable()){
            //container tabbar
            QStylePainter p(this);
            int count = this->count();
            int w = 0;
            for(int i=0;i<count;i++){
                QRect rc = this->tabRect(i);
                w += rc.width();
            }
            QColor color, textColor;
            color = textColor = QColor("#cccccc");
            p.setPen(textColor);
            p.fillRect(w, 0, width() - w, 1, color);
        }else{
            //client tabbar
        }

    }

}
