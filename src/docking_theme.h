#ifndef DOCKING_THEME_H
#define DOCKING_THEME_H
#include <QColor>
#include <QString>

namespace ady{
    class DockingTheme{
    public:
        enum Style{
          Light,
          Dark
        };

        enum IconType{
            DropDown=0,
            Pin,
            PinFixed,
            Restore,
            Max,
            Close,
            TitleBackground,
            TabClose,

        };
        enum IconState{
            Normal=0,
            Active
        };

        virtual ~DockingTheme();
        virtual QString name()=0;
        virtual QColor color()=0;//default color
        virtual QColor primaryColor()=0;
        virtual QColor secondaryColor()=0;
        virtual QColor backgroundColor()=0;

        virtual QColor textColor()=0;
        virtual QColor primaryTextColor()=0;
        virtual QColor secondaryTextColor()=0;

        virtual QColor borderColor()=0;
        virtual QColor primaryBorderColor()=0;

        virtual QColor tabColor()=0;
        virtual QColor primaryTabColor()=0;
        virtual QColor secondaryTabColor()=0;

        virtual QColor clientTabColor()=0;
        virtual QColor primaryClientTabColor()=0;
        virtual QColor secondaryClientTabColor()=0;

        virtual QColor buttonColor()=0;
        virtual QColor primaryButtonColor()=0;
        virtual QColor secondaryButtonColor()=0;

        virtual QString* guides()=0;//guide diamond image path array
        virtual QString guide(int position,bool state)=0;

        virtual QString icon(int type,int state)=0;

        template<class T>
        static DockingTheme* init(){
            destory();//delete before
            instance = new T;
            return instance;
        }

        static DockingTheme* init(Style style);

        static void destory();
        static DockingTheme* getInstance();

    protected:
        DockingTheme();

    private:
        static DockingTheme* instance;
    };
}

#endif // DOCKING_THEME_H
