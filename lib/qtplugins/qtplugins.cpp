#include "lib/framework/frame.h"
#include "../../src/objmem.h"
#include "../../src/multiplay.h"
#include "qtplugins.h"
#include <QString>
#include <QDir>
#include <QPluginLoader>
#include "../../src/ai.h" // alliances definition

QtPluginsEngine *qtPlugins;

bool initPlugins()
{
    debug(LOG_SCRIPT, "initialize plugins");
    qtPlugins = new QtPluginsEngine();
    return true;
}

bool preparePlugins()
{
    return true;
}

bool shutdownPlugins()
{
    return true;
}

bool updatePlugins()
{
    return true;
}

QtPluginsEngine::QtPluginsEngine()
{
    loadPlugin();
    return;
}

QtPluginsEngine::~QtPluginsEngine()
{
    return;
}

bool QtPluginsEngine::loadPlugin()
{
    dbg("Trying load plugin");
    QDir pluginsDir("/home/scor/tmp/warzone-plugins/");
    pluginsDir.cd("plugins");
    const QString filter = "lib*.so";
    foreach (QString fileName, pluginsDir.entryList(QStringList() << filter, QDir::NoDotAndDotDot | QDir::Files)) {
        dbg(QString("fund plugin: %1").arg(pluginsDir.absoluteFilePath(fileName)));
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            dbg("cast plugin pointer to IPluginInterface...");
            iPluginInterface = qobject_cast<IPluginInterface *>(plugin);
            if (iPluginInterface){
                dbg("OK");
                iPluginInterface->setHostInterface(qobject_cast<IHostInterface *>(this));
                iPluginInterface->onLoad();
                return true;
            }else{
                dbg("FAILED");
            }
        }else{
            dbg("plugin instancing failed");
            dbg(QString("err: %1").arg(pluginLoader.errorString()));
        }
    }
    return false;
}

void QtPluginsEngine::dbg(QString msg)
{
    debug(LOG_SCRIPT, msg.toAscii());
    return;
}

void QtPluginsEngine::getPluginVersion()
{
    return;
}

QList<Unit> QtPluginsEngine::getUnits()
{
    QList<Unit> units;
    for (DROID *psDroid = apsDroidLists[0]; psDroid; psDroid = psDroid->psNext){
        Unit u;
        u.id = psDroid->id;
        u.x  = psDroid->pos.x;
        u.y  = psDroid->pos.y;
        u.z  = psDroid->pos.z;
        u.t  = fromObjectType(psDroid->type);
        u.health = psDroid->body;
        units.append(u);
    }
    return units;
}

QList<Unit> QtPluginsEngine::getFeatures()
{
    QList<Unit> units;
    for (FEATURE *f = apsFeatureLists[0]; f; f = f->psNext){
        units.append(toUnit(f));
    }
    return units;
}

UnitType QtPluginsEngine::fromObjectType(const OBJECT_TYPE &ot)
{
    return ST_HQ;
}

Unit QtPluginsEngine::toUnit(DROID *d)
{
    Unit u;
    return u;
}
Unit QtPluginsEngine::toUnit(FEATURE *f)
{
    Unit u;
    return u;
}
Unit QtPluginsEngine::toUnit(STRUCTURE *s)
{
    Unit u;
    return u;
}

int QtPluginsEngine::whoAmI()
{
    for (int i = 0; i < MAX_PLAYERS; i++){
        if (myResponsibility(i))
            return i;
    }
    return -1;
}

QList<Player> QtPluginsEngine::myAllies()
{
    QList<Player> res;
    int me = (int)whoAmI();
    for (int i = 0; i < MAX_PLAYERS; i++){
        if (alliances[i][me] == ALLIANCE_FORMED && me != i)
            res.append((Player)i);
    }
    return res;
}

bool QtPluginsEngine::isOur(const BASE_OBJECT *pObj)
{
    if (pObj->player == whoAmI() || myAllies().contains(pObj->player))
        return true;
    return false;
}

void QtPluginsEngine::evGameInit()
{
    dbg("event: game init.");
    dbg(QString("My player number is: %1").arg(whoAmI()));
    QList<Player> allies = myAllies();
    for (int i = 0; i < allies.length(); i++){
        dbg(QString("My allies %1: %2").arg(i).arg((int)allies.at(i)));
    }
}

// events
void QtPluginsEngine::triggerEvent(SCRIPT_TRIGGER_TYPE ev)
{
    QString evName;
    switch(ev){
    case TRIGGER_GAME_INIT:
        evName="gameInit";
        break;
    case TRIGGER_START_LEVEL:
        evName="gameStartLevel";
        break;
    case TRIGGER_TRANSPORTER_ARRIVED:
        evName="gameTransporterArrived";
        break;
    case TRIGGER_TRANSPORTER_LANDED:
        evName="gameTransporterLanded";
        break;
    case TRIGGER_TRANSPORTER_LAUNCH:
        evName="gameTransportelLaunch";
        break;
    case TRIGGER_TRANSPORTER_EXIT:
        evName="gameTransporterExit";
        break;
    case TRIGGER_TRANSPORTER_DONE:
        evName="gameTransportedDone";
        break;
    case TRIGGER_VIDEO_QUIT:
        evName="gameVideoQuit";
        break;
    case TRIGGER_MISSION_TIMEOUT:
        evName="gameMissionTimeout";
        break;
    case TRIGGER_GAME_LOADED:
        evName="gameGameLoaded";
        break;
    case TRIGGER_GAME_SAVING:
        evName="gameGameSaving";
        break;
    case TRIGGER_GAME_SAVED:
        evName="gameGameSaved";
        break;
    default:
        evName=QString("unknownEventId: %1").arg(ev);
    }

    dbg(QString("event triggered: %1").arg(evName));
    return;
}

// TODO: maybe remove this func. only one callback with DROID* param
void QtPluginsEngine::triggerEvent(SCRIPT_TRIGGER_TYPE ev, DROID *pDroid)
{
    dbg(QString("event triggered: %1, droidId: %2").arg(ev).arg(pDroid->id));
    return;
}

// must be our droid
void QtPluginsEngine::triggerEventDroidBuilt(DROID *psDroid, STRUCTURE *psFactory)
{
    if (isOur(psDroid))
        dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// victim or attacker must be our droid or struct
void QtPluginsEngine::triggerEventAttacked(BASE_OBJECT *psVictim, BASE_OBJECT *psAttacker, int lastHit)
{
    if (isOur(psVictim) || isOur(psVictim))
        dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// struct must be our
void QtPluginsEngine::triggerEventResearched(RESEARCH *psResearch, STRUCTURE *psStruct, int player)
{
    if (isOur(psStruct))
        dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// droid bust be our
void QtPluginsEngine::triggerEventStructBuilt(STRUCTURE *psStruct, DROID *psDroid)
{
    if (isOur(psDroid))
        dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// droid bust be our
void QtPluginsEngine::triggerEventDroidIdle(DROID *psDroid)
{
    if (isOur(psDroid))
        dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// victim must be our
void QtPluginsEngine::triggerEventDestroyed(BASE_OBJECT *psVictim)
{
    if (isOur(psVictim))
        dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// struct must be our
void QtPluginsEngine::triggerEventStructureReady(STRUCTURE *psStruct)
{
    if (isOur(psStruct))
        dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// viever must be our
void QtPluginsEngine::triggerEventSeen(BASE_OBJECT *psViewer, BASE_OBJECT *psSeen)
{
    if (isOur(psViewer))
        dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// TODO: who is who?:)
void QtPluginsEngine::triggerEventObjectTransfer(BASE_OBJECT *psObj, int from)
{
    //if (psObj->player != whoAmI()) return;
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// TODO: from or to must be self
void QtPluginsEngine::triggerEventChat(int from, int to, const char *message)
{
//    if (psDroid->player != whoAmI()) return;
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// TODO: from or to must be self

void QtPluginsEngine::triggerEventBeacon(int from, int to, const char *message, int x, int y)
{
//    if (psDroid->player != whoAmI()) return;
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// TODO: from or to must be self
void QtPluginsEngine::triggerEventBeaconRemoved(int from, int to)
{
//    if (psDroid->player != whoAmI()) return;
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// droid must be our
void QtPluginsEngine::triggerEventPickup(FEATURE *psFeat, DROID *psDroid)
{
    if (isOur(psDroid))
        dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventCheatMode(bool entered)
{
//    if (psDroid->player != whoAmI()) return;
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

/*
void QtPluginsEngine::triggerEventGroupLoss(BASE_OBJECT *psObj, int group, int size, QScriptEngine *engine)
{
    return;
}
*/

// droid must be our
void QtPluginsEngine::triggerEventDroidMoved(DROID *psDroid, int oldx, int oldy)
{
    if (isOur(psDroid))
        dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// TODO: maybe remove this event
void QtPluginsEngine::triggerEventArea(QString label, DROID *psDroid)
{
    //if (psDroid->player != whoAmI()) return;
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// TODO: maybe remove this event
void QtPluginsEngine::triggerEventSelected()
{
//    if (psDroid->player != whoAmI()) return;
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}


void QtPluginsEngine::triggerEventPlayerLeft(int id)
{
//    if (psDroid->player != whoAmI()) return;
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

// TODO: who is who?
void QtPluginsEngine::triggerEventDesignCreated(DROID_TEMPLATE *psTemplate)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}


// internal helpers
QList<Unit> QtPluginsEngine::ourUnits()
{
    QList<Unit> res;
    return res;
}

bool QtPluginsEngine::isVisibleBy(const BASE_OBJECT *pViewer, const BASE_OBJECT *pTarget)
{
    return true;
}

bool QtPluginsEngine::isVisible(const BASE_OBJECT *pTarget)
{
    for (int i = 0; i < ourUnits().length(); i++){
        // unit to BASE_OBJECT by id
        // if (isVisibleBy(unitObject, pTarget)) return true;
    }
    return false;
}
