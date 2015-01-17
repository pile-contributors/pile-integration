/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file         inprocess.h
  \date         January 2015
  \author       Nicu Tofan

*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef PROWIZ_INTEGRATION_PROCESS_INCLUDE
#define PROWIZ_INTEGRATION_PROCESS_INCLUDE

//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "integration_config.h"

#include <QList>
#include <QProcess>
#include <QDateTime>
#include <QEventLoop>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */


/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

/*  DATA    ================================================================ */
//
//
//
//
/*  CLASS    --------------------------------------------------------------- */

namespace intest {

class Process : public QProcess {
    Q_OBJECT
public:

    Process();

    virtual ~Process();

    qint64
    runDuration() const {
        return end_time_.secsTo (start_time_);
    }

    void perform (
            const QStringList & input);

private slots:
    void errorSlot (QProcess::ProcessError error);
    void finishedSlot (int exitCode, QProcess::ExitStatus exitStatus);
    void readyReadStandardErrorSlot ();
    void readyReadStandardOutputSlot ();
    void startedSlot ();
    void stateChangedSlot (QProcess::ProcessState newState);

public:
    QDateTime start_time_;
    QDateTime end_time_;
    QString std_err_output_;
    QString std_out_output_;
    QString all_output_;
    bool b_started_;
    QList<QProcess::ProcessError> errors_;
    QList<QProcess::ProcessState> states_;
private:
    QEventLoop ev_loop_;
}; // class Process

} // namespace intest

/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#endif /* PROWIZ_INTEGRATION_PROCESS_INCLUDE */
