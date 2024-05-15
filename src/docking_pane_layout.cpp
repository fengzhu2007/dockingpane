#include "docking_pane_layout.h"
#include "docking_pane_layout_item_info.h"
#include "docking_pane_container.h"
#include "docking_workbench.h"
#include <QDebug>
namespace ady {
    DockingPaneLayout::DockingPaneLayout(QWidget *parent, int margin, int spacing)
        :QLayout(parent){
        setMargin(margin);
        setSpacing(spacing);
        m_rootItem = new DockingPaneLayoutItemInfo(nullptr,DockingPaneManager::Top);
        m_rootItem->setObjectName("rootIteminfo");
    }


    DockingPaneLayout::~DockingPaneLayout()
    {

    }

    void DockingPaneLayout::addItem(QLayoutItem *item)
    {
        addItem(item,DockingPaneManager::Right);
    }


    Qt::Orientations DockingPaneLayout::expandingDirections() const
    {
        return Qt::Horizontal | Qt::Vertical;
    }


    bool DockingPaneLayout::hasHeightForWidth() const
    {
        return false;
    }

    int DockingPaneLayout::count() const
    {
        return m_rootItem->size();
    }

    QLayoutItem *DockingPaneLayout::itemAt(int index) const
    {
        Q_FOREACH(DockingPaneLayoutItemInfo* one, m_rootItem->m_children){
            QLayoutItem* item = one->itemAt(index);
            if(item!=nullptr){
                return item;
            }else{
                index -= 1;
            }
        }
        return nullptr;
    }

    QSize DockingPaneLayout::minimumSize() const
    {
        return m_rootItem->calculateSize(DockingPaneLayoutItemInfo::MinimumSize,spacing());
    }

    void DockingPaneLayout::setGeometry(const QRect &rect)
    {
        QLayout::setGeometry(rect);

        DockingWorkbench* workbench = (DockingWorkbench*)parentWidget();
        QSize left = workbench->tabBarSize(DockingPaneManager::S_Left);
        QSize top = workbench->tabBarSize(DockingPaneManager::S_Top);
        QSize right = workbench->tabBarSize(DockingPaneManager::S_Right);
        QSize bottom = workbench->tabBarSize(DockingPaneManager::S_Bottom);
        int width = rect.width() - left.width() - right.width();
        int height = rect.height() - top.height() - bottom.height();
        int x = left.width();
        int y = top.height();
        QRect center_rect(x,y,width,height);
        m_rootItem->setGeometry(center_rect,spacing());
    }

    QSize DockingPaneLayout::sizeHint() const
    {
        return m_rootItem->calculateSize(DockingPaneLayoutItemInfo::SizeHint,spacing());
    }

    QLayoutItem *DockingPaneLayout::takeAt(int index)
    {
        return nullptr;
    }


    void DockingPaneLayout::addPane(DockingPane* pane,DockingPaneManager::Position position)
    {

    }

    void DockingPaneLayout::addPane(DockingPane* pane,QLayoutItem* relation,DockingPaneManager::Position position)
    {

    }

    void DockingPaneLayout::addItem(DockingPaneContainer* widget,DockingPaneManager::Position position)
    {
        addItem(new QWidgetItem((QWidget*)widget),position);
    }

    void DockingPaneLayout::addItem(QLayoutItem* itemInfo,DockingPaneManager::Position position)
    {
        int count = m_rootItem->m_children.size();
        if(count==0){
            m_rootItem->insertItem(parentWidget(),itemInfo,position);
        }else{
            if(m_rootItem->m_children_ori==DockingPaneLayoutItemInfo::Horizontal){
                if(position==DockingPaneManager::Left || position==DockingPaneManager::S_Left){
                    //insert left 1
                    m_rootItem->insertItem(parentWidget(),itemInfo,DockingPaneManager::Left);
                }else if(position==DockingPaneManager::Right || position==DockingPaneManager::S_Right){
                    //append 1
                    m_rootItem->insertItem(parentWidget(),itemInfo,DockingPaneManager::Right);
                }else if(position == DockingPaneManager::Center){


                }else{
                    DockingPaneLayoutItemInfo* child = m_rootItem;
                    if(count==1){
                        child->m_item = child->m_children.at(0)->m_item;
                        //((DockingPaneContainer*)child->m_item->widget())->setItemInfo(child);
                        child->m_children.clear();
                        child->m_children_ori = DockingPaneLayoutItemInfo::Unkown;
                    }
                    m_rootItem = new DockingPaneLayoutItemInfo(nullptr,DockingPaneManager::Top);
                    m_rootItem->setObjectName("rootItemInfo");
                    child->setParent(m_rootItem);
                    child->initHandle(parentWidget());
                    m_rootItem->m_children.append(child);

                    if(position==DockingPaneManager::Top || position==DockingPaneManager::S_Top){
                        m_rootItem->insertItem(parentWidget(),itemInfo,DockingPaneManager::Top);
                    }else{
                        m_rootItem->insertItem(parentWidget(),itemInfo,DockingPaneManager::Bottom);
                    }
                }

            }else{
                if(position==DockingPaneManager::Top || position==DockingPaneManager::S_Top){
                    //insert left 1
                    m_rootItem->insertItem(parentWidget(),itemInfo,DockingPaneManager::Top);
                }else if(position==DockingPaneManager::Bottom || position==DockingPaneManager::S_Bottom){
                    //append 1
                    m_rootItem->insertItem(parentWidget(),itemInfo,DockingPaneManager::Bottom);
                }else if(position == DockingPaneManager::Center){


                }else{
                    DockingPaneLayoutItemInfo* child = m_rootItem;
                    if(count==1){
                        child->m_item = child->m_children.at(0)->m_item;
                        child->m_children.clear();
                        child->m_children_ori = DockingPaneLayoutItemInfo::Unkown;
                    }
                    m_rootItem = new DockingPaneLayoutItemInfo(nullptr,DockingPaneManager::Left);
                    m_rootItem->setObjectName("rootItemInfo");
                    child->setParent(m_rootItem);
                    child->initHandle(parentWidget());
                    m_rootItem->m_children.append(child);

                    if(position==DockingPaneManager::Left || position==DockingPaneManager::S_Left){
                        m_rootItem->insertItem(parentWidget(),itemInfo,DockingPaneManager::Left);
                    }else{
                        m_rootItem->insertItem(parentWidget(),itemInfo,DockingPaneManager::Right);
                    }
                }

            }
        }
    }


    void DockingPaneLayout::addItem(DockingPaneContainer* widget,DockingPaneLayoutItemInfo* relation,DockingPaneManager::Position position)
    {
        DockingPaneLayoutItemInfo* parentItemInfo = relation->parent();
        DockingPaneLayoutItemInfo::Orientation parentItemOrientation = parentItemInfo->childrenOrientation();
        QWidgetItem* itemInfo = new QWidgetItem((QWidget*)widget);
        if((position==DockingPaneManager::Left || position==DockingPaneManager::Right)){

            if((parentItemOrientation==DockingPaneLayoutItemInfo::Unkown || parentItemOrientation==DockingPaneLayoutItemInfo::Horizontal)){

                int row = relation->row();

                if(position==DockingPaneManager::Left){
                    //row -= 1;
                    if(row<0){
                        row = 0;
                    }
                }else if(position==DockingPaneManager::Right){
                    row += 1;
                }
                //qDebug()<<"row:"<<row;
                parentItemInfo->insertItem(parentWidget(),itemInfo,position,row);
            }else{
                relation->insertItem(parentWidget(),itemInfo,position);
            }
        }else if((position==DockingPaneManager::Top || position==DockingPaneManager::Bottom)){
            if((parentItemOrientation==DockingPaneLayoutItemInfo::Unkown || parentItemOrientation==DockingPaneLayoutItemInfo::Vertical)){
                //parentItemInfo->insertItem()

                int row = relation->row();
                if(position==DockingPaneManager::Top){
                    //row -= 1;
                    if(row<0){
                        row = 0;
                    }
                }else if(position==DockingPaneManager::Bottom){
                    row += 1;
                }

                parentItemInfo->insertItem(parentWidget(),itemInfo,position,row);
            }else{
                //relation->insertItem()
                relation->insertItem(parentWidget(),itemInfo,position);
            }
        }
    }

    DockingPaneLayoutItemInfo* DockingPaneLayout::rootItem()
    {
        return m_rootItem;
    }


}
