#include "docking_workbench.h"
#include "docking_pane_client.h"
#include "docking_guide.h"
#include "docking_guide_diamond.h"
#include "docking_guide_cover.h"
#include "docking_pane_layout_item_info.h"
#include "docking_pane_layout.h"
#include "docking_pane_float_window.h"
#include "docking_pane_fixed_window.h"
#include "docking_pane_tabbar.h"
#include "docking_pane.h"
#include <QLayout>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

namespace ady {
    class DockingWorkbenchPrivate {
    public:
        //QSplitter* vertical_splitter;//level 1
        DockingGuide* guide = nullptr;
        DockingGuideDiamond* siders[4];
        bool guide_visibility = false;
        QList<DockingPaneContainer*> children;
        bool lookup = false;
        DockingGuideCover* cover = nullptr;
        DockingPaneTabBar* tabBars[4];
        DockingPaneClient* client = nullptr;
        DockingPaneFixedWindow* fixed_window = nullptr;
    };


    DockingWorkbench::DockingWorkbench(QWidget* parent)
        :QFrame(parent){
        setStyleSheet(".ady--DockingWorkbench{background:#eeeef2}");//theme
        d = new DockingWorkbenchPrivate();

        for(int i=0;i<4;i++){
            d->tabBars[i] = new DockingPaneTabBar(this);
            d->tabBars[i]->setPosition((DockingPaneManager::Position)i);
        }
        //init client container
    }

    void DockingWorkbench::initClient()
    {
        d->client = new DockingPaneClient(this);
        d->client->setObjectName("client_container");
        DockingPaneLayout* layout = (DockingPaneLayout*)this->layout();
        //layout->setMargin(3);
        assert(layout!=nullptr);
        layout->addItem(d->client,DockingPaneManager::Center);
    }


    DockingWorkbench::~DockingWorkbench(){
        delete d;
    }

    void DockingWorkbench::showGuide(QWidget* widget,QRect rect)
    {
        if(d->guide==nullptr){
            for(int i=0;i<4;i++){
                d->siders[i] = new DockingGuideDiamond((DockingPaneManager::Position)i,this);
                d->siders[i]->setWindowFlags(Qt::CoverWindow|Qt::Tool|Qt::FramelessWindowHint);
                d->siders[i]->setMaximumSize(40,40);
            }
            d->guide = new DockingGuide(this);
            d->guide->raise();
        }
        d->guide->show();

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

    }

    void DockingWorkbench::showSiderGuide()
    {
        if(d->guide_visibility==true){
            return ;
        }


        if(d->guide==nullptr){
            return ;
        }
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
        if(d->guide!=nullptr){
            DockingGuideDiamond* diamond = d->guide->updateActive(pos);
            if(diamond!=nullptr){
                return diamond->position();
            }
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

        if(container==nullptr && position>=4){
            position -= 4;
            if(position==4){
                position = 0;
            }
        }

        if(position==DockingPaneManager::S_Left){
            QRect rc = geometry();
            coverRect.setX(workbenchPos.x());
            coverRect.setY(workbenchPos.y());
            coverRect.setWidth(rect.width());
            coverRect.setHeight(geometry().height());

            //qDebug()<<"rootItemOrientation:"<<rootItemOrientation;
           // qDebug()<<"rootItemOrientation size:"<<rootItem->childrenCount();
            if(rootItemOrientation==DockingPaneLayoutItemInfo::Unkown || rootItemOrientation==DockingPaneLayoutItemInfo::Horizontal){

            }else{
                coverRect.setWidth(rc.width() / 2);
            }


        }else if(position==DockingPaneManager::S_Top){
            QRect rc = geometry();
            coverRect.setX(workbenchPos.x());
            coverRect.setY(workbenchPos.y());
            coverRect.setWidth(geometry().width());
            coverRect.setHeight(rect.height());
            if(rootItemOrientation==DockingPaneLayoutItemInfo::Unkown || rootItemOrientation==DockingPaneLayoutItemInfo::Vertical){

            }else{
                coverRect.setHeight(rc.height() / 2);
            }


        }else if(position==DockingPaneManager::S_Right){
            QRect rc = geometry();
            coverRect.setX(workbenchPos.x() + rc.width() - rect.width());
            coverRect.setY(workbenchPos.y());
            coverRect.setWidth(rect.width());
            coverRect.setHeight(rc.height());

            if(rootItemOrientation==DockingPaneLayoutItemInfo::Unkown || rootItemOrientation==DockingPaneLayoutItemInfo::Horizontal){

            }else{
                coverRect.setWidth(rc.width() / 2);
            }

        }else if(position==DockingPaneManager::S_Bottom){
            QRect rc = geometry();
            coverRect.setX(workbenchPos.x());
            coverRect.setY(workbenchPos.y() + rc.height() - rect.height());
            coverRect.setWidth(rc.width());
            coverRect.setHeight(rect.height());

            if(rootItemOrientation==DockingPaneLayoutItemInfo::Unkown || rootItemOrientation==DockingPaneLayoutItemInfo::Vertical){

            }else{
                coverRect.setY(workbenchPos.y() + rc.height() /2);
                coverRect.setHeight(rc.height() / 2);
            }
        }else{
            //DockingPaneLayoutItemInfo* itemInfo = container->itemInfo();
            //DockingPaneLayoutItemInfo* parentItemInfo = itemInfo->parent();
            //DockingPaneLayoutItemInfo::Orientation parentItemOrientation = parentItemInfo->childrenOrientation();

            QPoint containerPos = mapToGlobal(container->pos());
            QRect rc = container->geometry();

            if(position==DockingPaneManager::Left){

                coverRect.setX(containerPos.x());
                coverRect.setY(containerPos.y());
                coverRect.setWidth(rc.width() / 2);
                coverRect.setHeight(rc.height());

                if(rootItemOrientation==DockingPaneLayoutItemInfo::Unkown || rootItemOrientation==DockingPaneLayoutItemInfo::Horizontal){

                }else{

                }
            }else if(position==DockingPaneManager::Top){

                coverRect.setX(containerPos.x());
                coverRect.setY(containerPos.y());
                coverRect.setWidth(rc.width());
                coverRect.setHeight(rc.height() / 2);


                if(rootItemOrientation==DockingPaneLayoutItemInfo::Unkown || rootItemOrientation==DockingPaneLayoutItemInfo::Vertical){

                }else{


                }

            }else if(position==DockingPaneManager::Right){

                coverRect.setX(containerPos.x() + rc.width() /2);
                coverRect.setY(containerPos.y());
                coverRect.setWidth(rc.width() / 2);
                coverRect.setHeight(rc.height());


                if(rootItemOrientation==DockingPaneLayoutItemInfo::Unkown || rootItemOrientation==DockingPaneLayoutItemInfo::Horizontal){

                }else{


                }

            }else if(position==DockingPaneManager::Bottom){

                coverRect.setX(containerPos.x());
                coverRect.setY(containerPos.y() + rc.height() / 2);
                coverRect.setWidth(rc.width() );
                coverRect.setHeight(rc.height() / 2);


                if(rootItemOrientation==DockingPaneLayoutItemInfo::Unkown || rootItemOrientation==DockingPaneLayoutItemInfo::Horizontal){

                }else{


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
            layout->addItem(widget,(DockingPaneManager::Position)position);
        }else if(position==DockingPaneManager::Left || position==DockingPaneManager::Top || position==DockingPaneManager::Right ||position==DockingPaneManager::Bottom){
            DockingPaneLayoutItemInfo* relation = container->itemInfo();
            //qDebug()<<"item info:"<<relation<<";name:"<<relation->objectName();
            layout->addItem(widget,relation,(DockingPaneManager::Position)position);
        }else{
            int size = widget->paneCount();
            for(int i=0;i<size;i++){
                DockingPane* pane = widget->pane(0);
                container->appendPane(pane);
                if(i==0){
                   container->setPane(container->paneCount()-1);
                }
            }

            widget->close();
        }

        layout->update();
        window->hide();
        window->deleteLater();

    }

    void DockingWorkbench::restoreWidget(DockingPaneContainer* widget,int position){
        DockingPaneLayout* layout = (DockingPaneLayout*)this->layout();
        widget->setState(DockingPaneContainer::Inner);

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
        d->fixed_window = nullptr;
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
            DockingPaneTabBar* tabBar = this->tabBar(position);

            //tabBar->removeContainer(widget);
            //d->fixed_window->close();
            //delete d->fixed_window;
            //d->fixed_window = nullptr;
            //widget->setParent(this);
            layout->addItem(widget,(DockingPaneManager::Position)position);
            widget->activeWidget(false);

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
        //qDebug()<<"position:"<<position<<";visibile:"<<d->tabBars[position]->isVisible()<<d->tabBars[position]->size();
        return d->tabBars[position]->isVisible()?d->tabBars[position]->size():QSize(0,0);
    }

    void DockingWorkbench::siderFixed(DockingPaneContainer* container,int position)
    {
        //DockingPaneLayout* layout = (DockingPaneLayout*)this->layout();
        DockingPaneLayoutItemInfo* itemInfo = container->itemInfo();

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
        container->setParent(nullptr);
        DockingPaneTabBar* tabBar = d->tabBars[position];

        QRect r = geometry();
        if(position==DockingPaneManager::S_Top || position==DockingPaneManager::S_Bottom){
            tabBar->setGeometry(0,0,r.width(),30);
        }else if(position==DockingPaneManager::S_Left || position==DockingPaneManager::S_Right){
            tabBar->setGeometry(0,0,30,r.height());
        }

        tabBar->addContainer(container);
        updateTabBars(size());
        this->layout()->update();
    }

    void DockingWorkbench::updateTabBars(const QSize& size)
    {
        QSize left = tabBarSize(DockingPaneManager::S_Left);
        QSize top = tabBarSize(DockingPaneManager::S_Top);
        QSize right = tabBarSize(DockingPaneManager::S_Right);
        QSize bottom = tabBarSize(DockingPaneManager::S_Bottom);
        //qDebug()<<"right:"<<right;

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

    DockingPaneClient* DockingWorkbench::client()
    {
        return d->client;
    }

    void DockingWorkbench::showFixedWindow(DockingPaneContainer* container,int position)
    {
        if(d->fixed_window==nullptr){
            d->fixed_window = new DockingPaneFixedWindow(this,5);
            d->fixed_window->setCenterWidget(container);
            d->fixed_window->setFixedPosition(position);
        }else{
            d->fixed_window->setCenterWidget(container);
        }
        //qDebug()<<"fixed:"<<container;
        d->fixed_window->raise();
        container->setFocus();
        QRect rect = geometry();

        QSize left = tabBarSize(DockingPaneManager::S_Left);
        QSize top = tabBarSize(DockingPaneManager::S_Top);
        QSize right = tabBarSize(DockingPaneManager::S_Right);
        QSize bottom = tabBarSize(DockingPaneManager::S_Bottom);


        QPoint pos = this->pos();
        QRect win_rc;

        if(position==DockingPaneManager::S_Left){
            win_rc = QRect(0,0,200,rect.height() - top.height() - bottom.height());
            pos.rx() += left.width();
            pos.ry() += top.height();
        }else if(position==DockingPaneManager::S_Top){
            win_rc = QRect(0,0,rect.width() - left.width() - right.width() ,200);
            pos.rx() += left.width();
            pos.ry() += top.height();

        }else if(position==DockingPaneManager::S_Right){
            int width = 200;
            win_rc = QRect(0,0,width,rect.height() - top.height() - bottom.height());
            pos.rx() += (rect.width() - right.width() - width);
            pos.ry() += top.height();
        }else if(position==DockingPaneManager::S_Bottom){
            win_rc = QRect(0,0,rect.width() - left.width() - right.width(),200);
            pos.rx() += left.width();
            pos.ry() += (top.height() - bottom.height());
        }
        d->fixed_window->setGeometry(win_rc);
        d->fixed_window->show();
        d->fixed_window->updateResizer();
        d->fixed_window->move(pos);
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
            d->fixed_window->hide();
        }
    }

    void DockingWorkbench::resizeEvent(QResizeEvent *event)
    {
        QWidget::resizeEvent(event);
        QSize size = event->size();
        updateTabBars(size);
        resizeFixedWindow(size);
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
        if(d->fixed_window!=nullptr && d->fixed_window->isHidden()==false){
            QRect rc = d->fixed_window->geometry();
            if(rc.contains(p.x(),p.y())){
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
                    //qDebug()<<"rc:"<<rc<<";pos:"<<p<<";one:"<<one;
                    if(name=="ady::DockingPaneContainer"){
                        ((DockingPaneContainer*)one)->activeWidget(true);
                    }
                    if(d->fixed_window!=nullptr){
                        d->fixed_window->hide();
                    }
                }else{
                    if(name=="ady::DockingPaneContainer"){
                        ((DockingPaneContainer*)one)->activeWidget(false);
                    }
                }
            }
        }
    }


}
