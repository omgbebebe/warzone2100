#ifndef QTPLUGINS_H
#define QTPLUGINS_H

//#include "qtpluginsinterface.h"
#include "qtpluginsunitsdefs.h"
#include <QObject>
#include <QList>
#include <QString>
#include "../../src/objmem.h"

bool initPlugins();
bool preparePlugins();
bool shutdownPlugins();
bool updatePlugins();

class IHostInterface
{
public:
    virtual ~IHostInterface() {}
    virtual void dbg(QString msg) = 0;

    virtual QList<Unit> getUnits() = 0; // return all visible units
    // commands
    virtual bool actionBuild(UnitId unitId, int x, int y, int z, UnitType stType) = 0;
    virtual bool actionAttack(UnitId unitId, UnitId victimId) = 0;
    virtual bool actionStop(UnitId unitId) = 0;
};

Q_DECLARE_INTERFACE(IHostInterface, "net.wz2100.Plugin.IHostInterface/0.0.1")


class IPluginInterface
{
public:
    virtual ~IPluginInterface() {}
    virtual QString version() = 0;
    virtual void setHostInterface(IHostInterface*) = 0;

    // events callback
    virtual void onLoad() = 0;

    virtual void onStart() = 0;
    virtual void onEnd(bool isWinner) = 0; // i'm win?
    virtual void onFrame() = 0;

    virtual void onUnitAttacked(UnitId) = 0; // check pUnit->lastAttacked for found aggressor
    virtual void onUnitDiscover(UnitId) = 0;
    virtual void onUnitEvade(UnitId) = 0;
    virtual void onUnitDestroy(UnitId) = 0;

    virtual void onResearchComplete(UnitId) = 0; // research facility id
    virtual void onProductionComplete(UnitId) = 0; // research facility id

};

Q_DECLARE_INTERFACE(IPluginInterface, "net.wz2100.Plugins.IPluginInterface/0.0.1")


class QtPluginsEngine : public QObject, public IHostInterface
{
    Q_OBJECT
    Q_INTERFACES(IHostInterface)

public:
    QtPluginsEngine();
    ~QtPluginsEngine();
    void dbg(QString msg);
    bool actionBuild(UnitId, int, int, int, UnitType) {return true;}
    bool actionAttack(UnitId, UnitId) {return true;}
    bool actionStop(UnitId) {return true;}

    QList<Unit> getUnits();
//    ~QtPluginsEngine();

private:
    IPluginInterface *iPluginInterface;
    bool loadPlugin();
    int myPlayer;

    UnitType fromObjectType(const OBJECT_TYPE&);
    QList<Unit> units;

    QList<Unit> getFeatures();
    // converters
    Unit toUnit(FEATURE*);
    Unit toUnit(DROID*);
    Unit toUnit(STRUCTURE*);

    int whoAmI();
public slots:
    void getPluginVersion();
};

#endif // QTPLUGINS_H
