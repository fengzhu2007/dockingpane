#include "docking_pane_handle.h"
#include "docking_pane_layout_item_info.h"
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

namespace ady {
    DockingPaneHandle::DockingPaneHandle(QWidget* parent,DockingPaneLayoutItemInfo*itemInfo)
        :QFrame(parent)
    {
        //setStyleSheet("background:#eeeef2");//theme
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
                //qDebug()<<"1"<<pos;
                pos.rx() += x;
                //qDebug()<<"2"<<pos;
                //qDebug()<<"x"<<x<<pos;
                if(x>0){
                    if(next!=nullptr){
                        if(next->resize(DockingPaneLayoutItemInfo::Horizontal,true,pos)==false){
                            //return ;
                            //qDebug()<<"3"<<pos;
                            //return ;
                        }
                    }
                    m_itemInfo->resize(DockingPaneLayoutItemInfo::Horizontal,false,pos);
                }else if(x<0){
                    if(m_itemInfo->resize(DockingPaneLayoutItemInfo::Horizontal,false,pos)==false){
                        //return ;
                    }
                    if(next!=nullptr){
                        next->resize(DockingPaneLayoutItemInfo::Horizontal,true,pos);
                    }
                }else{
                    return ;
                }
                //next->setGeometry()
                next->invalidate();
                m_itemInfo->invalidate();
                move(pos);
            }else if(m_ori==Vertical){
                int y = event->y();
                QPoint pos = this->pos();
                pos.ry() += y;
                if(y>0){
                    if(next!=nullptr){
                        if(next->resize(DockingPaneLayoutItemInfo::Vertical,true,pos.y())==false){
                            //return ;
                        }
                    }
                    m_itemInfo->resize(DockingPaneLayoutItemInfo::Vertical,false,pos.y());
                }else if(y<0){
                    if(m_itemInfo->resize(DockingPaneLayoutItemInfo::Vertical,false,pos.y())==false){
                       // return ;
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
        //qDebug()<<"handle:"<<m_itemInfo<<";seq:"<<m_itemInfo->seq();
        //hide();
        QFrame::mousePressEvent(event);
        m_start_moving = true;
        m_offset = 0;
        //lock parent info setGeometry
        m_itemInfo->parent()->setGeometryState(m_start_moving);//lock
    }

    void DockingPaneHandle::mouseReleaseEvent(QMouseEvent *event)
    {
        QFrame::mouseReleaseEvent(event);
        m_start_moving = false;
        m_itemInfo->parent()->setGeometryState(m_start_moving);//unlock
    }


}
