/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file         inprocess.cc
  \date         January 2015
  \author       Nicu Tofan

*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "integration.h"
#include "inprocess.h"
#include "inmanager.h"

#include <QCoreApplication>
#include <QProcess>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

using namespace intest;

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
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
Process::Process() :
    QProcess(),
    start_time_(QDateTime::currentDateTime ()),
    end_time_(QDateTime::currentDateTime ()),
    std_err_output_(),
    std_out_output_(),
    all_output_(),
    b_started_(false),
    errors_(),
    states_(),
    ev_loop_()
{
    connect (this,
             SIGNAL(error(QProcess::ProcessError)),
             SLOT(errorSlot(QProcess::ProcessError)));
    connect (this,
             SIGNAL(finished(int,QProcess::ExitStatus)),
             SLOT(finishedSlot(int,QProcess::ExitStatus)));
    connect (this,
             SIGNAL(readyReadStandardError()),
             SLOT(readyReadStandardErrorSlot()));
    connect (this,
             SIGNAL(readyReadStandardOutput()),
             SLOT(readyReadStandardOutputSlot()));
    connect (this,
             SIGNAL(started()),
             SLOT(startedSlot()));
    connect (this,
             SIGNAL(stateChanged (QProcess::ProcessState)),
             SLOT(stateChangedSlot (QProcess::ProcessState)));
    connect (this,
             SIGNAL(finished(int,QProcess::ExitStatus)),
             &ev_loop_,
             SLOT(quit()));
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
Process::~Process()
{
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Process::errorSlot (QProcess::ProcessError error)
{
    errors_.append (error);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Process::finishedSlot (int, QProcess::ExitStatus)
{
    // no need to cache them as they are available from QProcess
    end_time_ = QDateTime::currentDateTime ();
    Manager::processEnds (this);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Process::readyReadStandardErrorSlot ()
{
    QString s (readAllStandardError ());
    std_err_output_.append (s);
    all_output_.append (s);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Process::readyReadStandardOutputSlot ()
{
    QString s (readAllStandardOutput());
    std_out_output_.append (s);
    all_output_.append (s);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Process::startedSlot ()
{
    b_started_ = true;
    start_time_ = QDateTime::currentDateTime ();
    Manager::processStarts (this);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Process::stateChangedSlot (QProcess::ProcessState newState)
{
    states_.append (newState);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Process::perform (const QStringList & input)
{

    // start the program
    this->start (QIODevice::ReadWrite);
    if (!this->waitForStarted()) {
        return;
    }

    // provide the input
    foreach (const QString & s, input) {
        this->write (s.toLatin1 ().constData ());
    }
    this->closeWriteChannel();

    // allow it to run
    // this->waitForFinished();
    ev_loop_.exec ();
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
