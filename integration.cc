/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file         integration.cc
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
Integration::Integration() :
    successful_tests_ (0),
    failed_tests_ (0)
{
    Manager::add (this);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
Integration::~Integration()
{
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
Process * Integration::runMonitoredProcess (
        const QString & s_program_path,
        const QStringList & arg_list,
        const QStringList & input,
        const QString & s_working_path)
{
    intest::Process * retp = new intest::Process ();
    retp->setProgram (s_program_path);
    retp->setArguments (arg_list);
    if (!s_working_path.isEmpty ()) {
        retp->setWorkingDirectory (s_working_path);
    }

    // run the program
    retp->perform (input);

    return retp;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
bool Integration::evaluateBoolean(
        bool condition, bool /*fatal*/, const char * s_condition,
        const char *file, int line)
{
    QString s_sondition_s (s_condition);

    // we do this in order to present a consistent view to the user
    s_sondition_s.replace (STRINGIFY(PROG_STD_ERR), "PROG_STD_ERR");
    s_sondition_s.replace (STRINGIFY(PROG_STD_OUT), "PROG_STD_OUT");
    s_sondition_s.replace (STRINGIFY(PROG_OUT), "PROG_OUT");
    s_sondition_s.replace (STRINGIFY(DURATION), "DURATION");
    s_sondition_s.replace (STRINGIFY(MILI_DURATION), "MILI_DURATION");
    s_sondition_s.replace (STRINGIFY(EXIT_CODE), "EXIT_CODE");
    s_sondition_s.replace (STRINGIFY(EXIT_STATUS), "EXIT_STATUS");

    if (condition) {
        ++successful_tests_;

        if (intest::Manager::hasFlag (PRINT_SUCCESSFUL)) {
            PRNT << file
                 << "(" << line << ") : succes\n    "
                 << s_sondition_s << "\n";
        }

    } else {
        ++failed_tests_;

        if (intest::Manager::hasFlag (PRINT_FAILED)) {
            PRNT << file
                 << "(" << line << ") : failure\n    "
                 << s_sondition_s << "\n";
        }
    }

    return condition;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Integration::printResults() const
{
    if (successful() == 0) {
        if (failed() == 0) {
            PRNT << "     no tests\n";
        } else {
            PRNT << "     "
                 << failed()
                 << " failed\n";
        }
    } else if (failed() == 0) {
        PRNT << "     "
             << successful()
             << " successful\n";
    } else {
        PRNT << "     "
             << successful()
             << " succesful "
             << failed()
             << " failed\n";
    }
}
/* ========================================================================= */

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
