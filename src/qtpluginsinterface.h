#ifndef qtpluginsinterface_H
#define qtpluginsinterface_H

#include <QString>

class QtPluginsInterface
{
public:
    virtual ~QtPluginsInterface() {}
    virtual QString version() = 0;
    virtual QBool cmdIn() = 0;
    virtual QBool dataOut() = 0;
};

Q_DECLARE_INTERFACE(QtPluginsInterface,
                    "net.wz2100.Plugin.QtPluginsInterface/0.0.1")

#endif
