#include <QtCore>
#include "lib/framework/frame.h"
#include "lib/framework/debug.h"
#include "featuredef.h"
#include "objmem.h"
#include "qtpluginsinterface.h"

QtPluginsInterface *qtPluginsInterface;

bool loadPlugin()
{
  QDir pluginsDir("/home/scor/tmp/warzone/");
  pluginsDir.cd("plugins");
  foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
    QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
    QObject *plugin = pluginLoader.instance();
    if (plugin) {
      qtPluginsInterface = qobject_cast<QtPluginsInterface *>(plugin);
      if (qtPluginsInterface)
	return true;
    }
  }
  return false;
}


QList <FEATURE *> getFeatures()
{
  QList <FEATURE *> fs;
  for (FEATURE *psFeat = apsFeatureLists[0]; psFeat; psFeat = psFeat->psNext){
    fs.push_back(psFeat);
  }
  return fs;
}

bool initPlugins()
{
  debug(LOG_SCRIPT, "*** Called init plugins!");
  return true;
}

bool shutdownPlugins()
{
  debug(LOG_SCRIPT, "*** Called shutdown plugins!");
  return true;
}

bool preparePlugins()
{
  debug(LOG_SCRIPT, "*** Called prepare plugins!");
  debug(LOG_SCRIPT, "*** Try load plugins!");
  if (!loadPlugin()) {
    debug(LOG_SCRIPT, "*** Loading plugins failed!");
  }else{
    debug(LOG_SCRIPT, "*** Loading plugins success!");
    debug(LOG_SCRIPT, "*** Loaded plugin version %s", qtPluginsInterface->version().toUtf8().constData());
  }
  return true;
}

bool updatePlugins()
{
  // get all features
  QList <FEATURE *> fs = getFeatures();
  if (!fs.isEmpty()){
    for (int i = 0; i < fs.size(); i++){
      //FEATURE *pf = fs.at(i);
      //debug(LOG_SCRIPT, "*** Feature id %d: found", pf->id);
    }
  }
  //debug(LOG_SCRIPT, "*** Called update plugins!");
  return true;
}

