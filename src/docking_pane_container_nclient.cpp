#include "docking_pane_container_nclient.h"
#include "docking_pane_container.h"
#include "docking_pane_float_window.h"
#include "docking_pane_fixed_window.h"
#include "docking_pane_layout_item_info.h"
#include "docking_workbench.h"
#include "docking_pane_handle.h"
#include "docking_pane_tabbar.h"
#include "docking_pane.h"
//#include "qss.h"
#include "ui_docking_pane_container_nclient.h"
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>


namespace ady{
    class DockingPaneContainerNClientPrivate {
    public:
        bool moving = false;
        int offsetX = 0;
        int offsetY = 0;
        int position = -1;
        DockingPaneContainer* guide_container = nullptr;
        bool active = false;

    };

    DockingPaneContainerNClient::DockingPaneContainerNClient(DockingPaneContainer* parent)
        :QFrame((QWidget*)parent)
        ,ui(new Ui::DockingPaneContainerNClient){
        //setMouseTracking(true);
        //setObjectName("nclient");

        ui->setupUi(this);

        this->setCursor(Qt::ArrowCursor);
        d = new DockingPaneContainerNClientPrivate;
        connect(ui->close,&QPushButton::clicked,this,&DockingPaneContainerNClient::onClose);
        connect(ui->pin,&QPushButton::clicked,this,&DockingPaneContainerNClient::onAutoHide);
        connect(ui->menu,&QPushButton::clicked,this,&DockingPaneContainerNClient::onMenuRequested);
        setActive(false);

    }

    void DockingPaneContainerNClient::onMenuRequested()
    {
        QMenu* menu = new QMenu;

        QAction* fAction = menu->addAction(tr("Float"),this,SLOT(onFloat()));
        QAction* dAction = menu->addAction(tr("Docking"),this,SLOT(onDock()));
        QAction* tdAction = menu->addAction(tr("Tab Docking"),this,SLOT(onTabDock()));
        menu->addSeparator();
        menu->addAction(tr("Close"),this,SLOT(onClose()));

        DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
        DockingPaneContainer::State state = container->state();
        if(state==DockingPaneContainer::Inner){
            dAction->setEnabled(false);
        }else if(state==DockingPaneContainer::Float){
            fAction->setEnabled(false);
        }else if(state==DockingPaneContainer::Fixed){
            tdAction->setEnabled(false);
        }
        menu->exec(QCursor::pos());
    }

    DockingPaneContainerNClient::~DockingPaneContainerNClient(){
        delete ui;
        delete d;
    }

    void DockingPaneContainerNClient::updateTitle(QString title)
    {
        ui->title->setText(title);
    }

    void DockingPaneContainerNClient::setButtonState(Button b,State s)
    {
        if(d->active){
            if(b==Pin){
                if(s==Inner){
                    ui->pin->setIcon(QIcon(QString::fromUtf8(":/images/vs2019/dock_pin_white.png")));
                }else if(s==Float){
                    QWidget* window = parentWidget()->parentWidget();
                    if(window->isMaximized()){
                        ui->pin->setIcon(QIcon(QString::fromUtf8(":/images/vs2019/dock_restore_white.png")));
                    }else{
                        ui->pin->setIcon(QIcon(QString::fromUtf8(":/images/vs2019/dock_max_white.png")));
                    }
                }else if(s==Fixed){
                    ui->pin->setIcon(QIcon(QString::fromUtf8(":/images/vs2019/dock_pin_fixed_white.png")));
                }
            }else if(b==Dropdown){
                ui->menu->setIcon(QIcon(QString::fromUtf8(":/images/vs2019/dock_drow_white.png")));
            }else if(b==Close){
                ui->close->setIcon(QIcon(QString::fromUtf8(":/images/vs2019/dock_close_white.png")));
            }
        }else{
            if(b==Pin){
                if(s==Inner){
                    ui->pin->setIcon(QIcon(QString::fromUtf8(":/images/vs2019/dock_pin_gray.png")));
                }else if(s==Float){
                    QWidget* window = parentWidget()->parentWidget();
                    if(window->isMaximized()){
                        ui->pin->setIcon(QIcon(QString::fromUtf8(":/images/vs2019/dock_restore_gray.png")));
                    }else{
                        ui->pin->setIcon(QIcon(QString::fromUtf8(":/images/vs2019/dock_max_gray.png")));
                    }
                }else if(s==Fixed){
                    ui->pin->setIcon(QIcon(QString::fromUtf8(":/images/vs2019/dock_pin_fixed_gray.png")));
                }
            }else if(b==Dropdown){
                ui->menu->setIcon(QIcon(QString::fromUtf8(":/images/vs2019/dock_drow_gray.png")));
            }else if(b==Close){
                ui->close->setIcon(QIcon(QString::fromUtf8(":/images/vs2019/dock_close_gray.png")));
            }
        }

    }

    void DockingPaneContainerNClient::setActive(bool active)
    {
        //theme
        d->active = active;
        DockingPaneContainer* container = (DockingPaneContainer*)this->parentWidget();
        int state = container->state();
        setButtonState(Close,Inner);
        setButtonState(Dropdown,Inner);
        setButtonState(Pin,(State)state);
    }

    void DockingPaneContainerNClient::setMoving(bool state){
        d->moving = state;
    }

    void DockingPaneContainerNClient::stylePolish(){
        QStyle* style = this->style();
        style->polish(ui->title);
        style->polish(ui->label);
        style->polish(ui->close);
        style->polish(ui->menu);
        style->polish(ui->pin);
        style->polish(this);
    }

    void DockingPaneContainerNClient::onClose()
    {
        DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
        DockingPaneContainer::State state = container->state();
        int paneCount = container->paneCount();
        if(state==DockingPaneContainer::Inner){
            if(paneCount>1){
                container->closeCurrent();
                return ;
            }
            DockingPaneLayoutItemInfo* itemInfo = container->itemInfo();
            itemInfo->remove();
            delete itemInfo;
            container->setParent(nullptr);
            container->close();
            container->deleteLater();
        }else if(state==DockingPaneContainer::Float){
            container->parentWidget()->close();
            DockingPaneFloatWindow* window = (DockingPaneFloatWindow*)container->parentWidget();
            window->close();
        }else if(state==DockingPaneContainer::Fixed){
            DockingPaneFixedWindow* window = (DockingPaneFixedWindow*)container->parentWidget();
            int position = window->fixedPosition();
            DockingWorkbench* workbench = (DockingWorkbench*)window->parentWidget();
            DockingPaneTabBar* tabBar = workbench->tabBar(position);
            if(tabBar!=nullptr){
                tabBar->removeContainerChild(container,container->current());//fixed
            }
            if(container->paneCount()==0){
                container->setParent(nullptr);
                container->close();
                container->deleteLater();
            }
            window->hide();
        }
    }

    void DockingPaneContainerNClient::onAutoHide()
    {
        DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
        DockingPaneContainer::State state = container->state();
        if(state==DockingPaneContainer::Inner){
            DockingWorkbench* workbench = (DockingWorkbench*)container->parentWidget();
            workbench->siderFixed(container,DockingPaneManager::S_Left);
        }else if(state==DockingPaneContainer::Fixed){
            //restore to inner
            DockingPaneFixedWindow *window = (DockingPaneFixedWindow*)container->parentWidget();
            int paneCount = container->paneCount();
            DockingWorkbench* workbench = (DockingWorkbench*)container->parentWidget()->parentWidget();
            if(paneCount>1){
                int pos = window->fixedPosition();
                DockingPaneTabBar* tabBar = workbench->tabBar(pos);
                tabBar->removeContainerChild(container,container->current());
                DockingPane* pane = container->takeCurrent();
                workbench->restorePane(pane,pos);
                window->hide();
                return ;
            }
            workbench->restoreWidget(container,window->fixedPosition());
            //window->close();
            //window->deleteLater();
            //workbench->updateLayout();
            window->setCenterWidget(nullptr);
            workbench->hideFixedWindow();
        }else if(state==DockingPaneContainer::Float){
            /*DockingPaneFloatWindow *window = (DockingPaneFloatWindow*)container->parentWidget();
            DockingWorkbench* workbench = (DockingWorkbench*)container->parentWidget()->parentWidget();
            workbench->restoreWidget(container,container->oriPosition());
            window->close();
            window->deleteLater();*/
            DockingPaneFloatWindow *window = (DockingPaneFloatWindow*)container->parentWidget();
            DockingWorkbench* workbench = (DockingWorkbench*)container->parentWidget()->parentWidget();

            workbench->unActiveAll();
            container->activeWidget(true);
            if(!window->isMaximized()){
                window->showMaximized();
            }else{
                window->showNormal();
            }
            this->setButtonState(Pin,Float);
        }
    }

    void DockingPaneContainerNClient::onFloat(bool moving){
        DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
        if(container!=nullptr){
            int paneCount = container->paneCount();
            DockingPaneContainer::State state = container->state();
            //qDebug()<<"onFloat"<<state;
            if(state==DockingPaneContainer::Inner){
                //fixed create float window;
                QRect rc = container->geometry();
                QPoint pos = mapToGlobal(container->pos());
                int margin = 6;
                DockingWorkbench* workbench = (DockingWorkbench*)container->parentWidget();
                DockingPaneFloatWindow* window = new DockingPaneFloatWindow(workbench,margin);
                if(false && paneCount>1){//drag move all tabs
                    int current = container->current();
                    DockingPane* pane = container->pane(current);

                    DockingPaneContainer* restore_container = new DockingPaneContainer(workbench);
                    restore_container->setState(DockingPaneContainer::Inner);
                    restore_container->setOriPosition(container->oriPosition());
                    int j = 0;
                    for(int i=0;i<paneCount;i++){
                        DockingPane* p = container->pane(j);
                        if(p!=pane){
                            container->takeAt(j);
                            restore_container->appendPane(p);
                        }else{
                            j += 1;
                            if(j==1){
                                restore_container->setObjectName(pane->id()+"_containter");
                            }
                        }
                    }
                    restore_container->setPane(0);
                    DockingPaneLayoutItemInfo* itemInfo = container->itemInfo();
                    restore_container->setItemInfo(itemInfo);
                    itemInfo->setItem(new QWidgetItem(restore_container));
                }else{
                    DockingPaneLayoutItemInfo* itemInfo = container->itemInfo();
                    qDebug()<<"itemInfo:"<<itemInfo;
                    DockingPaneHandle* handle = itemInfo->handle();
                    if(handle!=nullptr){
                        handle->hide();
                    }
                    itemInfo->remove();
                    delete itemInfo;
                }
                QRect rect;

                if(!moving){
                    QRect w_rc = workbench->geometry();
                    int x = (w_rc.width() - rc.width()) / 2;
                    int y = (w_rc.height() - rc.height() / 4 * 3);

                    QPoint gpos = workbench->mapToGlobal(QPoint(x,y));
                    /*QPoint cpos = QCursor::pos();
                    rect.setX(cpos.x() - rect.width() + 80);//80= three buttons width
                    rect.setY(cpos.y());*/
                    //rect.setX(gpos.x());
                    //rect.setY(gpos.y());
                    rect = QRect(gpos.x(),gpos.y(),rc.width(),rc.height());
                    //qDebug()<<"on float rc:"<<rc<<w_rc<<rect;

                }else{
                    rect.setX(pos.x() - margin);
                    rect.setY(pos.y()  - margin);
                    //qDebug()<<rect;
                    rect.setWidth(rc.width() + margin * 2);
                    rect.setHeight(rc.height() + margin * 2);
                }
                container->activeWidget(true);
                window->setGeometry(rect);
                window->setCenterWidget(container);
                window->updateResizer();
                window->show();

            }else if(state==DockingPaneContainer::Fixed){
                //fixed create float window;
                DockingPaneFixedWindow* fixed_window = (DockingPaneFixedWindow*)container->parentWidget();
                //remove sider tabbar items
                int position = fixed_window->fixedPosition();
                //qDebug()<<"position"<<position;
                DockingWorkbench* workbench = (DockingWorkbench*)fixed_window->parentWidget();
                DockingPaneTabBar* tabBar = workbench->tabBar(position);
                //tabBar->removeContainerChild(container,container->current());
                //qDebug()<<"container:"<<container;
                QRect rc = container->geometry();
                QPoint pos = mapToGlobal(container->pos());
                int index = tabBar->search(container);
                tabBar->removeContainer(container);
                //container->setParent(nullptr);
                if(paneCount>1){
                    int current = container->current();
                    DockingPane* pane = container->pane(current);
                    DockingPaneContainer* restore_container = new DockingPaneContainer(nullptr);
                    restore_container->setState(DockingPaneContainer::Fixed);
                    restore_container->setOriPosition((DockingPaneManager::Position)position);
                    int j = 0;
                    for(int i=0;i<paneCount;i++){
                        DockingPane* p = container->pane(j);
                        if(p!=pane){
                            container->takeAt(j);
                            restore_container->appendPane(p);
                        }else{
                            j += 1;
                            if(j==1){
                                restore_container->setObjectName(p->id()+"_containter");
                            }
                        }
                    }
                    restore_container->setPane(0);
                    tabBar->insertContainer(index,restore_container);
                }
                int margin = 6;
                QRect rect;
                rect.setWidth(rc.width() + margin * 2);
                rect.setHeight(rc.height() + margin * 2);
                if(!moving){
                    QPoint cpos = QCursor::pos();
                    rect.setX(cpos.x() - rect.width() + 80);//80= three buttons width
                    rect.setY(cpos.y());
                }else{
                    rect.setX(pos.x() - margin);
                    rect.setY(pos.y()  - margin);
                }
                container->activeWidget(true);
                DockingPaneFloatWindow* window = new DockingPaneFloatWindow(fixed_window->parentWidget(),margin);
                window->setGeometry(rect);
                window->setCenterWidget(container);
                window->updateResizer();
                window->show();
                fixed_window->setCenterWidget(nullptr);
                fixed_window->hide();
                //workbench->updateLayout();

            }
        }
    }

    void DockingPaneContainerNClient::onDock(){
        DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
        if(container!=nullptr){
            DockingWorkbench* workbench = container->workbench();
            QWidget* parent = container->parentWidget();
            int paneCount = container->paneCount();
            DockingPaneContainer::State state = container->state();

            if(state==DockingPaneContainer::Float){
                DockingPane* pane = container->takeCurrent();
                workbench->restorePane(pane,DockingPaneManager::S_Left);
                if(paneCount<=1){
                    parent->close();
                    parent->deleteLater();
                }
                return ;
            }else if(state==DockingPaneContainer::Fixed){
                DockingPaneFixedWindow* window = (DockingPaneFixedWindow*)parent;
                int position = window->fixedPosition();
                DockingPaneTabBar* tabBar = workbench->tabBar(position);
                tabBar->removeContainerChild(container,container->current());
                DockingPane* pane = container->takeCurrent();
                workbench->restorePane(pane,position);
                if(paneCount<=1){
                    window->setCenterWidget(nullptr);
                    container->close();
                    container->deleteLater();
                }
                workbench->hideFixedWindow();
                return ;
            }
        }
    }

    void DockingPaneContainerNClient::onTabDock(){
        DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
        DockingWorkbench* workbench = container->workbench();
        DockingPaneContainer::State state = container->state();
        QWidget* parent = container->parentWidget();
        int paneCount = container->paneCount();

        if(state==DockingPaneContainer::Float){
            DockingPane* pane = container->takeCurrent();
            workbench->restorePane(pane,DockingPaneManager::Center);
            if(paneCount<=1){
                parent->close();
                parent->deleteLater();
            }
            return ;
        }else if(state==DockingPaneContainer::Fixed){
            DockingPaneFixedWindow* window = (DockingPaneFixedWindow*)parent;
            int position = window->fixedPosition();
            DockingPaneTabBar* tabBar = workbench->tabBar(position);
            tabBar->removeContainerChild(container,container->current());
            DockingPane* pane = container->takeCurrent();
            workbench->restorePane(pane,DockingPaneManager::Center);
            workbench->hideFixedWindow();
            if(paneCount<=1){
                window->setCenterWidget(nullptr);
                container->close();
                container->deleteLater();
            }
            return ;
        }else if(state==DockingPaneContainer::Inner){
            DockingPane* pane = container->takeCurrent();
            workbench->restorePane(pane,DockingPaneManager::Center);
            if(paneCount<=1){
                DockingPaneLayoutItemInfo* itemInfo = container->itemInfo();
                DockingPaneLayoutItemInfo* parentItemInfo = itemInfo->parent();
                parentItemInfo->removeItem(itemInfo);
                container->close();
                container->deleteLater();
                workbench->updateLayout();
            }
        }
    }


    void DockingPaneContainerNClient::mouseMoveEvent(QMouseEvent *e)
    {
        QWidget::mouseMoveEvent(e);
        if(d->moving){
            DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
            if(container!=nullptr){
                DockingPaneContainer::State state = container->state();
                if(state==DockingPaneContainer::Inner || state==DockingPaneContainer::Fixed){
                    this->onFloat(true);
                }else{
                    //int paneCount = container->paneCount();
                    //float move window
                    QPoint cursorPos = e->pos();
                    int x = e->x() ;
                    int y = e->y();
                    x -= d->offsetX;
                    y -= d->offsetY;
                    QWidget* window = container->parentWidget();
                    QPoint pos = window->pos();
                    pos.rx() += x;
                    pos.ry() += y;
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
        }else{
            if(abs(d->offsetX - e->x())>3 || abs(d->offsetY-e->y())>3){
                d->moving = true;
                DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
                if(container->state()==DockingPaneContainer::Fixed){
                    container->visibleTabBar(true);
                }
            }

        }
    }

    void DockingPaneContainerNClient::mousePressEvent(QMouseEvent *e)
    {
        QWidget::mousePressEvent(e);

        //d->moving = true;
        d->offsetX = e->x();
        d->offsetY = e->y();
    }

    void DockingPaneContainerNClient::mouseReleaseEvent(QMouseEvent *e)
    {
        QWidget::mouseReleaseEvent(e);
        d->moving = false;
        DockingPaneContainer* parent = (DockingPaneContainer*)parentWidget();
        DockingPaneContainer::State state = parent->state();
        if(state==DockingPaneContainer::Inner){
            DockingWorkbench* workbench = (DockingWorkbench*)parent->parentWidget();
            workbench->hideGuide();
            workbench->hideSiderGuide();
            workbench->endLookup();
        }else if(state==DockingPaneContainer::Float){
            DockingWorkbench* workbench = (DockingWorkbench*)parent->parentWidget()->parentWidget();

            workbench->hideGuide();
            workbench->hideSiderGuide();
            workbench->endLookup();

            if(d->position>=0){
                //qDebug()<<"widget:"<<parent<<";guid:"<<d->guide_container<<";position:"<<d->position;
                workbench->lockContainer((DockingPaneFloatWindow*)parent->parentWidget(),d->guide_container,d->position);
            }
        }
    }

    void DockingPaneContainerNClient::paintEvent(QPaintEvent *e)
    {
        //QFrame::paintEvent(e);
        Q_UNUSED(e);
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Frame, &opt, &p, this);
    }




}
