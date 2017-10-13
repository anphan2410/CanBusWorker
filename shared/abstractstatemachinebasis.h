#ifndef ABSTRACTSTATEMACHINEBASIS_H
#define ABSTRACTSTATEMACHINEBASIS_H

#include <QCoreApplication>
#include <QObject>
#include <QVariant>
#include <QState>
#include <QStateMachine>
#include <QMap>
#include <QMetaType>
#include <QMetaEnum>
#include "commonthings.h"
#include "anLogger/src/anlogger.h"

class AbstractStateMachineBasis : public QObject
{
    Q_OBJECT
public:
    explicit AbstractStateMachineBasis(QObject *parent = nullptr);

    QMap<qint16,QList<GlobalSignal>> prioritizedBuffer;
    QString previousStateName;

    void addAGlobalSignal(const GlobalSignal &aGlobalSignal);
    void deleteEmptyListsFromPrioritizedBuffer();
    void clearPrioritizedBuffer();
signals:
    void Out(const GlobalSignal &);
    void isInitialized();
    void requestDirectTransition(const QString &);
    void prioritizedBufferRefilled();
    void aGlobalSignalAdded();
    void ErrorOccurred();
};

#endif // ABSTRACTSTATEMACHINEBASIS_H
