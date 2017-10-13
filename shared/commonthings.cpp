#include "commonthings.h"

const Qt::ConnectionType uniqueQtConnectionType = static_cast<Qt::ConnectionType>(Qt::AutoConnection | Qt::UniqueConnection);

const QString piLocalDBWorkerObjName = QStringLiteral("pilambdaReturnWorker");
const QString UHV2WorkerObjName = QStringLiteral("UHV2Worker");
const QString UHV4WorkerObjName = QStringLiteral("UHV4Worker");
const QString UHV2PVICollectorObjName = QStringLiteral("UHV2PVICollector");
const QString UHV4PVICollectorObjName = QStringLiteral("UHV4PVICollector");
const QString SmallCoordinatorObjName = QStringLiteral("SmallCoordinator");
