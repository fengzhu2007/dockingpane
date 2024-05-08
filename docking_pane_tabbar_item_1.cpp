#include "docking_pane_tabbar_item_1.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QDebug>
namespace ady {

    /*class DockingPaneTabBarItemPrivate {
    public:
        DockingPaneTabBarItemV2* label;

    };

    DockingPaneTabBarItem::DockingPaneTabBarItem(QWidget* parent)
        :QFrame(parent)
    {
        d = new DockingPaneTabBarItemPrivate;
        d->label = new DockingPaneTabBarItemV2(this);
        qDebug()<<"rect1:"<<d->label->geometry()<<this->geometry();
        QRect rect = this->geometry();
        this->setGeometry(0,0,rect.height(),rect.width());
        d->label->setGeometry(0,0,rect.height(),rect.width());
        qDebug()<<"rect2:"<<d->label->geometry()<<this->geometry();
        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->setMargin(0);

        layout->addWidget(d->label);
        setStyleSheet("background:red");
        this->setLayout(layout);
    }

    DockingPaneTabBarItem::~DockingPaneTabBarItem()
    {
        delete d;
    }

    void DockingPaneTabBarItem::setText(QString text)
    {
        d->label->setText(text);
    }

    QString DockingPaneTabBarItem::text()
    {
        return d->label->text();
    }
*/





    DockingPaneTabBarItemV2::DockingPaneTabBarItemV2(QWidget* parent)
        :QLabel(parent){
        setStyleSheet("border:1px solid green");
    }

    void DockingPaneTabBarItemV2::initPainter(QPainter *painter) const
    {
        painter->translate(this->width() / 2 , this->height() / 2);
        painter->rotate(90);
        painter->translate(-this->width() / 2 , -this->height() / 2);
        QLabel::initPainter(painter);
    }
}
