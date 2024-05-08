#include "docking_pane_client.h"
#include "docking_workbench.h"
#include "docking_pane_container_tabbar.h"

#include <QStyleOption>
#include <QPainter>
namespace ady {

    DockingPaneClient::DockingPaneClient(DockingWorkbench* parent)
        :DockingPaneContainer(parent,true)
    {
        DockingPaneContainerTabBar* tabBar = this->tabBar();
        tabBar->setTabsClosable(true);

        //this->setStyleSheet("border:1px solid red;");
        //setStyleSheet("background:red");//theme
    }


    void DockingPaneClient::focusInEvent(QFocusEvent *event)
    {
        QWidget::focusInEvent(event);

    }

    void DockingPaneClient::focusOutEvent(QFocusEvent *event)
    {
        QWidget::focusOutEvent(event);

    }

    void DockingPaneClient::paintEvent(QPaintEvent *e)
    {
        //QFrame::paintEvent(e);
        Q_UNUSED(e);
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

}
