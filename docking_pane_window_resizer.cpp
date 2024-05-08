#include "docking_pane_window_resizer.h"
#include "docking_pane_float_window.h"

#include <QMouseEvent>
#include <QDebug>
namespace ady{
    class DockingPaneWindowResizerPrivate {
    public:
        DockingPaneWindowResizer::Region region;

        bool moving = false;
        int offsetX = 0;
        int offsetY = 0;
    };

    DockingPaneWindowResizer::DockingPaneWindowResizer(QWidget* parent,Region region)
        :QFrame(parent)
    {
        d = new DockingPaneWindowResizerPrivate;
        d->region = region;
        //setStyleSheet("background:black");
        switch(d->region){
        case Left:
        case Right:
            this->setCursor(Qt::SizeHorCursor);
            break;
        case Top:
        case Bottom:
            this->setCursor(Qt::SizeVerCursor);
            break;
        case LeftTop:
        case RightBottom:
            this->setCursor(Qt::SizeFDiagCursor);
            break;
        case LeftBottom:
        case RightTop:
            this->setCursor(Qt::SizeBDiagCursor);
            break;
        }
        this->setAttribute(Qt::WA_TranslucentBackground);

    }

    DockingPaneWindowResizer::~DockingPaneWindowResizer()
    {
        delete d;
    }

    void DockingPaneWindowResizer::setRegion(Region region)
    {
        d->region = region;
        switch(d->region){
        case Left:
        case Right:
            this->setCursor(Qt::SizeHorCursor);
            break;
        case Top:
        case Bottom:
            this->setCursor(Qt::SizeVerCursor);
            break;
        case LeftTop:
        case RightBottom:
            this->setCursor(Qt::SizeFDiagCursor);
            break;
        case LeftBottom:
        case RightTop:
            this->setCursor(Qt::SizeBDiagCursor);
            break;
        }
    }

    DockingPaneWindowResizer::Region DockingPaneWindowResizer::region()
    {
        return d->region;
    }

    void DockingPaneWindowResizer::mouseMoveEvent(QMouseEvent *event)
    {
        QWidget::mouseMoveEvent(event);
        if(d->moving){
            QWidget* parent = (QWidget*)parentWidget();
            if(parent!=nullptr){
                    int x = event->x() ;
                    int y = event->y();
                    x -= d->offsetX;
                    y -= d->offsetY;
                    QPoint pos = parent->pos();
                    pos.rx() += x;
                    pos.ry() += y;
                    QRect rc = parent->geometry();
                    if(d->region==DockingPaneWindowResizer::Left || d->region==DockingPaneWindowResizer::LeftTop || d->region==DockingPaneWindowResizer::LeftBottom){
                        rc.setX(rc.x() + x);
                    }
                    if(d->region==DockingPaneWindowResizer::Right || d->region==DockingPaneWindowResizer::RightTop || d->region==DockingPaneWindowResizer::RightBottom){
                        rc.setWidth(rc.width() + x);
                    }
                    if(d->region==DockingPaneWindowResizer::Top || d->region==DockingPaneWindowResizer::LeftTop || d->region==DockingPaneWindowResizer::RightTop){
                        rc.setY(rc.y() + y);
                    }
                    if(d->region==DockingPaneWindowResizer::Bottom || d->region==DockingPaneWindowResizer::LeftBottom || d->region==DockingPaneWindowResizer::RightBottom){
                        rc.setHeight(rc.height() + y);
                    }
                    parent->setGeometry(rc);
                    //parent->updateResizer();
            }
        }
    }

    void DockingPaneWindowResizer::mousePressEvent(QMouseEvent *e)
    {
        QWidget::mousePressEvent(e);
        d->moving = true;
        d->offsetX = e->x();
        d->offsetY = e->y();
    }

    void DockingPaneWindowResizer::mouseReleaseEvent(QMouseEvent *e)
    {
        QWidget::mouseReleaseEvent(e);
        d->moving = false;
    }


}
