#include "commonthings.h"

const TypeOfGlobalSignalContentPriority maxGlobalSignalContentPriority = std::numeric_limits<TypeOfGlobalSignalContentPriority>::max();
const TypeOfGlobalSignalContentPriority minGlobalSignalContentPriority = std::numeric_limits<TypeOfGlobalSignalContentPriority>::min();
const TypeOfGlobalSignalPriority maxGlobalSignalPriority = std::numeric_limits<TypeOfGlobalSignalPriority>::max();
const TypeOfGlobalSignalPriority minGlobalSignalPriority = std::numeric_limits<TypeOfGlobalSignalPriority>::min();

const Qt::ConnectionType uniqueQtConnectionType = static_cast<Qt::ConnectionType>(Qt::AutoConnection | Qt::UniqueConnection);

const QString piLocalDBWorkerObjName = QStringLiteral("piLocalDBWorker");
const QString UHV2WorkerObjName = QStringLiteral("UHV2Worker");
const QString UHV4WorkerObjName = QStringLiteral("UHV4Worker");
const QString UHV2PVICollectorObjName = QStringLiteral("UHV2PVICollector");
const QString UHV4PVICollectorObjName = QStringLiteral("UHV4PVICollector");
const QString SmallCoordinatorObjName = QStringLiteral("SmallCoordinator");
