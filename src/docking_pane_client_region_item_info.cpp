#include "docking_pane_client_region_item_info.h"

namespace ady{

DockingPaneClientRegionItemInfo::DockingPaneClientRegionItemInfo(QLayoutItem* item,DockingPaneManager::Position position,DockingPaneLayoutItemInfo* parent)
    :DockingPaneLayoutItemInfo(item,position,parent){


}

bool DockingPaneClientRegionItemInfo::isClientRegion(){
    return true;
}
}
