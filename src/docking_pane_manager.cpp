#include "docking_pane_manager.h"
#include "docking_workbench.h"
#include "docking_pane_container.h"
#include "docking_pane_layout.h"
#include "docking_pane.h"
#include "docking_pane_layout_item_info.h"
#include "docking_pane_client.h"
#include "docking_pane_float_window.h"
#include <QLayoutItem>
#include <QHBoxLayout>
#include <QDebug>
namespace ady {

    class DockingPaneManagerPrivate {
    public:
       DockingWorkbench*  workbench;
       DockingPaneLayout* layout;
       QWidget* widget;
    };


    DockingPaneManager::DockingPaneManager(QWidget* parent)
        :QObject(parent){
        d = new DockingPaneManagerPrivate;
        d->widget = new QWidget(parent);


        d->workbench = new DockingWorkbench(d->widget);
        d->layout = new DockingPaneLayout(d->workbench,0,6);

        d->workbench->initClient();

        QHBoxLayout* l = new QHBoxLayout(d->widget);
        l->setMargin(0);
        l->setSpacing(0);
        l->addWidget(d->workbench);
        d->widget->setLayout(l);
    }

    DockingPaneManager::~DockingPaneManager(){
        delete d;
    }

    DockingWorkbench* DockingPaneManager::workbench()
    {
        return d->workbench;
    }

    QWidget* DockingPaneManager::widget()
    {
        return d->widget;
    }

    DockingPaneLayoutItemInfo* DockingPaneManager::createPane(DockingPane* pane,Position position,bool active)
    {
        if(position==Center){
            DockingPaneClient* client = d->workbench->client();
            if(client!=nullptr){
                pane->setParent(client);
                client->initView();
                client->appendPane(pane,active);
                return client->itemInfo();
            }
        }
        auto root = d->layout->rootItem();
        auto orientation = root->childrenOrientation();
        DockingPaneContainer* container = nullptr;
        DockingPaneLayoutItemInfo* relation = nullptr;
        DockingPaneLayoutItemInfo* clientRelation = nullptr;
        if(position==Position::S_Left){
            if(orientation==DockingPaneLayoutItemInfo::Horizontal){
                relation = root->first();
                if(relation->isClient()==false){
                    container = relation->container();
                }
            }
        }else if(position==Position::S_Right){
            if(orientation==DockingPaneLayoutItemInfo::Horizontal){
                relation = root->last();
                if(relation->isClient()==false){
                    container = relation->container();
                }
            }
        }else if(position==Position::S_Top){
            if(orientation==DockingPaneLayoutItemInfo::Vertical){
                relation = root->first();
                if(relation->isClient()==false){
                    container = relation->container();
                }
            }
        }else if(position==Position::S_Bottom){
            if(orientation==DockingPaneLayoutItemInfo::Vertical){
                relation = root->last();
                if(relation->isClient()==false){
                    container = relation->container();
                }
            }
        }else if(position==Position::Left || position==Position::Right || position==Position::Top || position==Position::Bottom){
            //get client
            int clientCount = d->workbench->clientCount();
            auto client = d->workbench->client();
            if(client!=nullptr){
                auto info = client->itemInfo();
                DockingPaneLayoutItemInfo* parent = info->parent();
                if(clientCount>1){
                    parent = parent->parent();
                }
                auto orientation = parent->childrenOrientation();
                if(orientation==DockingPaneLayoutItemInfo::Horizontal){
                    if(position==Position::Left){
                        relation = parent->first();
                        if(relation->isClient()==false){
                            container = relation->container();
                        }
                    }else if(position==Position::Right){
                        relation = parent->last();
                        if(relation->isClient()==false){
                            container = relation->container();
                        }
                    }else{
                        clientRelation = clientCount>1?info->parent():info;
                    }
                }else if(orientation==DockingPaneLayoutItemInfo::Vertical){
                    if(position==Position::Top){
                        relation = parent->first();
                        if(relation->isClient()==false){
                            container = relation->container();
                        }
                    }else if(position==Position::Bottom){
                        relation = parent->last();
                        if(relation->isClient()==false){
                            container = relation->container();
                        }
                    }else{
                        clientRelation = clientCount>1?info->parent():info;
                    }
                }
            }else{
                return this->createPane(pane,static_cast<Position>(position - 4),active);
            }
        }

        /*if(orientation==DockingPaneLayoutItemInfo::Horizontal && (position==Position::Left || position==Position::S_Left)){
            relation = root->first();
            if(relation->isClient()==false){
                container = relation->container();
            }
        }else if(orientation==DockingPaneLayoutItemInfo::Horizontal && (position==Position::Right || position==Position::S_Right)){
            relation = root->last();
            if(relation->isClient()==false){
                container = relation->container();
            }
        }else if(orientation==DockingPaneLayoutItemInfo::Vertical && (position==Position::Top || position==Position::S_Top)){
            relation = root->first();
            if(relation->isClient()==false){
                container = relation->container();
            }
        }else if(orientation==DockingPaneLayoutItemInfo::Vertical && (position==Position::Bottom || position==Position::S_Bottom)){
            relation = root->last();
            if(relation->isClient()==false){
                container = relation->container();
            }
        }*/
        if(container==nullptr){
            container = new DockingPaneContainer(d->workbench);
            pane->setParent(container);
            container->setObjectName(pane->id()+"_containter");
            container->appendPane(pane,active);
            if(clientRelation!=nullptr){
                return d->layout->addItem(container,clientRelation,position);
            }else{
                return d->layout->addItem(container,position);
            }

        }else{
            pane->setParent(container);
            container->appendPane(pane,active);
            //qDebug()<<"append pane :"<<active;
            return relation;
        }

    }

    void DockingPaneManager::createPane(DockingPane* pane,DockingPaneContainer* container,Position position)
    {
        //container->appendPane(pane);
    }

    DockingPane* DockingPaneManager::createPane(QString id,QString group,QString title,QWidget* widget,Position position)
    {
        //default client as target
        if(position==Center){
            //createPane(id,group,title,widget)
            DockingPaneClient* client = d->workbench->client();
            if(client!=nullptr){
                client->initView();
                DockingPane* pane = new DockingPane(nullptr);
                pane->setCenterWidget(widget);
                pane->setId(id);
                pane->setGroup(group);
                pane->setWindowTitle(title);
                client->appendPane(pane);
                return pane;
            }
        }
        DockingPaneContainer* container = new DockingPaneContainer(d->workbench);
        container->setObjectName(id+"_containter");
        DockingPane* pane = new DockingPane(container);
        pane->setCenterWidget(widget);
        pane->setId(id);
        pane->setGroup(group);
        pane->setWindowTitle(title);
        container->appendPane(pane);
        //container->setItemInfo(d->layout->m_rootItem);
        d->layout->addItem(container,position);
        return pane;
    }

    DockingPane* DockingPaneManager::createPane(QString id,QString group,QString title,QWidget* widget,DockingPaneContainer* target,Position position)
    {

        if(target==nullptr){
            return createPane(id,group,title,widget,position);
        }
        DockingPaneLayoutItemInfo* itemInfo = d->layout->m_rootItem->findItemInfo(target);
        if(itemInfo==nullptr){
             return createPane(id,group,title,widget,position);
        }
        if(position==Center){
            DockingPane* pane = new DockingPane(target);
            pane->setCenterWidget(widget);
            target->appendPane(pane);
            return pane;
        }
        DockingPaneContainer* container = new DockingPaneContainer(d->workbench);
        container->setObjectName(id+"_containter");
        DockingPane* pane = new DockingPane(container);
        pane->setCenterWidget(widget);
        pane->setId(id);
        pane->setGroup(group);
        pane->setWindowTitle(title);
        container->appendPane(pane);

        if(itemInfo->m_children_ori==DockingPaneLayoutItemInfo::Unkown){
            //container->setItemInfo(itemInfo);
            itemInfo->insertItem(d->workbench,new QWidgetItem(container),position);
            return pane;
        }else if(itemInfo->m_children_ori == DockingPaneLayoutItemInfo::Horizontal){
            if(position == Left || position == Right){
                //container->setItemInfo(itemInfo);
                itemInfo->insertItem(d->workbench,new QWidgetItem(container),position);
                return pane;
            }else{
                if(itemInfo->m_parent!=nullptr){
                    //container->setItemInfo(itemInfo->m_parent);
                    itemInfo->m_parent->insertItem(d->workbench,new QWidgetItem(container),position);
                    return pane;
                }
            }
        }else if(itemInfo->m_children_ori == DockingPaneLayoutItemInfo::Vertical){
            if(position == Top || position == Bottom){
                //container->setItemInfo(itemInfo);
                itemInfo->insertItem(d->workbench,new QWidgetItem(container),position);
                return pane;
            }else{
                if(itemInfo->m_parent!=nullptr){
                    //container->setItemInfo(itemInfo->m_parent);
                    itemInfo->m_parent->insertItem(d->workbench,new QWidgetItem(container),position);
                    return pane;
                }
            }
        }
        //container->setItemInfo(d->layout->m_rootItem);
        d->layout->m_rootItem->insertItem(d->workbench,new QWidgetItem(container),position);
        return pane;
    }

    DockingPane* DockingPaneManager::createFixedPane(const QString& id,const QString& group,const QString& title,QWidget* widget,Position position){
        DockingPaneContainer* container = new DockingPaneContainer(d->workbench,position);
        DockingPane* pane = new DockingPane(container);
        pane->setCenterWidget(widget);
        pane->setId(id);
        pane->setGroup(group);
        pane->setWindowTitle(title);
        container->appendPane(pane);
        d->workbench->siderFixed(container,position);
        //d->workbench->layout()->update();
        return pane;
    }

    DockingPaneFloatWindow* DockingPaneManager::createFloatPane(const QString& id,const QString& group,const QString& title,QWidget* widget){
        int margin = 6;
        //qDebug()<<"widget:"<<widget->geometry();

        DockingPaneFloatWindow* window = new DockingPaneFloatWindow(d->workbench,margin);
        DockingPaneContainer* container = new DockingPaneContainer(d->workbench,Position::Left);
        DockingPane* pane = new DockingPane(container);
        pane->setCenterWidget(widget);
        pane->setId(id);
        pane->setGroup(group);
        pane->setWindowTitle(title);
        container->appendPane(pane);
        //qDebug()<<"container:"<<container->geometry();
        window->setCenterWidget(container);
        window->updateResizer();
        QRect rc = d->workbench->geometry();
        //qDebug()<<"workbench:"<<rc;
        //QPoint pos =
        int width = rc.width() / 3;
        int height = rc.height() / 3;

        QPoint pos = d->workbench->mapToGlobal(QPoint(50,50));
        window->setGeometry(QRect(pos.x(),pos.y(),width,height));
        window->show();
        return window;
    }

    /*void DockingPaneManager::addItem(QWidget* widget,Position position)
    {
        d->layout->addItem(widget,position);
    }*/

    void DockingPaneManager::dump()
    {
        d->layout->m_rootItem->dump("");

        //sider tabBar
        //d->workbench->tabBarSize();

    }

}
