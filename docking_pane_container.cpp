#include "docking_pane_container.h"
#include "docking_pane_container_nclient.h"
#include "docking_pane_layout_item_info.h"
#include "docking_pane_container_tabbar.h"
#include "docking_pane.h"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

namespace ady {
    class DockingPaneContainerPrivate {
    public:
        QStackedWidget* stacked;
        DockingPaneContainerTabBar* tabbar;
        DockingPaneContainerNClient *nclient = nullptr;
        DockingPaneContainer::State state = DockingPaneContainer::Inner;
        DockingPaneLayoutItemInfo* info = nullptr;
        bool client = false;
    };

    DockingPaneContainer::DockingPaneContainer(QWidget* parent)
        :QWidget(parent)
    {
        setFocusPolicy(Qt::ClickFocus);
        setStyleSheet("ady--DockingPaneContainer{background:white}");//theme
        //ady--DockingPaneContainerNClient

        d = new DockingPaneContainerPrivate;

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setMargin(0);
        layout->setSpacing(0);


        d->nclient = new DockingPaneContainerNClient(this);
        d->stacked = new QStackedWidget(this);
        d->tabbar = new DockingPaneContainerTabBar(this);
        d->tabbar->setAutoHide(true);
        d->tabbar->setShape(QTabBar::RoundedSouth);
        d->tabbar->setExpanding(false);

        connect(d->tabbar,SIGNAL(currentChanged(int )),this,SLOT(onCurrentChanged(int)));
        //connect(d->stacked,SIGNAL(widgetRemoved(int )),this,SLOT(onWidgetRemoved(int)));


        layout->addWidget(d->nclient);
        layout->addWidget(d->stacked);
        layout->addWidget(d->tabbar);

        this->setLayout(layout);

    }

    DockingPaneContainer::DockingPaneContainer(QWidget* parent,bool client)
        :QWidget(parent)
    {
        setFocusPolicy(Qt::ClickFocus);
        d = new DockingPaneContainerPrivate;
        d->client = true;

        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setMargin(0);
        layout->setSpacing(0);
        //d->nclient = new DockingPaneContainerNClient(this);
        d->tabbar = new DockingPaneContainerTabBar(this);
        d->stacked = new QStackedWidget(this);
        //d->tabbar->setAutoHide(true);
        d->tabbar->setShape(QTabBar::RoundedNorth);
        d->tabbar->setExpanding(false);
        connect(d->tabbar,SIGNAL(currentChanged(int )),this,SLOT(onCurrentChanged(int)));
        connect(d->stacked,SIGNAL(widgetRemoved(int )),this,SLOT(onWidgetRemoved(int)));
        //layout->addWidget(d->nclient);
        layout->addWidget(d->tabbar);
        layout->addWidget(d->stacked);
        this->setLayout(layout);
    }

    DockingPaneContainer::~DockingPaneContainer()
    {
        delete d;
        //qDebug()<<"DockingPaneContainer";
    }


    void DockingPaneContainer::appendPane(DockingPane* pane)
    {
        pane->setParent(d->stacked);
        d->stacked->addWidget((QWidget*)pane);
        QString title = pane->windowTitle();
        d->tabbar->addTab(title);
        if(d->nclient!=nullptr){
            d->nclient->updateTitle(title);
        }
    }

    void DockingPaneContainer::insertPane(int index,DockingPane* pane)
    {
        pane->setParent(d->stacked);
        d->stacked->insertWidget(index,(QWidget*)pane);
        if(d->nclient!=nullptr){
            d->nclient->updateTitle(pane->windowTitle());
        }
    }

    void DockingPaneContainer::setPane(int index)
    {
        d->stacked->setCurrentIndex(index);
        DockingPane* pane = static_cast<DockingPane*>(d->stacked->widget(index));
        if(d->nclient!=nullptr){
            d->nclient->updateTitle(pane->windowTitle());
        }
    }

    void DockingPaneContainer::setState(State state)
    {
        d->state = state;
        if(d->nclient!=nullptr){
            if(d->state==Fixed){
                //hide pin update pin ori
                //hide max
                d->nclient->setButtonState(DockingPaneContainerNClient::Pin,DockingPaneContainerNClient::Other);
                d->nclient->setButtonState(DockingPaneContainerNClient::Max,DockingPaneContainerNClient::Gone);
            }else if(d->state==Float){
                //hide pin
                //show max
                d->nclient->setButtonState(DockingPaneContainerNClient::Pin,DockingPaneContainerNClient::Gone);
                d->nclient->setButtonState(DockingPaneContainerNClient::Max,DockingPaneContainerNClient::Visible);
            }else if(d->state==Inner){
                //show pin
                //hide max
                d->nclient->setButtonState(DockingPaneContainerNClient::Pin,DockingPaneContainerNClient::Visible);
                d->nclient->setButtonState(DockingPaneContainerNClient::Max,DockingPaneContainerNClient::Gone);
            }
        }
    }

    DockingPaneContainer::State DockingPaneContainer::state()
    {
        return d->state;
    }

    void DockingPaneContainer::setItemInfo(DockingPaneLayoutItemInfo* info)
    {
        d->info = info;
    }

    DockingPaneLayoutItemInfo* DockingPaneContainer::itemInfo()
    {
        return d->info;
    }

    bool DockingPaneContainer::isClient()
    {
        return d->client;
    }


    int DockingPaneContainer::paneCount()
    {
        return d->stacked->count();
    }

    DockingPane* DockingPaneContainer::pane(int i)
    {
        return (DockingPane*)d->stacked->widget(i);
    }

    DockingPane* DockingPaneContainer::takeAt(int i)
    {
        QWidget* widget = d->stacked->widget(i);
        d->stacked->removeWidget(widget);
        d->tabbar->removeTab(i);
        return (DockingPane*)widget;
    }

    DockingPaneContainerTabBar* DockingPaneContainer::tabBar()
    {
        return d->tabbar;
    }

    QStackedWidget* DockingPaneContainer::stacked()
    {
        return d->stacked;
    }

    void DockingPaneContainer::onCurrentChanged(int i)
    {
        d->stacked->setCurrentIndex(i);
        DockingPane* pane = (DockingPane*)d->stacked->widget(i);
        if(pane!=nullptr){
            if(d->nclient!=nullptr){
                d->nclient->updateTitle(pane->windowTitle());
            }
        }

    }

    void DockingPaneContainer::onWidgetRemoved(int i)
    {
        //qDebug()<<"name:"<<objectName()<<";i:"<<i;
        //d->tabbar->removeTab(i);
    }

     void DockingPaneContainer::focusInEvent(QFocusEvent *event)
     {
         QWidget::focusInEvent(event);

         //set active false for all container children
         DockingWorkbench* workbench = (DockingWorkbench*)parentWidget();
         QObjectList children = parentWidget()->children();
         foreach(QObject* one,children){
             QString name = one->metaObject()->className();
             if(name=="ady::DockingPaneClient"){

             }else if(name=="ady::DockingPaneContainer"){

             }
         }

         if(d->nclient!=nullptr){
             d->nclient->setActive(true);
         }
     }

     /*void DockingPaneContainer::focusOutEvent(QFocusEvent *event)
     {
         QWidget::focusOutEvent(event);
         if(d->nclient!=nullptr){
             d->nclient->setActive(false);
             if(d->state==Fixed){
                 parentWidget()->hide();
             }
         }
     }*/

     void DockingPaneContainer::paintEvent(QPaintEvent *e)
     {
         //QFrame::paintEvent(e);
         Q_UNUSED(e);
         QStyleOption opt;
         opt.init(this);
         QPainter p(this);
         style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
     }


}
