#include "docking_workbench.h"
#include "docking_pane_client.h"
#include "docking_guide.h"
//#include "docking_client_guide.h"
#include "docking_guide_diamond.h"
#include "docking_guide_cover.h"
#include "docking_pane_layout_item_info.h"
#include "docking_pane_layout.h"
#include "docking_pane_float_window.h"
#include "docking_pane_fixed_window.h"
#include "docking_pane_tabbar.h"
#include "docking_pane.h"
#include "qss.h"
#include <QLayout>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>
#include <QStyle>
#include <QTimer>
#include <QDebug>

namespace ady {
    class DockingWorkbenchPrivate {
    public:
        //QSplitter* vertical_splitter;//level 1
        DockingGuide* guide = nullptr;
        //DockingClientGuide* client_guide = nullptr;
        DockingGuideDiamond* siders[4];
        bool guide_visibility = false;
        QList<DockingPaneContainer*> children;
        bool lookup = false;
        DockingGuideCover* cover = nullptr;
        DockingPaneTabBar* tabBars[4];
        //DockingPaneClient* client = nullptr;
        QList<DockingPaneClient*> clients;
        DockingPaneFixedWindow* fixed_window = nullptr;
        QTimer* timer = nullptr;
    };


    DockingWorkbench::DockingWorkbench(QWidget* parent)
        :QFrame(parent){
        //setStyleSheet(".ady--DockingWorkbench{background:#eeeef2}");//theme
        setStyleSheet(QSS::global());
        d = new DockingWorkbenchPrivate();
        d->timer = new QTimer(this);
        connect(d->timer,&QTimer::timeout,this,&DockingWorkbench::onTimeout);
        for(int i=0;i<4;i++){
            d->tabBars[i] = new DockingPaneTabBar(this);
            d->tabBars[i]->setPosition((DockingPaneManager::Position)i);
        }
        //init client container
    }

    void DockingWorkbench::initClient()
    {
        auto client = new DockingPaneClient(this,false);
        client->setObjectName("client_container");
        DockingPaneLayout* layout = (DockingPaneLayout*)this->layout();
        //layout->setMargin(3);
        assert(layout!=nullptr);
        layout->addItem(client,DockingPaneManager::Center);
        //qDebug()<<"insertItem";
        //d->clients.append(client);
    }


    DockingWorkbench::~DockingWorkbench(){
        delete d;
    }

    void DockingWorkbench::showGuide(DockingPaneContainer* widget,QRect rect)
    {
        QWidget*g = nullptr;
        if(d->guide==nullptr){
            d->guide = new DockingGuide(this);
            d->guide->raise();
            for(int i=0;i<4;i++){
                d->siders[i] = new DockingGuideDiamond((DockingPaneManager::Position)i,this);
                d->siders[i]->setWindowFlags(Qt::CoverWindow|Qt::Tool|Qt::FramelessWindowHint);
                d->siders[i]->setMaximumSize(40,40);
            }
        }
        int flags = widget->guideFlags();
        d->guide->setSizeMode(flags);
        //qDebug()<<"widget container:"<<widget;
        {
            QPoint pos = widget!=nullptr?widget->pos():this->pos();
            QPoint globalPos = this->mapToGlobal(pos);
            int offsetX = globalPos.x();
            int offsetY = globalPos.y();

            QRect guide_rect = d->guide->geometry();
            int width = guide_rect.width();
            int height = guide_rect.height();
            guide_rect.setX((rect.width() - width)/2 + offsetX);
            guide_rect.setY((rect.height() - height)/2 + offsetY);
            guide_rect.setWidth(width);
            guide_rect.setHeight(height);
            d->guide->setGeometry(guide_rect);
        }
        d->guide->show();

    }

    void DockingWorkbench::showSiderGuide()
    {
        if(d->guide_visibility==true){
            return ;
        }
        if(d->guide!=nullptr){
            d->guide_visibility = true;
            QPoint pos = this->pos();
            QPoint globalPos = this->mapToGlobal(pos);
            int offsetX = globalPos.x();
            int offsetY = globalPos.y();
            QRect rect = geometry();
            for(int i=0;i<4;i++){
                d->siders[i]->show();
                QRect child_rect = d->siders[i]->geometry();
                int width = child_rect.width();
                int height = child_rect.height();
                DockingPaneManager::Position position = (DockingPaneManager::Position)i;
                if(position==DockingPaneManager::S_Top){
                    child_rect.setX((rect.width() - width)/2 + offsetX);
                    child_rect.setY(0 + offsetY);
                }else if(position==DockingPaneManager::S_Bottom){
                    child_rect.setX((rect.width() - width)/2 + offsetX);
                    child_rect.setY((rect.height() - height + offsetY));
                }else if(position==DockingPaneManager::S_Left){
                    child_rect.setX(0 + offsetX);
                    child_rect.setY((rect.height() - height)/2 + offsetY);
                }else if(position==DockingPaneManager::S_Right){
                    child_rect.setX((rect.width() - width) + offsetX);
                    child_rect.setY((rect.height() - height)/2 + offsetY);
                }
                child_rect.setWidth(width);
                child_rect.setHeight(height);
                d->siders[i]->setGeometry(child_rect);
            }

        }

    }

    void DockingWorkbench::hideGuide()
    {

        if(d->guide!=nullptr){
            d->guide->hide();
        }
    }

    void DockingWorkbench::hideSiderGuide()
    {
        d->guide_visibility = false;
        if(d->guide!=nullptr){
            for(int i=0;i<4;i++){
                d->siders[i]->hide();
            }
        }
        hideGuideCover();
    }

    int DockingWorkbench::activeGuide(const QPoint& pos )
    {
        if(d->guide!=nullptr && d->guide->isHidden()==false){
            DockingGuideDiamond* diamond = d->guide->updateActive(pos);
            if(diamond!=nullptr){
                return diamond->position();
            }
        }
        if(d->guide!=nullptr){
            for(int i=0;i<4;i++){
                if(d->siders[i]->updateActive(pos)){
                    return d->siders[i]->position();
                }
            }
        }
        return -1;
    }

    int DockingWorkbench::activeSiderGuide(const QPoint& pos)
    {
        if(d->guide!=nullptr){
            for(int i=0;i<4;i++){
                if(d->siders[i]->updateActive(pos)){
                    return d->siders[i]->position();
                }
            }
        }
        return -1;
    }

    void DockingWorkbench::startLookup()
    {
        if(d->lookup==false){
            d->lookup = true;
            QObjectList lists = children();
            //Q_FOREACH()
            d->children.clear();
            foreach(auto one,lists){
               QString name = one->metaObject()->className();
               if(name=="ady::DockingPaneContainer" || name=="ady::DockingPaneClient"){
                   d->children.push_back((DockingPaneContainer*)one);
               }
            }
        }


    }
    void DockingWorkbench::endLookup()
    {
        d->lookup = false;
        d->children.clear();
    }

    DockingPaneContainer* DockingWorkbench::lookup(const QPoint& pos,QRect& rect,bool &guide_visibility)
    {
        QRect rc = geometry();
        QPoint workbenchPos = mapToGlobal(this->pos());
        int offsetX = workbenchPos.x();
        int offsetY = workbenchPos.y();

        //qDebug()<<"rc:"<<rc<<";pos:"<<pos<<";rect:"<<rect;
        QPoint p(pos.x() - offsetX,pos.y() - offsetY);
        foreach(auto one,d->children){
            if(one->isHidden()){
               continue;
            }
            QRect rc = one->geometry();
            //qDebug()<<"rc:"<<rc<<";pos:"<<p<<";one:"<<one;
            if(rc.contains(p)){
                rect = rc;
                guide_visibility = true;
                //qDebug()<<"find ok:"<<one;
                return one;
            }
        }
        if(d->children.size()==0){
            guide_visibility = true;
            rect = rc;
        }
        return nullptr;
    }

    void DockingWorkbench::showGuideCover(DockingPaneContainer* container,int position,const QRect& rect)
    {
        if(d->cover==nullptr){
            d->cover = new DockingGuideCover(this);
        }


        DockingPaneLayout* layout = (DockingPaneLayout*)this->layout();
        DockingPaneLayoutItemInfo* rootItem = layout->rootItem();
        DockingPaneLayoutItemInfo::Orientation rootItemOrientation = rootItem->childrenOrientation();
        QPoint workbenchPos = mapToGlobal(this->pos());
        QRect coverRect;

        /*if(container==nullptr && position>=4){
            position -= 4;
            if(position==4){
                position = 0;
            }
        }*/

        if(position==DockingPaneManager::S_Left){
            QRect rc = geometry();
            int w = rc.width()/2;
            if(rect.width()<w){
                w = rect.width();
            }
            coverRect = QRect(workbenchPos.x(),workbenchPos.y(),w,rc.height());
            /*coverRect.setX(workbenchPos.x());
            coverRect.setY(workbenchPos.y());
            coverRect.setWidth(rect.width());
            coverRect.setHeight(geometry().height());*/

            //qDebug()<<"rootItemOrientation:"<<rootItemOrientation;
           // qDebug()<<"rootItemOrientation size:"<<rootItem->childrenCount();
            if(rootItemOrientation==DockingPaneLayoutItemInfo::Unkown || rootItemOrientation==DockingPaneLayoutItemInfo::Horizontal){

            }else{
                //coverRect.setWidth(rc.width() / 2);
            }
            //qDebug()<<"cover:"<<coverRect<<";rect:"<<rect<<";workbench rc:"<<rc;

        }else if(position==DockingPaneManager::S_Top){
            QRect rc = geometry();
            int h = rc.height()/2;
            if(rect.height()<h){
                h = rect.height();
            }
            coverRect = QRect(workbenchPos.x(),workbenchPos.y(),rc.width(),h);


            /*coverRect.setX(workbenchPos.x());
            coverRect.setY(workbenchPos.y());
            coverRect.setWidth(geometry().width());
            coverRect.setHeight(rect.height());*/
            if(rootItemOrientation==DockingPaneLayoutItemInfo::Unkown || rootItemOrientation==DockingPaneLayoutItemInfo::Vertical){

            }else{
                //coverRect.setHeight(rc.height() / 2);
            }


        }else if(position==DockingPaneManager::S_Right){
            QRect rc = geometry();
            int w = rc.width()/2;
            if(rect.width()<w){
                w = rect.width();
            }
            int x = workbenchPos.x() + rc.width() - w;
            int y = workbenchPos.y();
            coverRect = QRect(x,y,w,rc.height());

            /*coverRect.setX(workbenchPos.x() + rc.width() - rect.width());
            coverRect.setY(workbenchPos.y());
            coverRect.setWidth(w);
            coverRect.setHeight(rc.height());*/

            if(rootItemOrientation==DockingPaneLayoutItemInfo::Unkown || rootItemOrientation==DockingPaneLayoutItemInfo::Horizontal){

            }else{
                //coverRect.setWidth(rc.width() / 2);
            }
            //qDebug()<<"cover:"<<coverRect<<";rect:"<<rect<<";workbench rc:"<<rc;

        }else if(position==DockingPaneManager::S_Bottom){
            QRect rc = geometry();
            int h = rc.height()/2;
            if(rect.height()<h){
                h = rect.height();
            }
            int x = workbenchPos.x();
            int y = workbenchPos.y() + rc.height() - h;
            coverRect = QRect(x,y,rc.width(),h);

            /*coverRect.setX(workbenchPos.x());
            coverRect.setY(workbenchPos.y() + rc.height() - rect.height());
            coverRect.setWidth(rc.width());
            coverRect.setHeight(rect.height());*/

            if(rootItemOrientation==DockingPaneLayoutItemInfo::Unkown || rootItemOrientation==DockingPaneLayoutItemInfo::Vertical){

            }else{
                //coverRect.setY(workbenchPos.y() + rc.height() /2);
                //coverRect.setHeight(rc.height() / 2);
            }
        }else{
            DockingPaneLayoutItemInfo* itemInfo = container->itemInfo();
            DockingPaneLayoutItemInfo* parentItemInfo = itemInfo->parent();
            //DockingPaneLayoutItemInfo::Orientation parentItemOrientation = parentItemInfo->childrenOrientation();

            int flags = d->guide->sizeMode();
            QPoint containerPos = mapToGlobal(container->pos());
            QRect rc = container->geometry();

            if(position==DockingPaneManager::Left || position==DockingPaneManager::C_Left){

                if((flags&DockingGuide::C_Parent_Horizontal)>0){
                   coverRect.setX(containerPos.x());
                   coverRect.setY(containerPos.y());
                   coverRect.setWidth(rc.width() / 2);
                   coverRect.setHeight(rc.height());
                }else if((flags&DockingGuide::C_Parent_Vertical)>0){
                   QRect rc = parentItemInfo->clientGeometry();
                   QPoint startPos = mapToGlobal(QPoint(rc.x(),rc.y()));
                   coverRect.setX(startPos.x());
                   coverRect.setY(startPos.y());
                   coverRect.setWidth(rc.width()/2);
                   coverRect.setHeight(rc.height());
                }else{
                   coverRect.setX(containerPos.x());
                   coverRect.setY(containerPos.y());
                   coverRect.setWidth(rc.width() / 2);
                   coverRect.setHeight(rc.height());
                }
            }else if(position==DockingPaneManager::Top || position==DockingPaneManager::C_Top){

                if((flags&DockingGuide::C_Parent_Horizontal)>0){
                   QRect rc = parentItemInfo->clientGeometry();
                   QPoint startPos = mapToGlobal(QPoint(rc.x(),rc.y()));
                   coverRect.setX(startPos.x());
                   coverRect.setY(startPos.y());
                   coverRect.setWidth(rc.width());
                   coverRect.setHeight(rc.height()/2);
                }else if((flags&DockingGuide::C_Parent_Vertical)>0){

                   coverRect.setX(containerPos.x());
                   coverRect.setY(containerPos.y());
                   coverRect.setWidth(rc.width());
                   coverRect.setHeight(rc.height() / 2);

                }else{
                   coverRect.setX(containerPos.x());
                   coverRect.setY(containerPos.y());
                   coverRect.setWidth(rc.width());
                   coverRect.setHeight(rc.height() / 2);
                }


            }else if(position==DockingPaneManager::Right || position==DockingPaneManager::C_Right){

                if((flags&DockingGuide::C_Parent_Horizontal)>0){
                   coverRect.setX(containerPos.x() + rc.width() /2);
                   coverRect.setY(containerPos.y());
                   coverRect.setWidth(rc.width() / 2);
                   coverRect.setHeight(rc.height());
                }else if((flags&DockingGuide::C_Parent_Vertical)>0){
                   QRect rc = parentItemInfo->clientGeometry();
                   QPoint startPos = mapToGlobal(QPoint(rc.x(),rc.y()));
                   coverRect.setX(startPos.x()+rc.width()/2);
                   coverRect.setY(startPos.y());
                   coverRect.setWidth(rc.width()/2);
                   coverRect.setHeight(rc.height());
                }else{
                   coverRect.setX(containerPos.x() + rc.width() /2);
                   coverRect.setY(containerPos.y());
                   coverRect.setWidth(rc.width() / 2);
                   coverRect.setHeight(rc.height());
                }
            }else if(position==DockingPaneManager::Bottom || position==DockingPaneManager::C_Bottom){
                if((flags&DockingGuide::C_Parent_Horizontal)>0){
                   QRect rc = parentItemInfo->clientGeometry();
                   QPoint startPos = mapToGlobal(QPoint(rc.x(),rc.y()));
                   coverRect.setX(startPos.x());
                   coverRect.setY(startPos.y() + rc.height()/2);
                   coverRect.setWidth(rc.width());
                   coverRect.setHeight(rc.height()/2);
                }else if((flags&DockingGuide::C_Parent_Vertical)>0){
                   coverRect.setX(containerPos.x());
                   coverRect.setY(containerPos.y() + rc.height() / 2);
                   coverRect.setWidth(rc.width() );
                   coverRect.setHeight(rc.height() / 2);
                }else{
                   coverRect.setX(containerPos.x());
                   coverRect.setY(containerPos.y() + rc.height() / 2);
                   coverRect.setWidth(rc.width() );
                   coverRect.setHeight(rc.height() / 2);
                }
            }else if(position==DockingPaneManager::Center){
                coverRect.setX(containerPos.x());
                coverRect.setY(containerPos.y());
                coverRect.setWidth(rc.width() );
                coverRect.setHeight(rc.height());
            }else{
                d->cover->hide();
                return;
            }
        }
        d->cover->setGeometry(coverRect);
        d->cover->raise();
        d->cover->show();
    }

    void DockingWorkbench::lockContainer(DockingPaneFloatWindow* window,DockingPaneContainer* container,int position)
    {
        DockingPaneLayout* layout = (DockingPaneLayout*)this->layout();

        DockingPaneContainer* widget = window->centerWidget();
        widget->setState(DockingPaneContainer::Inner);
        widget->setParent(this);

        if(container==nullptr && position>=4){
            position -= 4;
            if(position==4){
                position = 0;
            }
        }


        if(position==DockingPaneManager::S_Left || position==DockingPaneManager::S_Top || position==DockingPaneManager::S_Right || position==DockingPaneManager::S_Bottom){
            //layout->addItem()
            //qDebug()<<"lockContainer:"<<position;
            layout->addItem(widget,(DockingPaneManager::Position)position);

        }else if(position==DockingPaneManager::Left || position==DockingPaneManager::Top || position==DockingPaneManager::Right ||position==DockingPaneManager::Bottom){
            DockingPaneLayoutItemInfo* relation = container->itemInfo();

            int flags = d->guide->sizeMode();
            qDebug()<<"flags:"<<flags <<";"<< (flags & DockingGuide::C_Parent_Horizontal)<<";"<<(flags & DockingGuide::C_Parent_Vertical);
            if((flags & DockingGuide::C_Parent_Horizontal)>0 && (position==DockingPaneManager::Top || position==DockingPaneManager::Bottom)){
                DockingPaneLayoutItemInfo* parentRelation = relation->parent();
                if(parentRelation->childrenOrientation()!=DockingPaneLayoutItemInfo::Vertical){
                    //new two item info
                    //add widget item info
                    QList<DockingPaneLayoutItemInfo*>list = parentRelation->clientChildren();
                    if(list.size()>0){
                        DockingPaneLayoutItemInfo* first = list.at(0);
                        DockingPaneContainer* fc = (DockingPaneContainer*)first->item()->widget();
                        layout->addItem(widget,first,(DockingPaneManager::Position)position);
                        QList<DockingPaneContainer*>containers;
                        for(int i=1;i<list.size();i++){
                            DockingPaneContainer* w = (DockingPaneContainer*)list.at(i)->item()->widget();
                            containers.push_back(w);
                            DockingPaneLayoutItemInfo* itemInfo1 = w->itemInfo();
                            parentRelation->removeItem(itemInfo1);
                        }
                        if(parentRelation->isEmpty()){
                            DockingPaneLayoutItemInfo* lii = parentRelation;
                            parentRelation = lii->parent();
                            parentRelation->removeItem(lii);
                        }
                        DockingPaneLayoutItemInfo* rl = parentRelation->findItemInfo(fc);
                        foreach (auto one, containers) {
                            layout->addItem(one,rl,DockingPaneManager::Right);
                        }
                        /*DockingPaneLayoutItemInfo* first = list.at(0);
                        DockingPaneContainer* fc = (DockingPaneContainer*)first->item()->widget();
                        layout->addItem(widget,first,(DockingPaneManager::Position)position);

                        DockingPaneLayoutItemInfo* new_first = first->findItemInfo(fc);
                        for(int i=1;i<list.size();i++){
                            QWidget* w = list.at(i)->item()->widget();
                            parentRelation->takeAt((DockingPaneContainer*)w);
                            layout->addItem((DockingPaneContainer*)w,new_first,DockingPaneManager::Right);
                        }*/
                        //first->dump("");
                    }else{
                        return ;
                    }

                }else{
                    layout->addItem(widget,parentRelation,(DockingPaneManager::Position)position);
                }

            }else if((flags & DockingGuide::C_Parent_Vertical)>0 && (position==DockingPaneManager::Left || position==DockingPaneManager::Right)){
                DockingPaneLayoutItemInfo* parentRelation = relation->parent();
                if(parentRelation->childrenOrientation()!=DockingPaneLayoutItemInfo::Horizontal){
                    QList<DockingPaneLayoutItemInfo*>list = parentRelation->clientChildren();
                    if(list.size()>0){
                        DockingPaneLayoutItemInfo* first = list.at(0);
                        DockingPaneContainer* fc = (DockingPaneContainer*)first->item()->widget();
                        layout->addItem(widget,first,(DockingPaneManager::Position)position);
                        QList<DockingPaneContainer*>containers;
                        for(int i=1;i<list.size();i++){
                            DockingPaneContainer* w = (DockingPaneContainer*)list.at(i)->item()->widget();
                            containers.push_back(w);
                            DockingPaneLayoutItemInfo* itemInfo1 = w->itemInfo();
                            parentRelation->removeItem(itemInfo1);
                        }
                        if(parentRelation->isEmpty()){
                            DockingPaneLayoutItemInfo* lii = parentRelation;
                            parentRelation = lii->parent();
                            parentRelation->removeItem(lii);
                        }
                        DockingPaneLayoutItemInfo* rl = parentRelation->findItemInfo(fc);
                        foreach (auto one, containers) {
                            layout->addItem(one,rl,DockingPaneManager::Bottom);
                        }
                        //parentRelation->dump("result");
                    }else{
                        return ;
                    }

                }else{
                    layout->addItem(widget,parentRelation,(DockingPaneManager::Position)position);
                }

            }else{
                qDebug()<<"lockContainer:"<<container<<"relation"<<relation<<";pos:"<<position;
                //this->dump("result");
                layout->addItem(widget,relation,(DockingPaneManager::Position)position);
            }

        }else if(position==DockingPaneManager::C_Left || position==DockingPaneManager::C_Top || position==DockingPaneManager::C_Right ||position==DockingPaneManager::C_Bottom){
            DockingPaneClient* client_container = new DockingPaneClient(this,true);
            int count = widget->paneCount();
            for(int i=0;i<count;i++){
                DockingPane* pane = widget->takeAt(0);
                client_container->appendPane(pane);
                if(i==0){
                   client_container->setObjectName(pane->id()+"_client_container");
                }
            }
            DockingPaneLayoutItemInfo* relation = container->itemInfo();
            layout->addItem(client_container,relation,(DockingPaneManager::Position)position);
            widget->close();
            widget->deleteLater();

        }else{
            //center
            //container->initView();
            window->hide();
            int size = widget->paneCount();
            for(int i=0;i<size;i++){
                DockingPane* pane = widget->pane(0);
                container->appendPane(pane);
                if(i==0){
                   container->setPane(container->paneCount()-1);
                }
            }
            widget->close();
            widget->deleteLater();
            this->unActiveAll();
            container->activeWidget(true);
        }

        layout->update();
        window->hide();
        window->deleteLater();

    }

    void DockingWorkbench::restoreWidget(DockingPaneContainer* widget,int position){
        DockingPaneLayout* layout = (DockingPaneLayout*)this->layout();
        widget->setState(DockingPaneContainer::Inner);
        //qDebug()<<"restoreWidget:"<<position;
        if(position==DockingPaneManager::S_Left || position==DockingPaneManager::S_Top || position==DockingPaneManager::S_Right || position==DockingPaneManager::S_Bottom){
            //layout->addItem()
            //sider tabbar remove item
            DockingPaneTabBar* tabBar = this->tabBar(position);
            tabBar->removeContainer(widget);
            //d->fixed_window->close();
            //delete d->fixed_window;
            //d->fixed_window = nullptr;
            widget->setParent(this);
            layout->addItem(widget,(DockingPaneManager::Position)position);
            widget->activeWidget(false);
        }
        //d->fixed_window = nullptr;
        d->fixed_window->hide();
        layout->update();
    }

    void DockingWorkbench::restorePane(DockingPane* pane,int position){
        DockingPaneLayout* layout = (DockingPaneLayout*)this->layout();
        DockingPaneContainer* widget = new DockingPaneContainer(this);
        widget->setState(DockingPaneContainer::Inner);
        widget->appendPane(pane);
        widget->setOriPosition((DockingPaneManager::Position)position);

        if(position==DockingPaneManager::S_Left || position==DockingPaneManager::S_Top || position==DockingPaneManager::S_Right || position==DockingPaneManager::S_Bottom){
            //layout->addItem()


            //sider tabbar remove item
            //DockingPaneTabBar* tabBar = this->tabBar(position);

            //tabBar->removeContainer(widget);
            //d->fixed_window->close();
            //delete d->fixed_window;
            //d->fixed_window = nullptr;
            //widget->setParent(this);
            layout->addItem(widget,(DockingPaneManager::Position)position);
            widget->activeWidget(false);

        }else if(position==DockingPaneManager::Center){
            DockingPaneClient* client = this->client();
            if(client!=nullptr){
                client->initView();
                client->appendPane(pane);
            }
        }
        //d->fixed_window = nullptr;
        layout->update();
    }

    void DockingWorkbench::hideGuideCover()
    {
        if(d->cover!=nullptr){
            d->cover->hide();
        }
    }

    DockingPaneTabBar* DockingWorkbench::tabBar(int position)
    {
        return d->tabBars[position];
    }

    QSize DockingWorkbench::tabBarSize(int position)
    {
        if(d->tabBars[position]->isVisible()){
            QSize size = d->tabBars[position]->size();
            if(d->tabBars[position]->count()>0){
                if(position==DockingPaneManager::S_Left || position==DockingPaneManager::S_Right){
                   size.setWidth(30);//theme
                }else if(position==DockingPaneManager::S_Top || position==DockingPaneManager::S_Bottom){
                   size.setHeight(30);//theme
                }
            }
            return size;
        }else{
            return QSize(0,0);
        }
        //return d->tabBars[position]->isVisible()?d->tabBars[position]->size():QSize(0,0);
    }

    void DockingWorkbench::siderFixed(DockingPaneContainer* container,int position)
    {
        DockingPaneLayoutItemInfo* itemInfo = container->itemInfo();
        if(itemInfo!=nullptr){
            DockingPaneLayoutItemInfo* l = itemInfo->level0();
            QRect rc = l->geometry(l->spacing());
            DockingPaneClient* client = this->client();
            QRect rect = client->geometry();

            if(rc.contains(rect)){
                //compare client rect and container rect
                rc = container->geometry();
                if(l->parent()->childrenOrientation()==DockingPaneLayoutItemInfo::Horizontal){
                    if(rc.y() + rc.height() < rect.y()){
                        //top
                        position = DockingPaneManager::S_Top;
                    }else{
                        //bottom
                        position = DockingPaneManager::S_Bottom;
                    }

                }else{
                    if(rc.x() + rc.width() < rect.x()){
                        //left
                        position = DockingPaneManager::S_Left;
                    }else{
                        //right
                        position = DockingPaneManager::S_Right;
                    }
                }
            }else{
                if(l->parent()->childrenOrientation()==DockingPaneLayoutItemInfo::Horizontal){
                    if(rc.x() + rc.width() < rect.x()){
                        //left
                        position = DockingPaneManager::S_Left;
                    }else{
                        //right
                        position = DockingPaneManager::S_Right;
                    }
                }else{
                    if(rc.y() + rc.height() < rect.y()){
                        //top
                        position = DockingPaneManager::S_Top;
                    }else{
                        //bottom
                        position = DockingPaneManager::S_Bottom;
                    }
                }
            }
            container->setItemInfo(nullptr);
            itemInfo->remove();//remove self
            delete itemInfo;
        }
        container->setParent(nullptr);
        DockingPaneTabBar* tabBar = d->tabBars[position];
        QRect r = geometry();
        if(position==DockingPaneManager::S_Top || position==DockingPaneManager::S_Bottom){
            tabBar->setGeometry(0,0,r.width(),30);
        }else if(position==DockingPaneManager::S_Left || position==DockingPaneManager::S_Right){
            tabBar->setGeometry(0,0,30,r.height());
        }
        tabBar->addContainer(container);
        //qDebug()<<"size:"<<size();
        updateTabBars(size());

        this->layout()->update();
    }

    void DockingWorkbench::updateTabBars(const QSize& size)
    {
        QSize left = tabBarSize(DockingPaneManager::S_Left);
        QSize top = tabBarSize(DockingPaneManager::S_Top);
        QSize right = tabBarSize(DockingPaneManager::S_Right);
        QSize bottom = tabBarSize(DockingPaneManager::S_Bottom);
        qDebug()<<"updateTabBars"<<left<<top<<right<<bottom;

        {
            int x = 0;
            int y = top.height();
            d->tabBars[DockingPaneManager::S_Left]->setGeometry(x,y,left.width(),size.height() - top.height() - bottom.height());
        }

        {
            int x = left.width();
            int y = 0;
            d->tabBars[DockingPaneManager::S_Top]->setGeometry(x,y,size.width() - left.width() - right.width(),top.height());
        }

        {
            int x = size.width() - right.width();
            int y = top.height();
            d->tabBars[DockingPaneManager::S_Right]->setGeometry(x,y,right.width(),size.height() - top.height() - bottom.height());
        }

        {
            int x = left.width();
            int y = size.height() - bottom.height();
            d->tabBars[DockingPaneManager::S_Bottom]->setGeometry(x,y,size.width() - left.width() - right.width(),bottom.height());
        }
    }

    QList<DockingPaneContainer*> DockingWorkbench::containers()
    {
        QList<DockingPaneContainer*> children;
        QObjectList lists = this->children();
        foreach(auto one,lists){
           QString name = one->metaObject()->className();
           if(name=="ady::DockingPaneContainer"){
               children.push_back((DockingPaneContainer*)one);
           }
        }
        return children;
    }

    DockingPaneClient* DockingWorkbench::client(int index)
    {
        QObjectList objectlist = children();
        int i = 0;
        foreach(auto one,objectlist){
           QString name = one->metaObject()->className();
           if(name=="ady::DockingPaneClient"){
               if(i==index){
                    return (DockingPaneClient*)one;
               }
               i+=1;
           }
        }
        return nullptr;
    }

    void DockingWorkbench::showFixedWindow(DockingPaneContainer* container,int position)
    {
        if(d->fixed_window==nullptr){
            d->fixed_window = new DockingPaneFixedWindow(this,5);
            d->fixed_window->setCenterWidget(container);
        }else{
            d->fixed_window->setCenterWidget(container);
            //d->fixed_window->show();
        }
        d->fixed_window->setFixedPosition(position);
        d->fixed_window->raise();

        //container->setFocus();
        //qDebug()<<"showFixedWindow"<<container->geometry();
        QRect rect = geometry();

        QSize left = tabBarSize(DockingPaneManager::S_Left);
        QSize top = tabBarSize(DockingPaneManager::S_Top);
        QSize right = tabBarSize(DockingPaneManager::S_Right);
        QSize bottom = tabBarSize(DockingPaneManager::S_Bottom);
        //qDebug()<<left<<top<<right<<bottom<<rect;


        //QPoint pos = this->pos();
        QRect ori_rc = container->oriRect();
        QRect win_rc;
        QPoint pos = QPoint(0,0);
        if(position==DockingPaneManager::S_Left){
            int width = ori_rc.width();
            if(width<=0){
               width = 200;
            }
            win_rc = QRect(0,0,width,rect.height() - top.height() - bottom.height());
            pos.rx() += left.width();
            pos.ry() += top.height();
        }else if(position==DockingPaneManager::S_Top){
            int height = ori_rc.height();
            if(height<=0){
               height = 200;
            }
            win_rc = QRect(0,0,rect.width() - left.width() - right.width() ,height);
            pos.rx() += left.width();
            pos.ry() += top.height();

        }else if(position==DockingPaneManager::S_Right){
            int width = ori_rc.width();
            if(width<=0){
               width = 200;
            }
            win_rc = QRect(0,0,width,rect.height() - top.height() - bottom.height());
            pos.rx() += (rect.width() - right.width() - width);
            pos.ry() += top.height();
        }else if(position==DockingPaneManager::S_Bottom){
            int height = ori_rc.height();
            if(height<=0){
               height = 200;
            }
            win_rc = QRect(0,0,rect.width() - left.width() - right.width(),height);
            pos.rx() += left.width();
            pos.ry() += (rect.height() - bottom.height()  - height);
        }
        //qDebug()<<"point:"<<pos<<rect<<win_rc;
        this->unActiveAll();
        d->fixed_window->setGeometry(win_rc);
        d->fixed_window->show();
        d->fixed_window->updateResizer();
        d->fixed_window->move(pos);
        d->fixed_window->centerWidget()->activeWidget(true);
    }

    void DockingWorkbench::resizeFixedWindow(const QSize& size)
    {
        if(d->fixed_window!=nullptr/* && d->fixed_window->isHidden()==false*/){
            int position = d->fixed_window->fixedPosition();
            //QPoint pos = this->pos();
            QRect rect = d->fixed_window->geometry();

            //qDebug()<<"resize fixed:"<<rect;
            QSize left = tabBarSize(DockingPaneManager::S_Left);
            QSize top = tabBarSize(DockingPaneManager::S_Top);
            QSize right = tabBarSize(DockingPaneManager::S_Right);
            QSize bottom = tabBarSize(DockingPaneManager::S_Bottom);
            QRect rc;
            if(position==DockingPaneManager::S_Left){
                rc = QRect(left.width(),top.height(),size.height() - top.height() - bottom.height(),rect.width());
                //pos.rx() += left.width();
                //pos.ry() += top.height();
            }else if(position==DockingPaneManager::S_Top){
                rc = QRect(left.width(),0,size.width() - left.width() - right.width() , rect.height());
                //pos.rx() += left.width();
                //pos.ry() += top.height();

            }else if(position==DockingPaneManager::S_Right){
                rc = QRect(size.width() - rect.width() - right.width(),top.height(),rect.width(),size.height() - top.height() - bottom.height());


                //int width = d->fixed_window->geometry().width();
                //pos.rx() += (rect.width() - right.width() - width);
                //pos.ry() += top.height();
            }else if(position==DockingPaneManager::S_Bottom){
                rc = QRect(left.width(),size.height() - rect.height() - bottom.height(),size.width() - left.width() - right.width(),rect.height());


                //int height = d->fixed_window->geometry().height();
                //pos.rx() += left.width();
                //pos.ry() += (top.height() - bottom.height() - height);
            }
            //d->fixed_window->move(pos);
            d->fixed_window->setGeometry(rc);
            //qDebug()<<"fix:"<<rc<<";size:"<<size;
        }
    }

    void DockingWorkbench::hideFixedWindow()
    {
        if(d->fixed_window!=nullptr){
            //int position = d->fixed_window->fixedPosition();
            //save container rect
            //QRect rc = d->fixed_window->centerWidget()->geometry();
            //qDebug()<<"hideFixedWindow"<<rc;
            DockingPaneContainer* container = (DockingPaneContainer*)d->fixed_window->centerWidget();
            if(container!=nullptr){
                container->setOriRect(container->geometry());
            }
            d->fixed_window->hide();
        }
    }

    void DockingWorkbench::unActiveAll(){
        QObjectList lists = children();
        foreach(auto one,lists){
            QString name = one->metaObject()->className();
            if(name=="ady::DockingPaneContainer" || name=="ady::DockingPaneClient"){
                ((DockingPaneContainer*)one)->activeWidget(false);
            }else if(name=="ady::DockingPaneFixedWindow"){
                DockingPaneContainer* c = ((DockingPaneFixedWindow*)one)->centerWidget();
                if(c!=nullptr){
                    c->activeWidget(false);
                }
            }else if(name=="ady::DockingPaneFloatWindow"){
                DockingPaneFloatWindow* w = (DockingPaneFloatWindow*)one;
                if(!w->isHidden())
                    w->centerWidget()->activeWidget(false);
            }
        }
    }

    void DockingWorkbench::updateLayout(){
        this->updateTabBars(this->size());
        this->layout()->update();
    }

    void DockingWorkbench::dump(QString prefix){
        DockingPaneLayout* layout = (DockingPaneLayout*)this->layout();
        if(layout!=nullptr){
            layout->dump(prefix);
        }
    }

    void DockingWorkbench::onTimeout(){
        emit onShow();
        if(d->timer!=nullptr){
            disconnect(d->timer,&QTimer::timeout,this,&DockingWorkbench::onTimeout);
            delete d->timer;
            d->timer = nullptr;
        }
    }

    void DockingWorkbench::resizeEvent(QResizeEvent *event)
    {
        QWidget::resizeEvent(event);
        QSize size = event->size();
        updateTabBars(size);
        resizeFixedWindow(size);

        if(d->timer!=nullptr){
            d->timer->stop();
            d->timer->start(200);
        }
        //qDebug()<<"resizeEvent";
    }

    void DockingWorkbench::paintEvent(QPaintEvent *e)
    {
        //QFrame::paintEvent(e);
        Q_UNUSED(e);
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Frame, &opt, &p, this);
    }

    void DockingWorkbench::mousePressEvent(QMouseEvent *event){
        QWidget::mousePressEvent(event);
        QPointF p = event->localPos();
        this->unActiveAll();
        if(d->fixed_window!=nullptr && d->fixed_window->isHidden()==false){
            QRect rc = d->fixed_window->geometry();
            if(rc.contains(p.x(),p.y())){
                d->fixed_window->centerWidget()->activeWidget(true);
                return ;
            }
        }
        QObjectList lists = children();
        foreach(auto one,lists){
            QString name = one->metaObject()->className();
            //qDebug()<<"name:"<<name;
            if(name=="ady::DockingPaneContainer" || name=="ady::DockingPaneClient"){
                QRect rc = ((QWidget*)one)->geometry();
                if(rc.contains(p.x(),p.y())){
                    if(name=="ady::DockingPaneContainer"){
                        ((DockingPaneContainer*)one)->activeWidget(true);
                    }
                    this->hideFixedWindow();
                }
            }
        }
    }


}
