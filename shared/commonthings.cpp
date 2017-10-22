#include "commonthings.h"

const TypeOfGlobalSignalContentPriority maxGlobalSignalContentPriority = std::numeric_limits<TypeOfGlobalSignalContentPriority>::max();
const TypeOfGlobalSignalContentPriority minGlobalSignalContentPriority = std::numeric_limits<TypeOfGlobalSignalContentPriority>::min();
const TypeOfGlobalSignalPriority maxGlobalSignalPriority = std::numeric_limits<TypeOfGlobalSignalPriority>::max();
const TypeOfGlobalSignalPriority minGlobalSignalPriority = std::numeric_limits<TypeOfGlobalSignalPriority>::min();

const Qt::ConnectionType uniqueQtConnectionType = static_cast<Qt::ConnectionType>(Qt::AutoConnection | Qt::UniqueConnection);

const QString piLocalDBWorkerObjName = QStringLiteral("piLocalDBWorker");
const QString UHV2SerialPortWorkerObjName = QStringLiteral("UHV2SerialPortWorker");
const QString UHV4SerialPortWorkerObjName = QStringLiteral("UHV4SerialPortWorker");
const QString UHV2SerialPortDataProcessorObjName = QStringLiteral("UHV2SerialPortDataProcessor");
const QString UHV4SerialPortDataProcessorObjName = QStringLiteral("UHV4SerialPortDataProcessor");
const QString CanBusWorkerObjName = QStringLiteral("CanBusWorker");
const QString CanDataProcessorObjName = QStringLiteral("CanDataProcessor");
const QString GlobalSignalCoordinatorObjName = QStringLiteral("GlobalSignalCoordinator");
