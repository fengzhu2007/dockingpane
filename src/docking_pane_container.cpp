#include "docking_pane_container.h"
#include "docking_pane_container_nclient.h"
#include "docking_pane_layout_item_info.h"
#include "docking_pane_container_tabbar.h"
#include "docking_guide.h"
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
        DockingPaneManager::Position ori_position = DockingPaneManager::S_Left;
        QRect ori_rc;
    };

    DockingPaneContainer::DockingPaneContainer(QWidget* parent,DockingPaneManager::Position position)
        :QWidget(parent)
    {
        setFocusPolicy(Qt::ClickFocus);
        d = new DockingPaneContainerPrivate;
        d->ori_position = position;
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setMargin(1);
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
        this->initView();
    }

    DockingPaneContainer::DockingPaneContainer(QWidget* parent,bool client,bool init_view)
    :QWidget(parent)
    {
        setFocusPolicy(Qt::ClickFocus);
        d = new DockingPaneContainerPrivate;
        d->client = true;
        d->stacked = nullptr;
        d->tabbar = nullptr;
        d->nclient = nullptr;
        d->state = DockingPaneContainer::Inner;
        if(init_view){
            this->initView();
        }
    }

    void DockingPaneContainer::initView(){
        if(d->stacked==nullptr){
            QVBoxLayout* layout = new QVBoxLayout(this);
            layout->setMargin(0);
            layout->setSpacing(0);
            d->tabbar = new DockingPaneContainerTabBar(this);
            d->stacked = new QStackedWidget(this);
            d->tabbar->setShape(QTabBar::RoundedNorth);
            d->tabbar->setExpanding(false);
            connect(d->tabbar,SIGNAL(currentChanged(int )),this,SLOT(onCurrentChanged(int)));
            connect(d->stacked,SIGNAL(widgetRemoved(int )),this,SLOT(onWidgetRemoved(int)));
            layout->addWidget(d->tabbar);
            layout->addWidget(d->stacked);
            this->setLayout(layout);
        }

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
        if(d->nclient!=nullptr && pane!=nullptr){
            d->nclient->updateTitle(pane->windowTitle());
        }
        d->tabbar->setCurrentIndex(index);
        qDebug()<<d->tabbar->children();
    }

    void DockingPaneContainer::setState(State state)
    {
        d->state = state;
        if(d->nclient!=nullptr){
            if(d->state==Fixed){
                //hide pin update pin ori
                //hide max
                d->nclient->setButtonState(DockingPaneContainerNClient::Pin,DockingPaneContainerNClient::Fixed);
                //d->nclient->setButtonState(DockingPaneContainerNClient::Max,DockingPaneContainerNClient::Gone);
            }else if(d->state==Float){
                //hide pin
                //show max
                d->nclient->setButtonState(DockingPaneContainerNClient::Pin,DockingPaneContainerNClient::Float);
                //d->nclient->setButtonState(DockingPaneContainerNClient::Max,DockingPaneContainerNClient::Visible);
            }else if(d->state==Inner){
                //show pin
                //hide max
                d->nclient->setButtonState(DockingPaneContainerNClient::Pin,DockingPaneContainerNClient::Inner);
                //d->nclient->setButtonState(DockingPaneContainerNClient::Max,DockingPaneContainerNClient::Gone);
            }
        }
    }

    DockingPaneContainer::State DockingPaneContainer::state()
    {
        return d->state;
    }


    void DockingPaneContainer::activeWidget(bool active){
        //m_active_state = active;
        //qDebug()<<this<<";"<<active;
        this->setProperty("activeState",active);
        if(d->nclient!=nullptr){
            d->nclient->setActive(active);
            d->nclient->stylePolish();
        }
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

    bool DockingPaneContainer::isClientRegion(){
        return false;
    }


    int DockingPaneContainer::paneCount()
    {
        return d->stacked->count();
    }

    int DockingPaneContainer::current(){
        return d->stacked->currentIndex();
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

    DockingPane* DockingPaneContainer::takeCurrent(){
        return this->takeAt(d->stacked->currentIndex());
    }

    DockingPaneContainerTabBar* DockingPaneContainer::tabBar()
    {
        return d->tabbar;
    }

    QStackedWidget* DockingPaneContainer::stacked()
    {
        return d->stacked;
    }

    void DockingPaneContainer::closeCurrent(){
        if(d!=nullptr&&d->stacked!=nullptr){
            int index = d->stacked->currentIndex();
            DockingPane* pane = this->takeAt(index);
            pane->close();
            index -= 1;
            if(index<0){
                index = 0;
            }
            this->onCurrentChanged(index);
        }
    }

    void DockingPaneContainer::visibleTabBar(bool visible){
        if(d->tabbar!=nullptr){
            if(visible && d->stacked->count()>1){
                d->tabbar->show();
            }else{
                d->tabbar->hide();
            }
        }
    }

    void DockingPaneContainer::setOriPosition(DockingPaneManager::Position position){
        d->ori_position = position;
    }

    DockingPaneManager::Position DockingPaneContainer::oriPosition(){
        return d->ori_position;
    }

    void DockingPaneContainer::setOriRect(QRect rc){
        d->ori_rc = rc;
    }

    void DockingPaneContainer::setOriRect(int w,int h){
        d->ori_rc.setWidth(w);
        d->ori_rc.setHeight(h);
    }

    void DockingPaneContainer::setMoving(bool state){
        if(d->nclient!=nullptr){
            d->nclient->setMoving(state);
        }
    }

    int DockingPaneContainer::guideFlags(){
        int flags = DockingGuide::Center;
        if(d->client){
            if(d->stacked==nullptr || d->stacked->count()==0){
                flags |= DockingGuide::Normal;
            }else{
                flags |= DockingGuide::Large;
                //parent
                DockingPaneLayoutItemInfo* itemInfo = this->itemInfo();
                DockingPaneLayoutItemInfo* previous = itemInfo->previous();
                DockingPaneLayoutItemInfo* next = itemInfo->next();
                bool p_client = false;
                bool n_client = false;
                if(previous!=nullptr){
                    //qDebug()<<"previous:"<<previous;
                    QLayoutItem* item = previous->item();
                    if(item!=nullptr){
                        DockingPaneContainer* pContainer = (DockingPaneContainer*)item->widget();
                        p_client = pContainer->isClient();
                    }

                }
                if(next!=nullptr){
                    QLayoutItem* item = next->item();
                    if(item!=nullptr){
                        DockingPaneContainer* nContainer = (DockingPaneContainer*)item->widget();
                        n_client = nContainer->isClient();
                    }

                }
                DockingPaneLayoutItemInfo* parentIteminfo = itemInfo->parent();
                if(p_client || n_client){

                    if(parentIteminfo->childrenOrientation()==DockingPaneLayoutItemInfo::Horizontal){
                        flags |= DockingGuide::C_Parent_Horizontal;
                        flags |= (DockingGuide::C_Left | DockingGuide::C_Right);
                        if(p_client==false){
                            flags |= DockingGuide::Left;
                        }
                        if(n_client==false){
                            flags |= DockingGuide::Right;
                        }
                        flags |= (DockingGuide::Top | DockingGuide::Bottom);
                    }else{
                        flags |= DockingGuide::C_Parent_Vertical;
                        flags |= (DockingGuide::C_Top | DockingGuide::C_Bottom);
                        if(p_client==false){
                            flags |= DockingGuide::Top;
                        }
                        if(n_client==false){
                            flags |= DockingGuide::Bottom;
                        }
                        flags |= (DockingGuide::Left | DockingGuide::Right);
                    }
                }else{
                    flags |= (DockingGuide::C_Left | DockingGuide::C_Right | DockingGuide::C_Top | DockingGuide::C_Bottom | DockingGuide::Left | DockingGuide::Top | DockingGuide::Right | DockingGuide::Bottom);
                }

                //flags |= (DockingGuide::Left | DockingGuide::Top | DockingGuide::Right | DockingGuide::Bottom);

                //int client_count = 0;
            }
        }else{
            flags |= (DockingGuide::Normal | DockingGuide::Left | DockingGuide::Top | DockingGuide::Right | DockingGuide::Bottom);
        }
        return flags;
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
