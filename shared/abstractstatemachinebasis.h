#ifndef ABSTRACTSTATEMACHINEBASIS_H
#define ABSTRACTSTATEMACHINEBASIS_H

#include <QCoreApplication>
#include <QObject>
#include <QVariant>
#include <QState>
#include <QStateMachine>
#include <QMap>
#include <QList>
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
    GlobalSignal currentGlobalSignal;
    QString currentStateName;
    QString previousStateName;
    bool isInitiated = false;

    void pushAGlobalSignalIntoPrioritizedBuffer(const GlobalSignal &aGlobalSignal);
    void deleteEmptyListsFromPrioritizedBuffer();
    void clearPrioritizedBuffer();
    GlobalSignal popMostPrioritizedGlobalSignalOutOfPrioritizedBuffer();
signals:
    void Out(const GlobalSignal &);
    void directTransitionRequested(const QString &);
    void ErrorOccurred();
    void InitiationRequested();
    void goIdle();
    void GlobalSignalExecutionRequested();
};

#endif // ABSTRACTSTATEMACHINEBASIS_H
