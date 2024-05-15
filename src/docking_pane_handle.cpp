#include "docking_pane_handle.h"
#include "docking_pane_layout_item_info.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

namespace ady {
    DockingPaneHandle::DockingPaneHandle(QWidget* parent,DockingPaneLayoutItemInfo*itemInfo)
        :QFrame(parent)
    {
        //this->setStyleSheet("background:orange");
        setStyleSheet("background:#eeeef2");//theme
        m_itemInfo = itemInfo;
    }

    void DockingPaneHandle::setOrientation(Orientation orient)
    {
        m_ori = orient;
        if(m_ori==Horizontal){
            this->setCursor(Qt::SizeHorCursor);
        }else{
            this->setCursor(Qt::SizeVerCursor);
        }
    }

    DockingPaneHandle::Orientation DockingPaneHandle::orientation()
    {
        return m_ori;
    }

    void DockingPaneHandle::mouseMoveEvent(QMouseEvent *event)
    {
        QFrame::mouseMoveEvent(event);
        if(m_itemInfo!=nullptr){
            int width = parentWidget()->width();
            int height = parentWidget()->height();
            DockingPaneLayoutItemInfo* next = m_itemInfo->next();
            if(m_ori==Horizontal){
                int x = event->x() ;
                QPoint pos = this->pos();
                pos.rx() += x;
                if(x>0){
                    if(next!=nullptr){
                        if(next->resize(DockingPaneLayoutItemInfo::Horizontal,true,pos.x())==false){
                            return ;
                        }
                    }
                    m_itemInfo->resize(DockingPaneLayoutItemInfo::Horizontal,false,pos.x());
                }else if(x<0){
                    if(m_itemInfo->resize(DockingPaneLayoutItemInfo::Horizontal,false,pos.x())==false){
                        return ;
                    }
                    if(next!=nullptr){
                        next->resize(DockingPaneLayoutItemInfo::Horizontal,true,pos.x());
                    }
                }else{
                    return ;
                }
                move(pos);
            }else if(m_ori==Vertical){
                int y = event->y();
                QPoint pos = this->pos();
                pos.ry() += y;
                if(y>0){
                    if(next!=nullptr){
                        if(next->resize(DockingPaneLayoutItemInfo::Vertical,true,pos.y())==false){
                            return ;
                        }
                    }
                    m_itemInfo->resize(DockingPaneLayoutItemInfo::Vertical,false,pos.y());
                }else if(y<0){
                    if(m_itemInfo->resize(DockingPaneLayoutItemInfo::Vertical,false,pos.y())==false){
                        return ;
                    }
                    if(next!=nullptr){
                        next->resize(DockingPaneLayoutItemInfo::Vertical,true,pos.y());
                    }
                }else{
                    return ;
                }
                move(pos);
            }
        }
        //qDebug()<<"x:"<<event->x();
        /*QRect rc = geometry();
        rc.setX(rc.x() + event->localPos().x());
        rc.setWidth(rc.width());
        setGeometry(rc);
        qDebug()<<event->localPos()<<rc;*/

        //qDebug()<<rc;
    }

    void DockingPaneHandle::mousePressEvent(QMouseEvent *event)
    {
        QFrame::mousePressEvent(event);
        m_start_moving = true;
        m_offset = 0;
    }

    void DockingPaneHandle::mouseReleaseEvent(QMouseEvent *event)
    {
        QFrame::mouseReleaseEvent(event);
        m_start_moving = false;
    }


}
