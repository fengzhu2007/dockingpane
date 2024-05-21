#ifndef DOCKING_PANE_CLIENT_REGION_ITEM_INFO_H
#define DOCKING_PANE_CLIENT_REGION_ITEM_INFO_H
#include "docking_pane_layout_item_info.h"
namespace ady{
class DOCKINGPANE_EXPORT DockingPaneClientRegionItemInfo :public DockingPaneLayoutItemInfo{
public:
    DockingPaneClientRegionItemInfo(QLayoutItem* item,DockingPaneManager::Position position,DockingPaneLayoutItemInfo* parent=nullptr);
    virtual bool isClientRegion();

};


}
#endif // DOCKING_PANE_CLIENT_REGION_ITEM_INFO_H
