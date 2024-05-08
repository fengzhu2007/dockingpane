#include "docking_pane_manager.h"
#include "docking_workbench.h"
#include "docking_pane_container.h"
#include "docking_pane_layout.h"
#include "docking_pane.h"
#include "docking_pane_layout_item_info.h"
#include "docking_pane_client.h"

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
        d->layout = new DockingPaneLayout(d->workbench,0,10);

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

    void DockingPaneManager::createPane(DockingPane* pane,Position position)
    {
        //parent containter workbench
        DockingPaneContainer* containter = new DockingPaneContainer(d->workbench);

        if(position==S_Left){
            //d->workbench->insertContainer(0,containter);
        }else if(position==S_Top){

        }else if(position==S_Right){
             //d->workbench->appendContainer(containter);
        }else if(position==S_Bottom){

        }else if(position==Left){

        }else if(position==Top){

        }else if(position==Right){

        }else if(position==Bottom){

        }else if(position==Center){

        }
    }

    void DockingPaneManager::createPane(DockingPane* pane,DockingPaneContainer* container,Position position)
    {

    }

    DockingPane* DockingPaneManager::createPane(QString id,QString group,QString title,QWidget* widget,Position position)
    {
        //default client as target
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
