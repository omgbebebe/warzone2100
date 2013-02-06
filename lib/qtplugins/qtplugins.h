#ifndef QTPLUGINS_H
#define QTPLUGINS_H

//#include "qtpluginsinterface.h"
#include "qtpluginsunitsdefs.h"
#include <QObject>
#include <QList>
#include <QString>
#include "../../src/objmem.h"
#include "../../src/qtscript.h"
#include "../../src/actiondef.h" // actions enums definitions

class QtPluginsEngine;
extern QtPluginsEngine* qtPlugins;

bool initPlugins();
bool preparePlugins();
bool shutdownPlugins();
bool updatePlugins();

typedef int Player;

class IHostInterface
{
public:
    virtual ~IHostInterface() {}
    virtual void dbg(QString msg) = 0;

    // game info
    virtual Player whoAmI() = 0;
    virtual QList<Player> myAllies() = 0;
    virtual QList<Unit> getUnits() = 0; // return all visible units
    // commands
    //virtual bool startBuild(UnitId droidId, StructureType structT, Position pos, bool exactPosition) = 0;
    //virtual bool helpBuild(UnitId droidId, UnitId structId) = 0;
    //virtual bool demolish(StructureId structId) = 0;

    //virtual bool action(DROID_ACTION action) = 0;
    virtual bool actionBuild(UnitId unitId, int x, int y, int z, UnitType stType) = 0;
    virtual bool actionAttack(UnitId unitId, UnitId victimId) = 0;
    virtual bool actionStop(UnitId unitId) = 0;
/*
    //events
    virtual void triggerEvent(SCRIPT_TRIGGER_TYPE ev) = 0;
    virtual void triggerEvent(SCRIPT_TRIGGER_TYPE ev, DROID *pDroid) = 0;
    virtual void triggerEventDroidBuilt(DROID *psDroid, STRUCTURE *psFactory) = 0;
    virtual void triggerEventAttacked(BASE_OBJECT *psVictim, BASE_OBJECT *psAttacker, int lastHit) = 0;
    virtual void triggerEventResearched(RESEARCH *psResearch, STRUCTURE *psStruct, int player) = 0;
    virtual void triggerEventStructBuilt(STRUCTURE *psStruct, DROID *psDroid) = 0;
    virtual void triggerEventDroidIdle(DROID *psDroid) = 0;
    virtual void triggerEventDestroyed(BASE_OBJECT *psVictim) = 0;
    virtual void triggerEventStructureReady(STRUCTURE *psStruct) = 0;
    virtual void triggerEventSeen(BASE_OBJECT *psViewer, BASE_OBJECT *psSeen) = 0;
    virtual void triggerEventObjectTransfer(BASE_OBJECT *psObj, int from) = 0;
    virtual void triggerEventChat(int from, int to, const char *message) = 0;
    virtual void triggerEventBeacon(int from, int to, const char *message, int x, int y) = 0;
    virtual void triggerEventBeaconRemoved(int from, int to) = 0;
    virtual void triggerEventPickup(FEATURE *psFeat, DROID *psDroid) = 0;
    virtual void triggerEventCheatMode(bool entered) = 0;
    //virtual void triggerEventGroupLoss(BASE_OBJECT *psObj, int group, int size, QScriptEngine *engine) = 0;
    virtual void triggerEventDroidMoved(DROID *psDroid, int oldx, int oldy) = 0;
    virtual void triggerEventArea(QString label, DROID *psDroid) = 0;
    virtual void triggerEventSelected() = 0;
    virtual void triggerEventPlayerLeft(int id) = 0;
    virtual void triggerEventDesignCreated(DROID_TEMPLATE *psTemplate) = 0;
*/
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

    // game info
    Player whoAmI();
    QList<Player> myAllies();

    QList<Unit> getUnits();
//    ~QtPluginsEngine();
    //events
    void evGameInit();
    void triggerEvent(SCRIPT_TRIGGER_TYPE ev);
    void triggerEvent(SCRIPT_TRIGGER_TYPE ev, DROID *pDroid);
    void triggerEventDroidBuilt(DROID *psDroid, STRUCTURE *psFactory);
    void triggerEventAttacked(BASE_OBJECT *psVictim, BASE_OBJECT *psAttacker, int lastHit);
    void triggerEventResearched(RESEARCH *psResearch, STRUCTURE *psStruct, int player);
    void triggerEventStructBuilt(STRUCTURE *psStruct, DROID *psDroid);
    void triggerEventDroidIdle(DROID *psDroid);
    void triggerEventDestroyed(BASE_OBJECT *psVictim);
    void triggerEventStructureReady(STRUCTURE *psStruct);
    void triggerEventSeen(BASE_OBJECT *psViewer, BASE_OBJECT *psSeen);
    void triggerEventObjectTransfer(BASE_OBJECT *psObj, int from);
    void triggerEventChat(int from, int to, const char *message);
    void triggerEventBeacon(int from, int to, const char *message, int x, int y);
    void triggerEventBeaconRemoved(int from, int to);
    void triggerEventPickup(FEATURE *psFeat, DROID *psDroid);
    void triggerEventCheatMode(bool entered);
    //void triggerEventGroupLoss(BASE_OBJECT *psObj, int group, int size, QScriptEngine *engine);
    void triggerEventDroidMoved(DROID *psDroid, int oldx, int oldy);
    void triggerEventArea(QString label, DROID *psDroid);
    void triggerEventSelected();
    void triggerEventPlayerLeft(int id);
    void triggerEventDesignCreated(DROID_TEMPLATE *psTemplate);

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

    bool isVisibleBy(const BASE_OBJECT *pViewer, const BASE_OBJECT *pTarget);
    bool isVisible(const BASE_OBJECT *pTarget);

    //internal helpers
    QList<Unit> ourUnits();
    bool isOur(const BASE_OBJECT *pObj);
public slots:
    void getPluginVersion();
};

#endif // QTPLUGINS_H
