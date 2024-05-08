#include "docking_pane_container_nclient.h"
#include "docking_pane_container.h"
#include "docking_pane_float_window.h"
#include "docking_pane_fixed_window.h"
#include "docking_pane_layout_item_info.h"
#include "docking_workbench.h"
#include "docking_pane_handle.h"
#include "docking_pane_tabbar.h"
#include "ui_docking_pane_container_nclient.h"
#include <QStyleOption>
#include <QPainter>
#include <QDebug>


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
        setActive(false);

        ui->pin->setAttribute(Qt::WA_TranslucentBackground);
        ui->close->setAttribute(Qt::WA_TranslucentBackground);
        ui->menu->setAttribute(Qt::WA_TranslucentBackground);
    }

    void DockingPaneContainerNClient::onMenuRequested(const QPoint &pos)
    {
        qDebug()<<"menu request:"<<pos;
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
        if(b==Max){

        }else if(b==Pin){
            if(s==Gone){
                ui->pin->hide();
            }else if(s==Visible){
                ui->pin->show();
            }else if(s==Other){
                ui->pin->show();
                //update ori
            }
        }
    }

    void DockingPaneContainerNClient::setActive(bool active)
    {
        //theme
        d->active = active;
        if(d->active){
            this->setStyleSheet("ady--DockingPaneContainerNClient{background:rgb(255,242,157)}");
            ui->title->setStyleSheet("background:rgb(255,242,157);color:black");
        }else{
            this->setStyleSheet("ady--DockingPaneContainerNClient{background:rgb(77,96,130)}");
            ui->title->setStyleSheet("background:rgb(77,96,130);color:white");
        }
    }

    void DockingPaneContainerNClient::onClose()
    {
        DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
        DockingPaneContainer::State state = container->state();
        if(state==DockingPaneContainer::Inner){
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
                tabBar->removeContainer(container);
            }
            container->setParent(nullptr);
            container->close();
            container->deleteLater();
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

        }

    }


    void DockingPaneContainerNClient::mouseMoveEvent(QMouseEvent *e)
    {
        QWidget::mouseMoveEvent(e);
        //qDebug()<<"move:"<<d->moving;
        if(d->moving){
            DockingPaneContainer* container = (DockingPaneContainer*)parentWidget();
            //qDebug()<<"parent:"<<parent;
            if(container!=nullptr){

                if(container->state()==DockingPaneContainer::Inner){
                    //fixed create float window;
                    DockingPaneLayoutItemInfo* itemInfo = container->itemInfo();
                    DockingPaneHandle* handle = itemInfo->handle();
                    if(handle!=nullptr){
                        //handle->setCursor(Qt::ArrowCursor);
                        handle->hide();
                    }
                    itemInfo->remove();
                    delete itemInfo;

                    QRect rc = container->geometry();
                    QPoint pos = mapToGlobal(container->pos());

                    int margin = 6;
                    DockingPaneFloatWindow* window = new DockingPaneFloatWindow(container->parentWidget(),margin);
                    QRect rect;
                    rect.setX(pos.x() - margin);
                    rect.setY(pos.y()  - margin);
                    rect.setWidth(rc.width() + margin * 2);
                    rect.setHeight(rc.height() + margin * 2);
                    window->setGeometry(rect);
                    window->setCenterWidget(container);
                    window->updateResizer();
                    window->show();
                }else if(container->state()==DockingPaneContainer::Fixed){
                    //fixed create float window;
                    /*DockingPaneLayoutItemInfo* itemInfo = container->itemInfo();
                    DockingPaneHandle* handle = itemInfo->handle();
                    if(handle!=nullptr){
                        //handle->setCursor(Qt::ArrowCursor);
                        handle->hide();
                    }
                    itemInfo->remove();
                    delete itemInfo;*/
                    DockingPaneFixedWindow* fixed_window = (DockingPaneFixedWindow*)container->parentWidget();
                    //remove sider tabbar items
                    int position = fixed_window->fixedPosition();
                    DockingWorkbench* workbench = (DockingWorkbench*)fixed_window->parentWidget();
                    DockingPaneTabBar* tabBar = workbench->tabBar(position);
                    tabBar->removeContainer(container);

                    container->setParent(nullptr);
                    QRect rc = container->geometry();
                    QPoint pos = mapToGlobal(container->pos());

                    int margin = 6;
                    DockingPaneFloatWindow* window = new DockingPaneFloatWindow(fixed_window->parentWidget(),margin);
                    QRect rect;
                    rect.setX(pos.x() - margin);
                    rect.setY(pos.y()  - margin);
                    rect.setWidth(rc.width() + margin * 2);
                    rect.setHeight(rc.height() + margin * 2);
                    window->setGeometry(rect);
                    window->setCenterWidget(container);
                    window->updateResizer();
                    window->show();

                    fixed_window->hide();
                }else{
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
        }
    }

    void DockingPaneContainerNClient::mousePressEvent(QMouseEvent *e)
    {
        QWidget::mousePressEvent(e);

        d->moving = true;
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
