#include "docking_pane_layout_item_info.h"
#include "docking_pane_handle.h"
#include "docking_pane_container.h"
#include "docking_pane.h"
#include <QDebug>
namespace ady {
int DockingPaneLayoutItemInfo::gSeq = 0;

    DockingPaneLayoutItemInfo::DockingPaneLayoutItemInfo(QLayoutItem* item,DockingPaneManager::Position position,DockingPaneLayoutItemInfo* parent)
        :QObject()
    {
        m_item = item;
        m_position = position;
        m_parent = parent;
        m_handle = nullptr;
        m_stretch = None;
        m_temp_stretch = 0.0f;
        m_temp_size = 0;
        m_lock_state = false;
        m_seq = DockingPaneLayoutItemInfo::gSeq++;
    }

    DockingPaneLayoutItemInfo::~DockingPaneLayoutItemInfo(){
        if(m_handle!=nullptr){
            m_handle->close();
            delete m_handle;
            m_handle = nullptr;
        }
    }

    bool DockingPaneLayoutItemInfo::isClient(){
        if(this->m_item!=nullptr){
            return ((DockingPaneContainer*)this->m_item->widget())->isClient();
        }else if(m_children.size()==1){
            return m_children.at(0)->isClient();
        }else{
            return false;
        }
    }

    bool DockingPaneLayoutItemInfo::hasClient(){
        bool ret = false;
        if(m_children.size()>0){
            for(auto one:m_children){
                ret = one->hasClient();
                if(ret){
                    break;
                }
            }
        }else if(m_item!=nullptr){
            ret = this->isClient();
        }
        return ret;
    }

    QList<DockingPaneLayoutItemInfo*> DockingPaneLayoutItemInfo::clientChildren(){
        QList<DockingPaneLayoutItemInfo*> list;
        Q_FOREACH(DockingPaneLayoutItemInfo* one,m_children){
            if(one->isClient()){
                list.push_back(one);
            }
        }
        return list;
    }

    void DockingPaneLayoutItemInfo::setChildrenOrientation(Orientation orientation)
    {
        m_children_ori = orientation;
    }




    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::insertItem(QWidget* workbench,QLayoutItem* item, DockingPaneManager::Position position)
    {
        int size = m_children.size();
        float stretch = -10;
        //qDebug()<<"insertItem1:";
        if(size==0 && m_item!=nullptr){
            DockingPaneLayoutItemInfo* child = new DockingPaneLayoutItemInfo(m_item,position,this);
            ((DockingPaneContainer*)m_item->widget())->setItemInfo(child);
            child->setObjectName(m_item->widget()->objectName()+"_itemInfo");
            child->initHandle(workbench);
            m_children.push_back(child);
            //qDebug()<<"container1:"<<m_item->widget();
        }else{
            DockingPaneContainer* container = static_cast<DockingPaneContainer*>(item->widget());
            if(container!=nullptr && container->isClient()==false){
                stretch = container->stretch();
            }
            //qDebug()<<"container2:"<<container<<stretch;
        }

        DockingPaneLayoutItemInfo* child = new DockingPaneLayoutItemInfo(item,position,this);
        child->setStretch(stretch);
        //qDebug()<<"insertItem:"<<stretch;

        ((DockingPaneContainer*)item->widget())->setItemInfo(child);
        child->setObjectName(item->widget()->objectName()+"_itemInfo");
        child->initHandle(workbench);
        if(position==DockingPaneManager::Top || position==DockingPaneManager::Left || position==DockingPaneManager::S_Top || position==DockingPaneManager::S_Left ){
            m_children.insert(0,child);
        }else{
            m_children.push_back(child);
        }
        if(m_children_ori==Unkown){
            if(position==DockingPaneManager::Top || position==DockingPaneManager::Bottom || position==DockingPaneManager::S_Top ||position==DockingPaneManager::S_Bottom){
                m_children_ori = Vertical;
            }else if(position==DockingPaneManager::Left || position==DockingPaneManager::Right || position==DockingPaneManager::S_Left ||position==DockingPaneManager::S_Right){
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
        return child;
    }

    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::insertItem(QWidget* workbench,QLayoutItem* item,DockingPaneManager::Position position,int index)
    {
        //qDebug()<<"insertItem2:";
        int size = m_children.size();
        float stretch = -10;
        if(size==0 && m_item!=nullptr){
            DockingPaneLayoutItemInfo* child = new DockingPaneLayoutItemInfo(m_item,position,this);
            ((DockingPaneContainer*)m_item->widget())->setItemInfo(child);
            child->setObjectName(m_item->widget()->objectName()+"_itemInfo");
            child->initHandle(workbench);
            m_children.push_back(child);
        }else{
            DockingPaneContainer* container = static_cast<DockingPaneContainer*>(item->widget());
            if(container!=nullptr && container->isClient()==false){
                stretch = container->stretch();
            }
            //qDebug()<<"container3:"<<item->widget()<<stretch;
        }

        DockingPaneLayoutItemInfo* child = new DockingPaneLayoutItemInfo(item,position,this);
        child->setStretch(stretch);
        ((DockingPaneContainer*)item->widget())->setItemInfo(child);
        child->setObjectName(item->widget()->objectName()+"_itemInfo");
        child->initHandle(workbench);
        m_children.insert(index,child);
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
        return child;
    }

    void DockingPaneLayoutItemInfo::appendItem(DockingPaneLayoutItemInfo* child){
        m_children<<child;
    }

    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::removeItem(DockingPaneLayoutItemInfo* itemInfo)
    {
        QList<DockingPaneLayoutItemInfo*>::iterator iter = m_children.begin();
        while(iter!=m_children.end()){
            if(itemInfo==(*iter)){
                delete (*iter);
                m_children.erase(iter);
                goto reset_layout_tree;
                break;
            }
            iter++;
        }
        delete itemInfo;
        reset_layout_tree:
        if(m_children.size()<=1){
            m_children_ori = Unkown;
        }
        if(m_children.size()==1 && m_parent!=nullptr){
            DockingPaneLayoutItemInfo* first = m_children.at(0);
            m_item = first->item();
            if(m_item!=nullptr){
                delete first;
                m_children.clear();
                DockingPaneContainer* container = (DockingPaneContainer*)m_item->widget();
                container->setItemInfo(this);
            }else{
                //set all children`s parent as this parent,and delete this later
                int row = this->row();
                int i = 1;
                DockingPaneLayoutItemInfo* parent = this->parent();
                foreach(auto child,first->m_children){
                    child->setParent(parent);
                    parent->m_children.insert(row+i,child);
                    i+=1;
                }
                //remove this from this.parent
                parent->m_children.removeAt(row);
                m_children.clear();
                delete first;
                return this;
            }
        }
        return nullptr;
    }

    QLayoutItem* DockingPaneLayoutItemInfo::itemAt(int &index)
    {
        if(m_children.size()>0){
            for(auto one:m_children){
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
        for(auto one:m_children){
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
            int w = 0;
            int h = 0;
            for(auto one:m_children){
                QSize size = one->calculateSize(sizeType,spacing);
                if(m_children_ori==Horizontal){
                    h = qMax(h,size.height());
                    totalSize.setHeight(h);
                    totalSize.rwidth() += size.width();
                }else{
                    w = qMax(w,size.width());
                    totalSize.rheight() += size.height();
                    totalSize.setWidth(w);
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
                    //totalSize = m_item->minimumSize();
                    totalSize = QSize(50,50);
                }else{
                    totalSize = m_item->sizeHint();
                }
            }

        }
        //qDebug()<<this<<sizeType<<totalSize;
        return totalSize;
    }

    void DockingPaneLayoutItemInfo::setGeometry(const QRect &rect,int spacing)
    {
        //qDebug()<<this<<rect;
        if(m_lock_state==true){
            return ;
        }
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
                /*
                Recalculate steps:
                1. First subtract the fixed size and the automatic expansion (calculated according to the minimum size) size
                2. Use the margin in the previous step to calculate the display size of the percentage item
                3. If there is an item smaller than the minimum size in the previous step, recalculate the size of the fixed item
                */
                int totalSize = m_children_ori==Horizontal?rect.width():rect.height();
                int leftSize = totalSize;
                int autoSize = 0;
                int minSize = 50;
                int manualSizeTotal = 0;

                for(auto one:m_children){
                    one->m_temp_stretch = 0;
                    one->m_temp_size = 0;
                    int manual_size = one->manualSize();
                    if(manual_size>0){
                        leftSize -= manual_size;
                        manualSizeTotal += manual_size;
                    }else if(one->m_stretch>0){
                        int size = one->m_stretch * totalSize;
                        if(size<minSize){
                            //need calc;
                            one->m_temp_size = minSize;
                        }else{
                            one->m_temp_size = size;
                        }
                        leftSize -= one->m_temp_size;
                        autoSize += one->m_temp_size;
                    }else{
                        leftSize -= minSize;
                        autoSize += minSize;
                    }

                }
                if(leftSize<0){
                    //re calc
                    //float manualSizePercent = (totalSize - autoSize) * 1.0 / totalSize;
                    int realManualSize = totalSize - autoSize;//Actual manual layout remaining size
                    for(auto one:m_children){
                        int manual_size = one->manualSize();
                        if(manual_size>0){
                             one->m_temp_size = manual_size * 1.0 / manualSizeTotal * realManualSize;
                            if(one->m_temp_size<minSize){
                                 one->m_temp_size = minSize;
                            }
                        }
                    }
                }
                //Calculate the size of each item evenly distributed
                leftSize = totalSize;
                int unknowSizeCount = count;//Number of items of unknown size
                for(auto one:m_children){
                    if(one->m_temp_size>0){
                        leftSize -= (one->m_temp_size);
                        unknowSizeCount -= 1;
                    }else if(one->m_manual_size>0){
                        leftSize -= one->m_manual_size;
                        unknowSizeCount -= 1;
                    }
                }

                if(m_children_ori==Horizontal){
                    w = unknowSizeCount>0?leftSize / unknowSizeCount:0;
                    h = rect.height();
                }else{
                    w = rect.width();
                    h = unknowSizeCount>0?leftSize / unknowSizeCount:0;
                }
                //qDebug()<<totalSize<<leftSize<<unknowSizeCount<<"wwww:"<<w<<"hhhh:"<<h;
            }


            int offset = 0;
            int i = 0;
            for(auto one:m_children){
                QRect child_rc (x,y,w,h);
                if(one->m_temp_size>0){
                    if(m_children_ori==Horizontal){
                        child_rc.setWidth(one->m_temp_size);
                    }else{
                        child_rc.setHeight(one->m_temp_size);
                    }
                }else if(one->m_manual_size>0){
                    if(m_children_ori==Horizontal){
                        child_rc.setWidth(one->m_manual_size);
                    }else{
                        child_rc.setHeight(one->m_manual_size);
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
                QRect rc = one->geometry(spacing);

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

            for(auto one:m_children){
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

    void DockingPaneLayoutItemInfo::setGeometryState(bool state){
        //state=true lock
        //state=false unlock

    }

    QRect DockingPaneLayoutItemInfo::clientGeometry(){
        QRect rc;
        int count = m_children.size();
        //int count = 0;
        if(count>0){
            int w = 0;
            int h = 0;
            int x = -1;
            int y = -1;
            int i = 0;
            for(auto one:m_children){
                DockingPaneContainer* container = (DockingPaneContainer*)one->item()->widget();
                if(container->isClient()){
                    i += 1;
                    QRect rect = one->geometry(m_spacing);
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

            }
            if(i>1){
                if(m_children_ori==Horizontal){
                    w += ((i - 1) * m_spacing);
                }else{
                    h += ((i - 1) * m_spacing);
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
        if(m_handle==nullptr){
            m_handle = new DockingPaneHandle(parent,this);
            m_handle->hide();
        }
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
        for(auto one:m_children){
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

    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::previous(){
        int row = this->row();
        if(row>0){
            return m_parent->child(row - 1);
        }else{
            return nullptr;
        }
    }

    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::first(){
        return this->child(0);
    }
    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::last(){
        return this->child(m_children.size() - 1);
    }

    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::child(int row)
    {
        if(row>-1 && row<m_children.size()){
            return m_children.at(row);
        }else{
            return nullptr;
        }
    }

    /**
     * @brief DockingPaneLayoutItemInfo::resize
     * @param orient
     * @param leftorright
     * @param stretch_size move size
     * @return
     */
    bool DockingPaneLayoutItemInfo::resize(Orientation orient,bool leftorright,int stretch_size)
    {
        QRect rc = geometry(m_spacing);
        QSize size = calculateSize(MinimumSize,m_spacing);
        if(orient==Horizontal){
            if(leftorright==false){
                rc.setWidth(stretch_size - rc.x());
            }else{
                rc.setX(stretch_size + m_spacing);
            }
            if(rc.width() < size.width()){
                return false;
            }
            QRect parent_rc = m_parent->geometry(m_spacing);
            //qDebug()<<this<<m_stretch;
            if(m_stretch>1){
                m_stretch = rc.width() ;
                this->setChildrenStretch(m_stretch);
            }else{
                m_stretch = rc.width() * 1.0f / parent_rc.width();
                this->setChildrenStretch(m_stretch);
            }
            this->m_manual_size = rc.width();
        }else{
            if(leftorright==false){
                rc.setHeight(stretch_size - rc.y());
            }else{
                rc.setY(stretch_size + m_spacing);
            }
            if(rc.height() < size.height()){
                return false;
            }
            QRect parent_rc = m_parent->geometry(m_spacing);
            if(m_stretch>1){
                m_stretch = rc.height() ;
                this->setChildrenStretch(m_stretch);
            }else{
                m_stretch = rc.height() * 1.0f / parent_rc.height();
                this->setChildrenStretch(m_stretch);
            }
            this->m_manual_size = rc.height();
        }
        this->setGeometry(rc,m_spacing);
        return true;
    }

    bool DockingPaneLayoutItemInfo::resize(Orientation orient,bool leftorright,QPoint& pos){
        QRect rc = m_rect;
        QSize size = calculateSize(MinimumSize,m_spacing);
        bool ret = true;
        if(orient==Horizontal){
            int x = pos.x();
            if(leftorright==false){
                rc.setWidth(x - rc.x());
            }else{
                rc.setX(x + m_spacing);//change x and width
            }
            if(rc.width() < size.width()){
                int s = qAbs(size.width() - rc.width());
                if(leftorright==false){
                    //from right to left
                    pos.rx() += s;
                    if(pos.x()<rc.right()){
                        pos.setX(rc.right());
                    }
                    rc.setWidth(size.width());//set min width
                }else{
                    //from left to right
                    int x = rc.right() - size.width();
                    pos.setX(x - m_spacing);
                    rc.setX(x);

                }
                ret = false;
            }
            if(hasClient()==false){
                m_stretch = rc.width() * 1.0f / m_parent->m_rect.width();
                this->m_manual_size = rc.width();
            }else{
                m_stretch = -10;
                this->m_manual_size = 0;
            }

        }else{
            int y = pos.y();
            if(leftorright==false){
                rc.setHeight(y - rc.y());
            }else{
                rc.setY(y + m_spacing);
            }
            if(rc.height() < size.height()){
                int s = qAbs(size.height() - rc.height());
                if(leftorright==false){
                    //from bottom to top
                    pos.ry() += s;
                    if(pos.y()<rc.bottom()){
                        pos.setX(rc.bottom());
                    }
                    rc.setHeight(size.width());//set min width
                }else{
                    //from top to bottom
                    int y = rc.bottom() - size.height();
                    pos.setY(y - m_spacing);
                    rc.setY(y);
                }
                rc.setHeight(size.height());//set min height
                ret = false;
            }
            if(hasClient()==false){
                m_stretch = rc.height() * 1.0f / m_parent->m_rect.height();
                this->m_manual_size = rc.height();
            }else{
                m_stretch = -10;
                this->m_manual_size = 0;
            }
        }
        this->setChildrenStretch(m_stretch);
        this->m_rect = rc;
        return ret;
    }

    void DockingPaneLayoutItemInfo::invalidate(){
        this->setGeometry(this->m_rect,m_spacing);
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
        for(auto one:m_children){
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

    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::take(DockingPaneLayoutItemInfo* itemInfo){
        QList<DockingPaneLayoutItemInfo*>::iterator iter = m_children.begin();
        while(iter!=m_children.end()){
            if((*iter)==itemInfo){
                m_children.erase(iter);
                itemInfo->setParent(nullptr);
                break;
            }
            iter++;
        }
        return itemInfo;
    }

    DockingPaneLayoutItemInfo* DockingPaneLayoutItemInfo::level0()
    {
        if(m_parent->m_parent==nullptr){
            return this;
        }else{
            return m_parent->level0();
        }
    }

    DockingPaneContainer* DockingPaneLayoutItemInfo::container(){
        if(m_item!=nullptr){
            return static_cast<DockingPaneContainer*>(m_item->widget());
        }else{
            return nullptr;
        }
    }

    void DockingPaneLayoutItemInfo::setChildrenStretch(float stretch){
        //qDebug()<<"setChildrenStretch"<<this<<stretch;
        auto container = this->container();
        if(container!=nullptr /*&& container->isClient()==false*/){
            int count = container->paneCount();
            for(int i=0;i<count;i++){
                auto pane = container->pane(i);
                pane->setStretch(stretch);
            }
        }
    }

    void DockingPaneLayoutItemInfo::setStretch(float stretch){
        //qDebug()<<"setStretch"<<this<<stretch;
        //qDebug()<<"setStretch:"<<std::isnan(stretch);
        if(std::isnan(stretch)){

             int i = 0;

        }
        if(stretch>1){



        }
        //Q_ASSERT(stretch<=1.0f);//false is error
        this->m_stretch = stretch;
        this->setChildrenStretch(stretch);
    }


    int DockingPaneLayoutItemInfo::stretchSize(){
        int size = 0;
        int orientation = parent()->childrenOrientation();
        auto rect = geometry(m_spacing);
        if(orientation==Vertical){
            size = rect.height();
        }else{
            size = rect.width();
        }
        return size;
    }

    void DockingPaneLayoutItemInfo::calculateStretch(){
        /*int count = m_children.size();
        if(count>0){
            int total = 0;
            for(auto one:m_children){
                total += one->manualSize();
            }
            float s = 1.0f;
            int i = 0;
            for(auto one:m_children){
                float stretch = one->manualSize() * 1.0 / total;
                if(++i<count){
                    one->setStretch(stretch);
                    s -= stretch;
                }else{
                    one->setStretch(s);
                }
            }
        }*/
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
        QString str = QString("DockingPaneLayoutItemInfo{children:%1,item:%2}").arg(m_children.size()).arg(m_item==nullptr?"null":"not null");
        return str;
    }

    void DockingPaneLayoutItemInfo::dump(QString prefix)
    {
        //qDebug()<<prefix<<":"<<this;
        if(m_children.size()>0){
            qDebug()<<prefix<<this;
            foreach(DockingPaneLayoutItemInfo* child,m_children){
                child->dump(prefix +"----");
            }
        }else{
            if(m_item!=nullptr){
                //qDebug()<<prefix<<this<<";ori:"<<m_children_ori<<m_item->widget()<<((DockingPaneContainer*)m_item->widget())->itemInfo()<<"parent:"<<parent();
                qDebug()<<prefix<<this<<";ori:"<<m_children_ori<<m_item->widget()<<((DockingPaneContainer*)m_item->widget())->itemInfo()->m_manual_size<<m_stretch;
            }else{
                qDebug()<<this<<prefix<<"seq:"<<m_seq<<";Item NULL";
            }
        }
    }


    void DockingPaneLayoutItemInfo::remove()
    {
        if(m_parent!=nullptr){
            int row = this->row();
            DockingPaneLayoutItemInfo* parent = m_parent;
            parent->m_children.removeAt(row);
            delete m_item;
            m_item = nullptr;

            //check parent children equal one and parent->parent() is not root item
            if(parent->m_children.size()==1 ){
                parent->setChildrenOrientation(Unkown);
                //will remove parent
                if(parent->parent()!=nullptr){
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


}
