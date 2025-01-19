#include "docking_pane_tabbar_item.h"
#include "docking_theme.h"
#include <QStylePainter>
#include <QDebug>
namespace ady {

    class DockingPaneTabBarItemPrivate {
    public:
        Qt::Orientation orientation = Qt::Horizontal;
        bool hover = false;
        bool mirrored = false;
        bool swap = false;
        QColor textColor;
        QColor lineColor;
        QColor primaryColor;
    };

    DockingPaneTabBarItem::DockingPaneTabBarItem(QWidget* parent)
        :QPushButton(parent)
    {
        d = new DockingPaneTabBarItemPrivate;
        auto theme = DockingTheme::getInstance();
        d->textColor = theme->textColor();
        d->lineColor = theme->borderColor();
        d->primaryColor = theme->primaryColor();
    }

    DockingPaneTabBarItem::~DockingPaneTabBarItem()
    {
        delete  d;
    }

    QSize DockingPaneTabBarItem::sizeHint() const
    {
        QSize size;
        QFontMetrics fm(this->font());
        size.setWidth(fm.width(this->text()));
        size.setHeight(30);//theme
        if (d->orientation == Qt::Vertical) {
            size.transpose();
        }
        return size;
    }

    void DockingPaneTabBarItem::setOrientation(Qt::Orientation orientation)
    {
        d->orientation = orientation;
        //update();
    }

    void DockingPaneTabBarItem::setSwap(bool swap)
    {
        d->swap = swap;
    }

    void DockingPaneTabBarItem::setMirrored(bool mirrored)
    {
        d->mirrored = mirrored;
    }


    void DockingPaneTabBarItem::paintEvent(QPaintEvent* event)
    {
        Q_UNUSED(event);
        QPainter p(this);
        QColor color, textColor;

        p.setRenderHint(QPainter::Antialiasing, true);
        p.setRenderHint(QPainter::HighQualityAntialiasing, true);

        if (d->hover) {
            color = d->primaryColor;
        } else {
            //#444444


            //textColor = Qt::white;
            //textColor = QColor("#444444");
            color = d->lineColor;
        }

        p.setPen(d->textColor);


        //qDebug()<<d->orientation<<";m:"<<d->mirrored<<";s:"<<d->swap;
        //qDebug()<<"color"<<d->textColor.name(QColor::HexRgb);

        switch (d->orientation) {
            case Qt::Horizontal: {
                if (d->mirrored) {
                    p.rotate(180);
                    p.translate(-width(), -height());
                }
                if (d->swap) {
                    p.fillRect(0, height()-6, width(), 6, color);
                    p.drawText(0, 6, width(), height()-10, 0, this->text());
                } else {
                    p.fillRect(0, 0, width(), 6, color);
                    p.drawText(0, 6, width(), height(), 0, this->text());
                }
                break;
            }

            case Qt::Vertical: {
                if (d->mirrored) {
                    p.rotate(-90);
                    p.translate(-height(), 0);
                } else {
                    p.rotate(90);
                    p.translate(0, -width());
                }
                QTransform transform = p.worldTransform();
                QPointF origin = transform.map(QPointF(0, 0));
                qDebug()<<"origin"<<origin;
                if (d->swap) {
                    p.fillRect(0, 0, height(), 6, color);
                    p.drawText(0, 10, height(), width(), 0, this->text());
                } else {
                    p.fillRect(0, width()-6, height(), 6, color);
                    p.drawText(0, 6, height(), width()-10, Qt::AlignCenter, this->text());
                }

                break;
             }
        }
    }

    void DockingPaneTabBarItem::enterEvent(QEvent *event)
    {
        d->hover = true;
        QPushButton::enterEvent(event);
#ifdef Q_OS_MAC
        update();
#endif
    }

    void DockingPaneTabBarItem::leaveEvent(QEvent *event)
    {
        d->hover = false;
        QPushButton::leaveEvent(event);
#ifdef Q_OS_MAC
        update();
#endif
    }
}
