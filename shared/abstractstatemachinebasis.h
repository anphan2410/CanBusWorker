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
    bool isCurrentRunningCycleCompleted = false;

    void addAGlobalSignal(const GlobalSignal &aGlobalSignal);
    void deleteEmptyListsFromPrioritizedBuffer();
    void clearPrioritizedBuffer();
signals:
    void Out(const GlobalSignal &);
    void requestDirectTransition(const QString &);
    void ErrorOccurred();
    void goToState0();//uninitiated
    void goToState1();//idle
    void goToState2();//running
};

#endif // ABSTRACTSTATEMACHINEBASIS_H
