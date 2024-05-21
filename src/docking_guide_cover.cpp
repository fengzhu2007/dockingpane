#include "docking_guide_cover.h"

namespace ady{

    DockingGuideCover::DockingGuideCover(QWidget* parent)
        :QFrame(parent){
        setWindowFlags( Qt::FramelessWindowHint| Qt::ToolTip);
        //this->setStyleSheet("background:#007acc;border:5px solid #aaa");
        setWindowOpacity(0.3);
    }






}
