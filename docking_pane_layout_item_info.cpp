#include "docking_pane_layout_item_info.h"
#include "docking_pane_handle.h"
#include "docking_pane_container.h"
#include <QDebug>
namespace ady {
    DockingPaneLayoutItemInfo::DockingPaneLayoutItemInfo(QLayoutItem* item,DockingPaneManager::Position position,DockingPaneLayoutItemInfo* parent)
        :QObject()
    {
        m_item = item;
        m_position = position;
        m_parent = parent;
        m_handle = nullptr;
        m_stretch = None;
    }

    DockingPaneLayoutItemInfo::~DockingPaneLayoutItemInfo(){
        //qDeleteAll(m_children);
        if(m_handle!=nullptr){
            delete m_handle;
        }
    }


    void DockingPaneLayoutItemInfo::setChildrenOrientation(Orientation orientation)
    {
        m_children_ori = orientation;
    }

    int DockingPaneLayoutItemInfo::insertItem(QWidget* workbench,QLayoutItem* item, DockingPaneManager::Position position)
    {
        int size = m_children.size();
        if(size==0 && m_item!=nullptr){
            //DockingPaneManager::Position pos =DockingPaneManager::Position::TOP;
            DockingPaneLayoutItemInfo* child = new DockingPaneLayoutItemInfo(m_item,position,this);
            ((DockingPaneContainer*)m_item->widget())->setItemInfo(child);
            child->setObjectName(m_item->widget()->objectName()+"_itemInfo");
            child->initHandle(workbench);
            m_children.push_back(child);
        }
        DockingPaneLayoutItemInfo* child = new DockingPaneLayoutItemInfo(item,position,this);
         ((DockingPaneContainer*)item->widget())->setItemInfo(child);
        child->setObjectName(item->widget()->objectName()+"_itemInfo");
        child->initHandle(workbench);
        if(position==DockingPaneManager::Top || position==DockingPaneManager::Left){
            m_children.insert(0,child);
        }else{
            m_children.push_back(child);
        }
        if(m_children_ori==Unkown){
            if(position==DockingPaneManager::Top || position==DockingPaneManager::Bottom){
                m_children_ori = Vertical;
            }else{
                m_children_ori = Horizontal;
            }
        }
        if(m_children.size()>0){
            QString objectName = "itemInfoGroup:";
            foreach(auto one,m_children){
                objectName += one->objectName();
            }
            setObjectName(objectName);
        }
        m_item = nullptr;
        return 1;
    }

    int DockingPaneLayoutItemInfo::insertItem(QWidget* workbench,QLayoutItem* item,DockingPaneManager::Position position,int index)
    {
        int size = m_children.size();
        if(size==0 && m_item!=nullptr){
            //DockingPaneManager::Position pos =DockingPaneManager::Position::TOP;
            DockingPaneLayoutItemInfo* child = new DockingPaneLayoutItemInfo(m_item,position,this);
            ((DockingPaneContainer*)m_item->widget())->setItemInfo(child);
            child->setObjectName(m_item->widget()->objectName()+"_itemInfo");
            child->initHandle(workbench);
            m_children.push_back(child);
        }
        DockingPaneLayoutItemInfo* child = new DockingPaneLayoutItemInfo(item,position,this);
         ((DockingPaneContainer*)item->widget())->setItemInfo(child);
        child->setObjectName(item->widget()->objectName()+"_itemInfo");
        child->initHandle(workbench);
        m_children.insert(index,child);
        //qDebug()<<"children:"<<m_children;
        /*if(position==DockingPaneManager::Top || position==DockingPaneManager::Left){
            m_children.insert(0,child);
        }else{
            m_children.push_back(child);
        }*/
        if(m_children_ori==Unkown){
            if(position==DockingPaneManager::Top || position==DockingPaneManager::Bottom){
                m_children_ori = Vertical;
            }else{
                m_children_ori = Horizontal;
            }
        }
        if(m_children.size()>0){
            QString objectName = "itemInfoGroup:";
            foreach(auto one,m_children){
                objectName += one->objectName();
            }
            setObjectName(objectName);
        }
        m_item = nullptr;
        return 1;
    }

    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::removeItem(DockingPaneLayoutItemInfo* itemInfo)
    {
        QList<DockingPaneLayoutItemInfo*>::iterator iter = m_children.begin();
        while(iter!=m_children.end()){
            if(itemInfo==(*iter)){
                //((DockingPaneContainer*)itemInfo->m_item->widget())->setItemInfo(nullptr);
                //qDebug()<<"removeItemremoveItemremoveItemremoveItem";
                delete (*iter);
                m_children.erase(iter);
                break;
            }
            iter++;
        }
        if(m_children.size()==1 && m_parent!=nullptr){
            iter = m_children.begin();
            m_item = (*iter)->item();
            if(m_item!=nullptr){
                delete (*iter);
                m_children.clear();
                m_children_ori = Unkown;
                DockingPaneContainer* container = (DockingPaneContainer*)m_item->widget();
                container->setItemInfo(this);
            }else{
                //set all children`s parent as this parent,and delete this later
                int row = this->row();
                int i = 1;
                DockingPaneLayoutItemInfo* parent = this->parent();
                foreach(auto child,(*iter)->m_children){
                    child->setParent(parent);
                    parent->m_children.insert(row+i,child);
                    i+=1;
                }
                //remove this from this.parent
                parent->m_children.removeAt(row);
                m_children.clear();
                delete (*iter);
                //delete this;//todo
                return this;
            }
        }
        return nullptr;
    }

    QLayoutItem* DockingPaneLayoutItemInfo::itemAt(int &index)
    {
        if(m_children.size()>0){
            Q_FOREACH(DockingPaneLayoutItemInfo* one,m_children){
                QLayoutItem* item = one->itemAt(index);
                if(item!=nullptr){
                    return item;
                }else{
                    index -= 1;
                }
            }
        }else{
            if(index==0){
                return m_item;
            }
        }
        return nullptr;

    }

    int DockingPaneLayoutItemInfo::size()
    {
        int size = 0;
        Q_FOREACH(DockingPaneLayoutItemInfo* one,m_children){
            size += one->size();
        }
        if(m_item!=nullptr){
            size += 1;
        }
        return size;
    }

    QSize DockingPaneLayoutItemInfo::calculateSize(SizeType sizeType,int spacing) const
    {
        QSize totalSize;
        int count = m_children.size();
        if(count>0){
            Q_FOREACH(DockingPaneLayoutItemInfo* one,m_children){
                QSize size = one->calculateSize(sizeType,spacing);
                if(m_children_ori==Horizontal){
                    totalSize.setHeight(size.height());
                    totalSize.rwidth() += size.width();
                    //totalSize.setWidth(totalSize.width() + size.width());
                }else{
                    //totalSize.setHeight(totalSize.height() + size.height());
                    totalSize.rheight() += size.height();
                    totalSize.setWidth(size.width());
                }
            }
            if(count>1){
                if(m_children_ori==Horizontal){
                    totalSize.rwidth() += ((count - 1) * spacing);
                }else{
                    totalSize.rheight() += ((count - 1) * spacing);
                }
            }

        }else{
            if(m_item!=nullptr){
                if(sizeType==MinimumSize){
                    totalSize = m_item->minimumSize();
                }else{
                    totalSize = m_item->sizeHint();
                }
            }

        }
        return totalSize;
    }

    void DockingPaneLayoutItemInfo::setGeometry(const QRect &rect,int spacing)
    {
        m_spacing = spacing;
        m_rect = rect;

        int count = m_children.size();
        if(count>0){
            int x = rect.x();
            int y = rect.y();
            int w = 0;
            int h = 0;
            if(count==1){
                w = rect.width();
                h = rect.height();
            }else{
                float stretch = 0.0f;
                int stretch_count = 0;
                Q_FOREACH(DockingPaneLayoutItemInfo* one,m_children){
                    if(one->m_stretch > 0){
                        stretch += one->m_stretch;
                        stretch_count += 1;
                    }
                }

                if(m_children_ori==Horizontal){
                    int width = (int)(rect.width() * (1-stretch));
                    if(count - stretch_count<=0){
                        w = 0;
                    }else{
                        w = (width - (count - 1) * spacing) / (count - stretch_count);
                    }
                    h = rect.height();
                }else{
                    w = rect.width();
                    int height = (int)(rect.height() * (1-stretch));
                    if(count - stretch_count<=0){
                        h = 0;
                    }else{
                        h = (height - (count - 1) * spacing) / (count - stretch_count);
                    }
                }
            }


            int offset = 0;
            int i = 0;
            Q_FOREACH(DockingPaneLayoutItemInfo* one,m_children){
                int cc = one->m_children.size();
                //qDebug()<<"name:"<<one->objectName();
                /*if(cc==0){
                    qDebug()<<"name:"<<one->item()->widget()->objectName();
                    qDebug()<<"-------------------start-------------------------";

                }*/

                QRect child_rc (x,y,w,h);
                /*if(cc==0){
                    qDebug()<<"child_rc:"<<child_rc;
                }*/

                if(one->m_stretch>0){
                    if(m_children_ori==Horizontal){
                        child_rc.setWidth(one->m_stretch * rect.width());
                    }else{
                        child_rc.setHeight(one->m_stretch * rect.height());
                    }
                }

                if(i==count - 1){
                    if(m_children_ori==Horizontal){
                        child_rc.setX(x);
                        child_rc.setWidth(rect.width() - offset);
                    }else{
                        child_rc.setY(y);
                        child_rc.setHeight(rect.height() - offset);
                    }
                }
                one->setGeometry(child_rc,spacing);
                /*if(cc==0){
                    qDebug()<<"child_rc:"<<child_rc;
                }*/
                QRect rc = one->geometry(spacing);
                /*if(cc==0){
                    qDebug()<<"rc:"<<rc;
                    qDebug()<<"-------------------end-------------------------";
                }*/

                if(m_children_ori==Horizontal){
                    x += rc.width();
                    x += spacing;
                    if(i<count - 1){
                        one->m_handle->setGeometry(QRect(rc.x() + rc.width(),rc.y(),spacing,rc.height()));
                        one->m_handle->setOrientation(DockingPaneHandle::Horizontal);
                        one->m_handle->show();
                    }else{
                        one->m_handle->hide();
                    }
                    offset += (rc.width() + spacing);
                }else{
                    y += rc.height();
                    y += spacing;
                    if(i<count - 1){
                        one->m_handle->setGeometry(QRect(rc.x() ,rc.y()+rc.height(),rc.width(),spacing));
                        one->m_handle->setOrientation(DockingPaneHandle::Vertical);
                        one->m_handle->show();
                    }else{
                        one->m_handle->hide();
                    }
                    offset += (rc.height() + spacing);
                }
                i++;
            }
        }else{
            if(m_item!=nullptr){
                //qDebug()<<"name:"<<m_item->widget()->objectName();
                m_item->widget()->show();
                m_item->setGeometry(rect);
            }
        }
    }

    QRect DockingPaneLayoutItemInfo::geometry(int spacing)
    {
        m_spacing = spacing;
        QRect rc;
        int count = m_children.size();
        if(count>0){
            int w = 0;
            int h = 0;
            int x = -1;
            int y = -1;

            Q_FOREACH(DockingPaneLayoutItemInfo* one,m_children){
                QRect rect = one->geometry(spacing);
                if(x==-1){
                    x = rect.x();
                }
                if(y==-1){
                    y = rect.y();
                }

                if(m_children_ori==Horizontal){
                    w += rect.width();
                    if(h==0){
                        h = rect.height();
                    }
                }else{
                    h += rect.height();
                    if(w==0){
                        w = rect.width();
                    }
                }
            }
            if(count>1){
                if(m_children_ori==Horizontal){
                    w += ((count - 1) * spacing);
                }else{
                    h += ((count - 1) * spacing);
                }
            }

            return QRect(x,y,w,h);
        }else{
            if(m_item!=nullptr){
                rc = m_item->widget()->geometry();
            }
        }
        return rc;
    }

    void DockingPaneLayoutItemInfo::initHandle(QWidget* parent)
    {
        if(m_handle==nullptr)
            m_handle = new DockingPaneHandle(parent,this);
    }

    int DockingPaneLayoutItemInfo::row()
    {
        if(m_parent==nullptr){
            return -1;
        }else{
            return m_parent->indexOf(this);
        }
    }

    int DockingPaneLayoutItemInfo::indexOf(DockingPaneLayoutItemInfo* child)
    {
        int i = 0;
        Q_FOREACH(DockingPaneLayoutItemInfo* one,m_children){
            if(one==child){
                return i;
            }
            i+=1;
        }
        return -1;
    }

    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::next()
    {
        int row = this->row();
        if(row>-1){
            return m_parent->child(row + 1);
        }else{
            return nullptr;
        }
    }

    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::child(int row)
    {
        if(row>-1 && row<m_children.size()){
            return m_children.at(row);
        }else{
            return nullptr;
        }
    }

    bool DockingPaneLayoutItemInfo::resize(Orientation orient,bool leftorright,int stretch_size)
    {
        QRect rc = geometry(m_spacing);
        qDebug()<<"src resize:"<<rc;
        QSize size = calculateSize(MinimumSize,m_spacing);
        if(orient==Horizontal){
            if(leftorright==false){
                //effect width fixed x
                //int x = rc.x();
                //int w = rc.width();
                rc.setWidth(stretch_size - rc.x());
                //rc.setWidth(rc.width() + stretch_size);
            }else{

                rc.setX(stretch_size + m_spacing);
                //rc.setWidth(rc.width() - stretch_size);
            }
            if(size.width()>=rc.width()){
                return false;
            }
            QRect parent_rc = m_parent->geometry(m_spacing);
            //m_stretch = rc.width();
            m_stretch = rc.width() * 1.0f / parent_rc.width();
        }else{
            if(leftorright==false){
                rc.setHeight(stretch_size - rc.y());

            }else{
                rc.setY(stretch_size + m_spacing);
            }
            if(size.height()>=rc.height()){
                return false;
            }
            //m_stretch = rc.height();
            QRect parent_rc = m_parent->geometry(m_spacing);
            m_stretch = rc.height() * 1.0f / parent_rc.height();
        }
        qDebug()<<"dst resize:"<<rc;
        this->setGeometry(rc,m_spacing);

        return true;
    }

    void DockingPaneLayoutItemInfo::setParent(DockingPaneLayoutItemInfo* parent)
    {
        m_parent = parent;
    }

    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::findItemInfo(DockingPaneContainer* container)
    {
        if(m_item!=nullptr){
            if(m_item->widget()==container){
                return this;
            }
        }
        DockingPaneLayoutItemInfo* info = nullptr;
        Q_FOREACH(DockingPaneLayoutItemInfo* one ,m_children){
            //if(one->findParentInfo())
            info = one->findItemInfo(container);
            if(info!=nullptr){
                return info;
            }
        }
        return nullptr;
    }

    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::takeAt(DockingPaneContainer* container)
    {
        DockingPaneLayoutItemInfo* info = nullptr;
         QList<DockingPaneLayoutItemInfo*>::iterator iter = m_children.begin();
         while(iter!=m_children.end()){
            if((*iter)->m_item!=nullptr && (*iter)->m_item->widget() == container){
                info = (*iter);
                break;
            }
             iter++;
         }
         if(info!=nullptr){
             return removeItem(info);
         }else{
            if(m_item!=nullptr){
                if(m_item->widget()==container){
                    delete m_item;
                    m_item = nullptr;
                    m_parent->m_children.removeAt(this->row());
                    return this;//todo
                }
            }
         }
         return nullptr;
    }

    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::level0()
    {
        if(m_parent->m_parent==nullptr){
            return this;
        }else{
            return m_parent->level0();
        }
    }

    bool DockingPaneLayoutItemInfo::isEmpty()
    {
        return m_children.size() == 0 && m_item==nullptr;
    }

    int DockingPaneLayoutItemInfo::childrenCount()
    {
        return m_children.size() + (m_item!=nullptr?1:0);
    }

    QString DockingPaneLayoutItemInfo::toString()
    {
        //QString str = "DockingPaneLayoutItemInfo:{objectName:"+this->objectName()+";parent:"+(m_parent==nullptr?"NULL":m_parent->objectName())+";children:"+m_children.size()+";item:"+(m_item==nullptr?"NULL":m_item->widget()->objectName())+";ori:"+m_children_ori+"}";
        //QString str = "DockingPaneLayoutItemInfo{children:"+m_children.size()+"}";
        QString str = QString("DockingPaneLayoutItemInfo{children:%1,item:%2}").arg(m_children.size()).arg(m_item==nullptr?"null":"not null");
        return str;
    }

    void DockingPaneLayoutItemInfo::dump(QString prefix)
    {
        if(m_children.size()>0){
            qDebug()<<prefix<<"Group size:"<<m_children.size()<<";rect:"<<m_rect;
            foreach(DockingPaneLayoutItemInfo* child,m_children){
                child->dump(prefix +"----");
            }
        }else{
            if(m_item!=nullptr){
                qDebug()<<prefix<<"Item name:"<<m_item->widget()->objectName()<<";rect:"<<m_item->widget()->geometry()<<";info rect:"<<m_rect<<";stretch:"<<m_stretch;
            }else{
                qDebug()<<prefix<<"Item NULL";
            }
        }
        /*qDebug()<<prefix<<"DockingPaneLayoutItemInfo:"<<objectName()<<";size:"<<m_children.size()<<(m_item==nullptr?"NULL":m_item->widget()->objectName());
        foreach(DockingPaneLayoutItemInfo* child,m_children){
            child->dump(prefix +"--");
        }*/
    }


    void DockingPaneLayoutItemInfo::remove()
    {
        if(m_parent!=nullptr){
            int row = this->row();
            DockingPaneLayoutItemInfo* parent = m_parent;
            parent->m_children.removeAt(row);
            delete m_item;
            m_item = nullptr;

            //check parent children equal one
            if(parent->m_children.size()==1){
                //will remove parent
                DockingPaneLayoutItemInfo* neighbor = parent->m_children.at(0);
                if(neighbor->m_children.size()>0){
                    //neighbor has children item
                    DockingPaneLayoutItemInfo* parentParent = parent->parent();
                    int parent_row = parent->row();
                    if(parentParent!=nullptr){
                        int i = 1;
                        foreach(DockingPaneLayoutItemInfo* child,neighbor->m_children){
                            child->setParent(parentParent);
                            parentParent->m_children.insert(parent_row + i,child);
                            i+=1;
                        }

                        //remove parent from parentParent
                        parentParent->m_children.removeAt(parent_row);
                        delete parent;
                        delete neighbor;
                    }else{
                        int neighbor_row = neighbor->row();
                        int i = 1;
                        foreach(DockingPaneLayoutItemInfo* child,neighbor->m_children){
                            child->setParent(parent);
                            parent->m_children.insert(neighbor_row + i,child);
                            i+=1;
                        }
                        parent->setChildrenOrientation(neighbor->childrenOrientation());
                        //remove parent from parentParent
                        parent->m_children.removeAt(neighbor_row);
                        //delete parent;
                        delete neighbor;
                        //qDebug()<<"removeremoveremoveremoveparentParentparentParentparentParent";
                    }

                }else{
                    //neighbor has only one item
                    //m_item = neighbor;
                    parent->m_item = neighbor->item();
                    parent->m_children_ori = Unkown;
                    DockingPaneContainer* container = (DockingPaneContainer*)parent->m_item->widget();
                    if(container!=nullptr){
                        container->setItemInfo(parent);
                    }
                    parent->m_children.clear();
                    delete neighbor;
                }
            }


        }



    }


}
