#ifndef DOCKING_PANE_LAYOUT_ITEM_INFO_H
#define DOCKING_PANE_LAYOUT_ITEM_INFO_H
#include <QLayoutItem>
#include <QList>
#include "docking_pane_manager.h"
#include "global.h"
namespace ady {
    class DockingPaneHandle;
    class DockingPaneLayout;
    class DockingPaneContainer;
    class DockingPaneClientRegionItemInfo;
    class DOCKINGPANE_EXPORT DockingPaneLayoutItemInfo : public QObject {
    public:
        enum Orientation {
            Unkown,
            Horizontal,
            Vertical
        };
        enum SizeType {
            MinimumSize,
            SizeHint
        };

        enum Stretch {
            None=-10,
            Auto=-2,
            Fill=-1
        };

        DockingPaneLayoutItemInfo(QLayoutItem* item,DockingPaneManager::Position position,DockingPaneLayoutItemInfo* parent=nullptr);
        ~DockingPaneLayoutItemInfo();
        bool isClient();
        QList<DockingPaneLayoutItemInfo*> clientChildren();
        void setChildrenOrientation(Orientation orientation);
        DockingPaneLayoutItemInfo* insertItem(QWidget* workbench,QLayoutItem* item,DockingPaneManager::Position position);
        DockingPaneLayoutItemInfo* insertItem(QWidget* workbench,QLayoutItem* item,DockingPaneManager::Position position,int index);

        inline QLayoutItem* item(){return m_item;}
        inline void setItem(QLayoutItem* item){m_item = item;}
        QLayoutItem* itemAt(int &index);

        int size();


        QSize calculateSize(SizeType sizeType,int spacing) const;

        void setGeometry(const QRect &rect,int spacing);
        QRect geometry(int spacing);
        QRect clientGeometry();

        void initHandle(QWidget* parent);

        int row();
        int indexOf(DockingPaneLayoutItemInfo* child);
        DockingPaneLayoutItemInfo* next();
        DockingPaneLayoutItemInfo* previous();
        DockingPaneLayoutItemInfo* child(int row);

        bool resize(Orientation orient,bool leftorright,int stretch_size);

        void setParent(DockingPaneLayoutItemInfo* parent);
        inline DockingPaneLayoutItemInfo* parent(){return m_parent;}


        DockingPaneLayoutItemInfo* findItemInfo(DockingPaneContainer* container);

        DockingPaneLayoutItemInfo* removeItem(DockingPaneLayoutItemInfo* itemInfo);
        DockingPaneLayoutItemInfo* takeAt(DockingPaneContainer* container);
        DockingPaneLayoutItemInfo* take(DockingPaneLayoutItemInfo* itemInfo);

        DockingPaneLayoutItemInfo* level0();




        inline Orientation childrenOrientation(){return m_children_ori;}
        inline DockingPaneHandle* handle(){return m_handle;};

        bool isEmpty();

        int childrenCount();

        void remove();//remove self

        inline int spacing(){return m_spacing;}

        QString toString();

        void dump(QString prefix);

        inline int seq(){return m_seq;}

    public:
        static int gSeq;

    private:
        QList<DockingPaneLayoutItemInfo*> m_children;
        Orientation m_children_ori = Unkown;
        QLayoutItem* m_item;

        DockingPaneLayoutItemInfo* m_parent;
        DockingPaneManager::Position m_position;
        DockingPaneHandle* m_handle;
        float m_stretch;
        int m_spacing;
        int m_seq;
        QRect m_rect;



    friend class DockingPaneLayout;
    friend class DockingPaneManager;
    };
}
#endif // DOCKING_PANE_LAYOUT_ITEM_INFO_H
