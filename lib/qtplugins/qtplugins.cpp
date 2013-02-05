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

QtPluginsEngine::~QtPluginsEngine()
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
void QtPluginsEngine::gameEvent(SCRIPT_TRIGGER_TYPE ev)
{
    dbg(QString("event triggered: %1").arg(ev));
    return;
}
void QtPluginsEngine::gameEvent(SCRIPT_TRIGGER_TYPE ev, DROID *pDroid)
{
    dbg(QString("event triggered: %1, droidId: %2").arg(ev).arg(pDroid->id));
    return;
}
