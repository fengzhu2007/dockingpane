#include "docking_pane_client_region.h"
#include "docking_workbench.h"
#include "docking_pane_container_tabbar.h"

#include <QStyleOption>
#include <QPainter>
namespace ady {

DockingPaneClientRegion::DockingPaneClientRegion(DockingWorkbench* parent)
    :DockingPaneContainer(parent,true,false)
{

}

bool DockingPaneClientRegion::isClientRegion(){
    return true;
}


void DockingPaneClientRegion::paintEvent(QPaintEvent *e)
{
    //QFrame::paintEvent(e);
    Q_UNUSED(e);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

}
