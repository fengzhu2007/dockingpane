#include "docking_pane_tabbar.h"
#include "docking_pane_container.h"
#include "docking_pane.h"
#include "docking_workbench.h"
#include "docking_pane_tabbar_item.h"
#include <QBoxLayout>
#include <QSpacerItem>
#include <QDebug>
namespace ady{
    class DockingPaneTabBarPrivate {
    public:
        DockingPaneManager::Position position;
        QList<DockingPaneContainer*> list;
        QList<DockingPaneTabBarItem*> children;
        DockingPaneTabBar::Shape shape;
        QBoxLayout* layout;
        QSpacerItem* spacer;
    };

    DockingPaneTabBar::DockingPaneTabBar(QWidget* parent)
        :QWidget(parent){
        d = new DockingPaneTabBarPrivate;
        d->layout = new QBoxLayout(QBoxLayout::LeftToRight,this);
        d->spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
        d->layout->setMargin(0);
        //d->layout->setContentsMargins(5,5,5,5);
        d->layout->setSpacing(10);
        d->layout->addItem(d->spacer);
        setVisible(false);

        connect(this,SIGNAL(currentChanged(int)),this,SLOT(onCurrentChanged(int)));
    }

    DockingPaneTabBar::~DockingPaneTabBar(){
        delete d;

    }

    DockingPaneManager::Position DockingPaneTabBar::position()
    {
        return d->position;
    }

    void DockingPaneTabBar::setPosition(DockingPaneManager::Position position)
    {
        d->position = position;
        if(d->position==DockingPaneManager::S_Left){
            setShape(DockingPaneTabBar::RoundedWest);
            d->layout->setContentsMargins(0,3,0,0);
        }else if(d->position==DockingPaneManager::S_Top){
            setShape(DockingPaneTabBar::RoundedNorth);
            d->layout->setContentsMargins(3,0,0,0);
        }else if(d->position==DockingPaneManager::S_Right){
            setShape(DockingPaneTabBar::RoundedEast);
            d->layout->setContentsMargins(0,3,0,0);
        }else if(d->position==DockingPaneManager::S_Bottom){
            setShape(DockingPaneTabBar::RoundedSouth);
            d->layout->setContentsMargins(3,0,0,0);
        }
    }

    void DockingPaneTabBar::addContainer(DockingPaneContainer* container)
    {
        container->setState(DockingPaneContainer::Fixed);
        d->list.append(container);
        int children = container->paneCount();
        for(int i=0;i<children;i++){
            DockingPane* pane = container->pane(i);
            QString title = pane->windowTitle();
            addTab(title);
        }
        setVisible(true);
        show();
    }

    void DockingPaneTabBar::removeContainer(DockingPaneContainer* container)
    {
        QList<DockingPaneContainer*>::iterator iter = d->list.begin();
        int i = 0;
        while(iter!=d->list.end()){
            if((*iter)==container){
                //remove tabs
                int children = container->paneCount();
                for(int j=0;j<children;j++){
                    qDebug()<<"removeTab:"<<i;
                    removeTab(i);
                }
                d->list.erase(iter);
                break;
            }
            i += (*iter)->paneCount();
            iter++;
        }
        if(d->children.size()==0){
            setVisible(false);
        }
    }

    void DockingPaneTabBar::removeContainerChild(DockingPaneContainer* container,int index){
        QList<DockingPaneContainer*>::iterator iter = d->list.begin();
        int i = 0;
        while(iter!=d->list.end()){
            if((*iter)==container){
                //remove tabs
                int children = container->paneCount();
                for(int j=0;j<children;j++){
                    if(j==index){
                        removeTab(i);
                        if(children<=1){
                            d->list.erase(iter);
                        }
                        break;
                    }
                }
                break;
            }
            i += (*iter)->paneCount();
            iter++;
        }
        if(d->children.size()==0){
            setVisible(false);
        }
    }

    void DockingPaneTabBar::setShape(Shape shape)
    {
        d->shape = shape;
        //qDebug()<<"shape:"<<d->shape;
        if(d->shape==RoundedNorth){
            d->layout->setDirection(QBoxLayout::LeftToRight);
            d->spacer->changeSize(10,10,QSizePolicy::Expanding,QSizePolicy::Minimum);
            foreach(DockingPaneTabBarItem* one,d->children){
                one->setOrientation(Qt::Horizontal);
            }
        }else if(d->shape==RoundedSouth){
            d->layout->setDirection(QBoxLayout::LeftToRight);
            d->spacer->changeSize(10,10,QSizePolicy::Expanding,QSizePolicy::Expanding);
            foreach(DockingPaneTabBarItem* one,d->children){
                one->setOrientation(Qt::Horizontal);
            }
        }else if(d->shape==RoundedWest){
            d->layout->setDirection(QBoxLayout::TopToBottom);
            d->spacer->changeSize(10,10,QSizePolicy::Expanding,QSizePolicy::Expanding);
            foreach(DockingPaneTabBarItem* one,d->children){
                one->setOrientation(Qt::Vertical);
            }
        }else if(d->shape==RoundedEast){

            d->layout->setDirection(QBoxLayout::TopToBottom);
            d->spacer->changeSize(10,10,QSizePolicy::Expanding,QSizePolicy::Expanding);
            foreach(DockingPaneTabBarItem* one,d->children){
                one->setOrientation(Qt::Vertical);

            }
        }
    }

    void DockingPaneTabBar::addTab(const QString& title)
    {
        /*DockingPaneTabBarItem* item = new DockingPaneTabBarItem(this);
        if(d->shape==RoundedNorth){
            item->setOrientation(Qt::Horizontal);
        }else if(d->shape==RoundedSouth){
            item->setOrientation(Qt::Horizontal);
            item->setSwap(true);
        }else if(d->shape==RoundedWest){
            item->setOrientation(Qt::Vertical);
        }else if(d->shape==RoundedEast){
            item->setOrientation(Qt::Vertical);
            item->setSwap(true);
        }
        item->setText(title);
        d->children.append(item);
        int size = d->children.size();
        d->layout->insertWidget(size - 1,item);
        setVisible(true);
        connect(item,&QPushButton::clicked,this,&DockingPaneTabBar::onItemClicked);*/
        addTab(-1,title);
    }

    void DockingPaneTabBar::addTab(int index,const QString& title){
        DockingPaneTabBarItem* item = new DockingPaneTabBarItem(this);
        if(d->shape==RoundedNorth){
            item->setOrientation(Qt::Horizontal);
        }else if(d->shape==RoundedSouth){
            item->setOrientation(Qt::Horizontal);
            item->setSwap(true);
        }else if(d->shape==RoundedWest){
            item->setOrientation(Qt::Vertical);
        }else if(d->shape==RoundedEast){
            item->setOrientation(Qt::Vertical);
            item->setSwap(true);
        }
        item->setText(title);
        if(index>=0 && index<d->children.size()){
            d->children.insert(index,item);
        }else{
            d->children.append(item);
        }

        int size = d->children.size();
        d->layout->insertWidget(size - 1,item);
        setVisible(true);
        connect(item,&QPushButton::clicked,this,&DockingPaneTabBar::onItemClicked);
    }

    void DockingPaneTabBar::removeTab(int i)
    {
        DockingPaneTabBarItem* child = d->children.takeAt(i);
        child->close();
        if(d->children.size()==0){
            setVisible(false);
        }
    }

    int DockingPaneTabBar::search(DockingPaneContainer* container){
        int i=0;
        foreach(auto one,d->list){
            if(one==container){
                return i;
            }
            i+=1;
        }
        return -1;
    }

    void DockingPaneTabBar::insertContainer(int index,DockingPaneContainer* container){
        int tabIndex = 0;
        for(int i=0;i<index;i++){
            tabIndex += d->list[i]->paneCount();
        }
        d->list.insert(index,container);
        //add tabs
        int children = container->paneCount();
        for(int i=0;i<children;i++){
            DockingPane* pane = container->pane(i);
            QString title = pane->windowTitle();
            addTab(tabIndex+i,title);
        }
        setVisible(true);
        show();
    }

    void DockingPaneTabBar::onCurrentChanged(int i)
    {
        //qDebug()<<"onCurrentChanged:"<<i<<";list:"<<d->list;
        foreach(DockingPaneContainer* one , d->list){
            int children = one->paneCount();
            if(i<children){
                //show fixed window and active tab i
                one->onCurrentChanged(i);
                //hide tabbar items
                one->visibleTabBar(false);
                DockingWorkbench* workbench = (DockingWorkbench*)parentWidget();
                //qDebug()<<"onCurrentChanged"<<i<<d->position;
                workbench->showFixedWindow(one,d->position);
                break ;
            }else{
                i -= children;
            }
        }
    }

    void DockingPaneTabBar::onItemClicked()
    {
        QObject* sender = this->sender();
        //qDebug()<<"sender:"<<sender;
        int i = 0;
        foreach(DockingPaneTabBarItem* one,d->children){
            if(one==sender){
                //qDebug()<<"sender:"<<sender<<"index:"<<i;
                emit currentChanged(i);
                return;
            }
            i++;
        }
    }

    /*QSize DockingPaneTabBar::sizeHint()
    {
        if(d->children.size()>0){
            foreach(DockingPaneTabBarItem* one,d->children){
                qDebug()<<"child:"<<one->sizeHint();
            }
            return QSize(0,0);
        }else{
            return QSize(0,0);
        }
    }*/
}
