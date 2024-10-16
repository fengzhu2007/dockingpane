#include "docking_pane_manager.h"
#include "docking_workbench.h"
#include "docking_pane_container.h"
#include "docking_pane_layout.h"
#include "docking_pane.h"
#include "docking_pane_layout_item_info.h"
#include "docking_pane_client.h"
#include "docking_pane_float_window.h"
#include "docking_pane_tabbar.h"
#include <QLayoutItem>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonArray>
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
                    if((position == Position::Left || position == Position::Right) && parent->childrenOrientation() == DockingPaneLayoutItemInfo::Vertical){

                        parent = parent->parent();
                    }else if((position == Position::Top || position == Position::Bottom) && parent->childrenOrientation() == DockingPaneLayoutItemInfo::Horizontal){

                        parent = parent->parent();
                    }

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

    DockingPaneLayoutItemInfo* DockingPaneManager::createPane(DockingPane* pane,DockingPaneContainer* target,Position position)
    {
        Q_ASSERT(target!=nullptr);
        if(position==Center){
            target->appendPane(pane);
            return target->itemInfo();
        }else if(position == C_Right || position == C_Bottom){
            //target is client and append next client
            if(target->isClient()){
                auto info = target->itemInfo();
                auto next = info->next();
                if(next!=nullptr){
                    auto cli = next->container();
                    if(cli!=nullptr && cli->isClient()){
                        return this->createPane(pane,cli,Center);
                    }
                }
                DockingPaneClient* cli = new DockingPaneClient(d->workbench,true);
                cli->appendPane(pane);
                cli->setObjectName(pane->id()+"_client_container");
                return d->layout->addItem(cli,info,position);
            }
            position = (Position)(position - 5);//position fixed to Right or Bottom
        }else if(position == C_Left || position == C_Top){
            //target is client and append next client
            if(target->isClient()){
                auto info = target->itemInfo();
                auto previous = info->previous();
                if(previous!=nullptr){
                    auto cli = previous->container();
                    if(cli!=nullptr && cli->isClient()){
                        return this->createPane(pane,cli,Center);
                    }
                }
                DockingPaneClient* cli = new DockingPaneClient(d->workbench,true);
                cli->appendPane(pane);
                cli->setObjectName(pane->id()+"_client_container");
                return d->layout->addItem(cli,info,position);

            }
            position = (Position)(position - 5);//position fixed to Left or Top
        }


        auto itemInfo = target->itemInfo();
        Q_ASSERT(itemInfo!=nullptr);
        DockingPaneContainer* container = new DockingPaneContainer(d->workbench);
        container->setObjectName(pane->id()+"_containter");
        pane->setParent(container);
        container->appendPane(pane);

        if(itemInfo->m_children_ori==DockingPaneLayoutItemInfo::Unkown){
            return itemInfo->insertItem(d->workbench,new QWidgetItem(container),position);
        }else if(itemInfo->m_children_ori == DockingPaneLayoutItemInfo::Horizontal){
            if(position == Left || position == Right){
                return itemInfo->insertItem(d->workbench,new QWidgetItem(container),position);
            }else{
                if(itemInfo->m_parent!=nullptr){
                    return itemInfo->m_parent->insertItem(d->workbench,new QWidgetItem(container),position);
                }
            }
        }else if(itemInfo->m_children_ori == DockingPaneLayoutItemInfo::Vertical){
            if(position == Top || position == Bottom){
                return itemInfo->insertItem(d->workbench,new QWidgetItem(container),position);
            }else{
                if(itemInfo->m_parent!=nullptr){
                    return itemInfo->m_parent->insertItem(d->workbench,new QWidgetItem(container),position);
                }
            }
        }
        return d->layout->m_rootItem->insertItem(d->workbench,new QWidgetItem(container),position);
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

    DockingPaneLayoutItemInfo* DockingPaneManager::createPane(DockingPane* pane,DockingPaneLayoutItemInfo* parent,Position position){
        if(position==Center){
            DockingPaneContainer* container = parent==nullptr?d->workbench->client():parent->container();
            container->appendPane(pane);
            return container->itemInfo();
        }else{
            auto container = new DockingPaneContainer(d->workbench);
            container->setObjectName(pane->id()+"_containter");
            container->appendPane(pane);
            //qDebug()<<"create pan:"<<pane->id()<<parent<<position;
            if(parent!=nullptr){
                return parent->insertItem(d->workbench,new QWidgetItem(container),position);
            }else{
                return d->layout->m_rootItem->insertItem(d->workbench,new QWidgetItem(container),position);
            }
        }
    }

    DockingPaneLayoutItemInfo* DockingPaneManager::restorePane(DockingPane* pane,Position position,DockingPaneLayoutItemInfo* parent,DockingPaneLayoutItemInfo* previous){
        if(position==Center){
            DockingPaneContainer* container = parent==nullptr?d->workbench->client():parent->container();
            container->appendPane(pane);
            return container->itemInfo();
        }else{
            auto container = new DockingPaneContainer(d->workbench);
            container->setObjectName(pane->id()+"_containter");
            container->appendPane(pane);
            if(parent!=nullptr){
                return parent->insertItem(d->workbench,new QWidgetItem(container),position);
            }else{
                return d->layout->m_rootItem->insertItem(d->workbench,new QWidgetItem(container),position);
            }
        }

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

    DockingPaneLayout* DockingPaneManager::layout(){
        return d->layout;
    }

    QJsonObject DockingPaneManager::toJson(){
        //QJsonDocument doc;
        auto list = this->toJsonOne(d->layout->m_rootItem);
        QJsonObject inner = {{"list",list},{"orientation",d->layout->m_rootItem->childrenOrientation()}};
        //floatlist
        QJsonArray floatlist;
        auto floatwindows = DockingPaneFloatWindow::windowsList();
        for(auto one:floatwindows){
            auto containerWidget = one->centerWidget();
            int active = containerWidget->current();
            int paneCount = containerWidget->paneCount();
            QJsonArray tabs;
            for(int i=0;i<paneCount;i++){
                auto pane = containerWidget->pane(i);
                auto jObject = pane->toJson();
                tabs<<jObject;
            }
            const QRect rc = one->geometry();
            QJsonObject container = {
                {"left",rc.left()},
                {"top",rc.top()},
                {"width",rc.width()},
                {"height",rc.height()},
                {"active",active},
                {"tabs",tabs}
            };
            floatlist<<container;
        }

        //fixed
        //DockingPaneTabBar* tabBar(int position);
        QJsonArray fixedlist;
        for(int i=0;i<4;i++){
            auto bar = d->workbench->tabBar(i);
            auto list = bar->containerList();

            for(auto one:list){
                int paneCount = one->paneCount();
                QJsonArray tabs;
                for(int i=0;i<paneCount;i++){
                    auto pane = one->pane(i);
                    auto jObject = pane->toJson();
                    tabs<<jObject;
                }
                QJsonObject container = {
                    {"stretch",one->stretch()},
                    {"position",i},
                    {"tabs",tabs}
                };
                fixedlist<<container;
            }
        }


        return {
            {"inner",inner},
            {"float",floatlist},
            {"fixed",fixedlist}
        };
    }

    QJsonArray DockingPaneManager::toJsonOne(DockingPaneLayoutItemInfo* layouItem){
        QJsonArray list;
        int client = 0;
        int count = layouItem->childrenCount();
        for(int i=0;i<count;i++){
            auto ci = layouItem->child(i);
            if(ci->item()==nullptr){
                //has children
                auto children = this->toJsonOne(ci);
                float stretch = ci->stretch();
                if(children.size()>1){
                    QJsonObject paneGroup = {
                        {"stretch",stretch},
                        {"children",children}
                    };
                    list<<paneGroup;
                }else if(children.size()==1){
                    list<<children.at(0);
                }
            }else{
                QJsonArray tabs;
                auto container = ci->container();
                //qDebug()<<"contaner:"<<container<<i;
                float stretch = ci->stretch();
                //qDebug()<<"stretch:"<<stretch;
                int active = container->current();
                //float stretch = container->pane(active)->stretch();
                //qDebug()<<"stretch:"<<stretch;
                if(container->isClient()){
                    client += 1;
                }else{
                    client = 0;
                }
                int paneCount = container->paneCount();
                for(int j=0;j<paneCount;j++){
                    auto pane = container->pane(j);
                    auto jObject = pane->toJson();
                    tabs<<jObject;
                }
                QJsonObject pane = {
                    {"client",client},
                    {"stretch",stretch},
                    {"active",active},
                    {"tabs",tabs}
                };
                list<<pane;
            }
        }
        return list;
    }

    void DockingPaneManager::dump()
    {
        d->layout->m_rootItem->dump("");

        //sider tabBar
        //d->workbench->tabBarSize();

    }

}
