#ifndef QTPLUGINSENGINE_H
#define QTPLUGINSENGINE_H

#include "qtpluginsinterface.h"
#include <QList>
#include <QString>

class QPlugin : public QObject, public IHostInterface
{
    Q_OBJECT
    Q_INTERFACES(IHostInterface)

public:
    QPlugin();
    void debug(QString msg);
    bool actionBuild(UnitId, int, int, int, UnitType) {return true;}
    bool actionAttack(UnitId, UnitId) {return true;}
    bool actionStop(UnitId) {return true;}

    QList<Unit> getUnits();
    ~QPlugin();

private:
    IPluginInterface *plugin;
    bool loadPlugin();

    QList<Unit> units;

public slots:
    void getPluginVersion();
};

#endif // QTPLUGINSENGINE_H
