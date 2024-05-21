#include "docking_pane_container_tabbar.h"
#include "docking_pane_float_window.h"
#include "docking_pane_container.h"
#include "docking_workbench.h"
#include "docking_pane_layout_item_info.h"
#include "docking_pane_handle.h"
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
    }


    void DockingPaneContainerTabBar::mousePressEvent(QMouseEvent *e)
    {
        QTabBar::mousePressEvent(e);
        //d->moving = true;
        d->offsetX = e->x();
        d->offsetY = e->y();
        //d->index = tabAt(e->pos());
        //qDebug()<<"index:"<<index;
    }

    void DockingPaneContainerTabBar::mouseMoveEvent(QMouseEvent *e)
    {
        QTabBar::mouseMoveEvent(e);
        if(d->moving){
            DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
            if(container!=nullptr){
                if(d->fixed==true){
                    int index = tabAt(e->pos());
                    if(index<0){
                        return ;
                    }
                    setAutoHide(false);
                    bool isClient = container->isClient();
                    QRect parentRect = container->geometry();
                    DockingPane* pane = container->takeAt(index);
                    DockingPaneContainer* new_container = new DockingPaneContainer(container->parentWidget());
                    new_container->setObjectName(pane->id()+"_containter");
                    pane->setParent(new_container);
                    new_container->appendPane(pane);
                    new_container->activeWidget(true);
                    QRect rc = pane->geometry();
                    //float widnow;
                    QPoint pos = mapToGlobal(e->pos());
                    int margin = 6;
                    d->current_window = new DockingPaneFloatWindow(new_container->parentWidget(),margin);
                    QRect rect;
                    if(isClient){
                        rect.setX(pos.x() - d->offsetX - margin);
                        rect.setY(pos.y() - d->offsetY  - margin);
                        rect.setWidth(rc.width() + margin * 2);
                        rect.setHeight(parentRect.height() + margin * 2);
                    }else{
                        rect.setX(pos.x() - d->offsetX - margin);
                        rect.setY(pos.y() - d->offsetY - rc.height()  - margin);
                        rect.setWidth(rc.width() + margin * 2);
                        rect.setHeight(parentRect.height() + margin * 2);
                    }
                    d->current_window->setGeometry(rect);
                    d->current_window->setCenterWidget(new_container);
                    d->current_window->updateResizer();
                    d->current_window->show();
                    if(isClient){
                        DockingPaneLayoutItemInfo* itemInfo = container->itemInfo();
                        //itemInfo->dump("tabbar");
                        DockingPaneLayoutItemInfo* parentItemInfo = itemInfo->parent();
                        //parentItemInfo->dump("111");
                        //qDebug()<<"client children:"<<parentItemInfo->clientChildren().size();
                        if(parentItemInfo->clientChildren().size()>1){
                            //DockingPaneLayoutItemInfo* itemInfo = container->itemInfo();
                            /*DockingPaneHandle* handle = itemInfo->handle();
                            if(handle!=nullptr){
                                handle->hide();
                            }*/
                            //itemInfo->remove();
                            parentItemInfo->removeItem(itemInfo);
                            //delete itemInfo;
                            container->hide();
                            d->del_parent = true;
                            //parentItemInfo->dump("222");
                            //container->hide();
                            //container->deleteLater();
                        }
                    }
                    //window->setMouseTracking(true);
                    //d->current_window->startMoving();

                    //QMouseEvent event(QEvent::MouseButtonPress, e->pos(), Qt::LeftButton, 0, 0);
                    //QApplication::sendEvent(window, &event);

                    d->fixed=false;
                }else{
                    QPoint cursorPos = e->pos();
                    int x = e->x() ;
                    int y = e->y();
                    x -= d->offsetX;
                    y -= d->offsetY;
                    d->offsetX = e->x();
                    d->offsetY = e->y();
                    QWidget* window = d->current_window;
                    QPoint pos = window->pos();
                    pos.rx() += x;
                    pos.ry() += y;
                    //qDebug()<<"x:"<<x<<";y:"<<y<<";";
                    //qDebug()<<"pos:"<<pos;
                    window->move(pos);

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


            }
            //qDebug()<<"DockingPaneContainerTabBar::mouseMoveEvent";
        }else{
            if(abs(d->offsetX - e->x())>3 || abs(d->offsetY-e->y())>3){
                d->moving = true;
            }
        }
    }

    void DockingPaneContainerTabBar::mouseReleaseEvent(QMouseEvent *e)
    {
        QTabBar::mouseReleaseEvent(e);
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

    void DockingPaneContainerTabBar::paintEvent(QPaintEvent* event){
        QTabBar::paintEvent(event);
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


        /*Q_UNUSED(event);

        QStylePainter p(this);
        QColor color, textColor;

        p.setRenderHint(QPainter::Antialiasing, true);
        p.setRenderHint(QPainter::HighQualityAntialiasing, true);

        if (d->hover) {
            color = textColor = QColor(0, 122, 204);
        } else {
            //#444444
            //textColor = Qt::white;
            textColor = QColor("#444444");
            color = QColor(0xcc, 0xce, 0xdb);
        }

        p.setPen(textColor);

        switch (d->orientation) {
        case Qt::Horizontal: {
            if (d->mirrored) {
                p.rotate(180);
                p.translate(-width(), -height());
            }
            if (d->swap) {
                p.fillRect(0, height()-6, width(), 6, color);
                p.drawText(0, 0, width(), height()-10, 0, this->text());
            } else {
                p.fillRect(0, 0, width(), 6, color);
                p.drawText(0, 10, width(), height(), 0, this->text());
            }
            break;
        }

        case Qt::Vertical: {
            if (d->mirrored) {
                p.rotate(-90);
                p.translate(-height(), 0);
            } else {
                p.rotate(90);
                p.translate(0, -width());
            }
            if (d->swap) {
                p.fillRect(0, 0, height(), 6, color);
                p.drawText(0, 10, height(), width(), 0, this->text());
            } else {
                p.fillRect(0, width()-6, height(), 6, color);
                p.drawText(0, 0, height(), width()-10, 0, this->text());
            }

            break;
        }
        }*/
    }

}
