#include "lib/framework/frame.h"
#include "../../src/objmem.h"
#include "../../src/multiplay.h"
#include "qtplugins.h"
#include <QString>
#include <QDir>
#include <QPluginLoader>

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

void QtPluginsEngine::evGameInit()
{
    dbg("event: game init.");
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

void QtPluginsEngine::triggerEvent(SCRIPT_TRIGGER_TYPE ev, DROID *pDroid)
{
    dbg(QString("event triggered: %1, droidId: %2").arg(ev).arg(pDroid->id));
    return;
}

void QtPluginsEngine::triggerEventDroidBuilt(DROID *psDroid, STRUCTURE *psFactory)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventAttacked(BASE_OBJECT *psVictim, BASE_OBJECT *psAttacker, int lastHit)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventResearched(RESEARCH *psResearch, STRUCTURE *psStruct, int player)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventStructBuilt(STRUCTURE *psStruct, DROID *psDroid)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventDroidIdle(DROID *psDroid)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventDestroyed(BASE_OBJECT *psVictim)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventStructureReady(STRUCTURE *psStruct)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventSeen(BASE_OBJECT *psViewer, BASE_OBJECT *psSeen)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventObjectTransfer(BASE_OBJECT *psObj, int from)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventChat(int from, int to, const char *message)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventBeacon(int from, int to, const char *message, int x, int y)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventBeaconRemoved(int from, int to)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventPickup(FEATURE *psFeat, DROID *psDroid)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventCheatMode(bool entered)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

/*
void QtPluginsEngine::triggerEventGroupLoss(BASE_OBJECT *psObj, int group, int size, QScriptEngine *engine)
{
    return;
}
*/

void QtPluginsEngine::triggerEventDroidMoved(DROID *psDroid, int oldx, int oldy)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventArea(QString label, DROID *psDroid)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventSelected()
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventPlayerLeft(int id)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}

void QtPluginsEngine::triggerEventDesignCreated(DROID_TEMPLATE *psTemplate)
{
    dbg(QString("complex event: %1").arg(Q_FUNC_INFO));
    return;
}
