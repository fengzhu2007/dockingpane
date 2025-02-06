#include "docking_pane_client.h"
#include "docking_workbench.h"
#include "docking_pane_container_tabbar.h"
#include "docking_pane_layout_item_info.h"
#include "docking_pane.h"
#include <QStyleOption>
#include <QPainter>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>
namespace ady {
class DockingPaneClientPrivate{
public:
    bool init_view=false;
    std::function<void(QDropEvent*)> func;

};

    DockingPaneClient::DockingPaneClient(DockingWorkbench* parent,bool init_view)
        :DockingPaneContainer(parent,true,init_view)
    {
        d = new DockingPaneClientPrivate;
        d->init_view = init_view;
        d->func = nullptr;
        if(init_view){
            DockingPaneContainerTabBar* tabBar = this->tabBar();
            tabBar->setTabsClosable(true);
            connect(tabBar,&QTabBar::tabCloseRequested,this,&DockingPaneClient::onTabClose);
            this->stacked()->hide();

        }
    }

    DockingPaneClient::~DockingPaneClient(){
        delete d;
    }

    void DockingPaneClient::initView(){
        DockingPaneContainer::initView();
        if(d->init_view==false){
            DockingPaneContainerTabBar* tabBar = this->tabBar();
            tabBar->setDropCallback(d->func);
            tabBar->setTabsClosable(true);
            connect(tabBar,&QTabBar::tabCloseRequested,this,&DockingPaneClient::onTabClose);
        }
        d->init_view = true;
    }

    void DockingPaneClient::setDropCallback(std::function<void(QDropEvent*)> func){
        this->setAcceptDrops(func!=nullptr);
        d->func = func;
        auto tab = this->tabBar();
        if(tab!=nullptr){
            tab->setDropCallback(func);
        }
    }

    std::function<void(QDropEvent*)> DockingPaneClient::dropCallback(){
        return d->func;
    }

    void DockingPaneClient::onTabClose(int i){
        DockingPane* pane = this->pane(i);
        if(pane!=nullptr){
            DockingWorkbench* workbench = this->workbench();
            bool isClient = this->isClient();
            QString id = pane->id();
            QString group = pane->group();
            bool closeEnable = pane->closeEnable();
            //workbench->beforePaneClose(pane,isClient);
            if(closeEnable==false){
                //stop close pane
                pane->setCloseEnable(closeEnable);//restore close enable
                return ;
            }
            pane = this->takeAt(i);
            pane->close();
            //pane->deleteLater();
            //delete pane;

            workbench->paneClosed(id,group,isClient);
            //remove client container;
            DockingPaneLayoutItemInfo* itemInfo = this->itemInfo();
            DockingPaneLayoutItemInfo* parentItemInfo = itemInfo->parent();
            //qDebug()<<"itemInfo:"<<itemInfo<<";parentItemInfo:"<<parentItemInfo;
            //qDebug()<<"size:"<<parentItemInfo->clientChildren().size();
            if(parentItemInfo->clientChildren().size()>1){
                if(this->paneCount()==0){
                    parentItemInfo->removeItem(itemInfo);
                    //delete itemInfo;
                    this->hide();
                    //d->del_parent = true;
                    this->deleteLater();
                }
            }
        }
    }

    void DockingPaneClient::focusInEvent(QFocusEvent *event)
    {
        QWidget::focusInEvent(event);

    }

    void DockingPaneClient::focusOutEvent(QFocusEvent *event)
    {
        QWidget::focusOutEvent(event);
    }

    void DockingPaneClient::paintEvent(QPaintEvent *e)
    {
        //QFrame::paintEvent(e);
        Q_UNUSED(e);
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

    void DockingPaneClient::dragEnterEvent(QDragEnterEvent *event){
        if (event->mimeData()->hasUrls()) {
            event->acceptProposedAction();
        } else {
            event->ignore();
        }
    }

    void DockingPaneClient::dragMoveEvent(QDragMoveEvent *event){
         event->accept();
    }

    void DockingPaneClient::dropEvent(QDropEvent *event){
        if(d->func!=nullptr){
            d->func(event);
        }
    }

}
