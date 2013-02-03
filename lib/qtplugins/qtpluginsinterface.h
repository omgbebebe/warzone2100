#ifndef qtpluginsinterface_H
#define qtpluginsinterface_H

#include <QObject>
#include <QString>
#include "qtpluginsunitsdefs.h"

class IHostInterface
{
public:
    virtual ~IHostInterface();
//    virtual void debug(QString msg) = 0;

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


#endif
