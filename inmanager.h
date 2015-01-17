/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file         inmanager.h
  \date         January 2015
  \author       Nicu Tofan

*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef PILE_INTEGRATION_MANAGER_INCLUDE
#define PILE_INTEGRATION_MANAGER_INCLUDE

//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "integration_config.h"

#include <QList>
#include <QProcess>
#include <QDateTime>
#include <QFile>
#include <QDebug>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

namespace intest {
    class Integration;
    class Process;
}
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

enum Flags {
    NO_FLAG = 0x00000000,
    PRINT_STD_OUT = 0x00000001,
    PRINT_STD_ERR = 0x00000002,
    PRINT_STD_ALL = 0x00000004,
    PRINT_EXIT_CODE = 0x00000008,
    PRINT_EXIT_STAT = 0x00000010,
    PRINT_TIME = 0x00000020,
    PRINT_COMMAND_LINE = 0x00000040,
    PRINT_ERROR_CODE_LIST = 0x00000080,
    PRINT_STATUS_LIST = 0x00000100,

    ALL_FLAGS =
        PRINT_STD_OUT | PRINT_STD_ERR |
        PRINT_EXIT_CODE | PRINT_EXIT_STAT | PRINT_TIME |
        PRINT_COMMAND_LINE | PRINT_ERROR_CODE_LIST | PRINT_STATUS_LIST
};

class Manager {

public:

    //! Constructor.
    Manager();

    //! Destructor.
    ~Manager();

public:

    //! Start-up.
    static void
    init ();

    //! End-up.
    static void
    end ();

    //! Add an Integration instance.
    static void
    add (
            Integration * value);

    //! Execute all tests.
    static int
    execAll ();

    //! Print the text exactly as provided.
    static void
    print (
            const QString & s_value);

    //! Print the text exactly as provided and append a new line.
    static void
    printLn (
            const QString & s_value);

    //! A process is starting.
    static void
    processStarts (
            Process * proc);

    //! A process is ending.
    static void
    processEnds (
            Process * proc);


    //! Get the active flags.
    static int
    flags () {
        autocreate ();
        return singleton_->flags_;
    }

    //! Set active flags.
    static void
    setFlags (
            int value) {
        autocreate ();
        singleton_->flags_ = value;
    }

private:

    //! Start-up.
    static void
    autocreate();

private:
    QList<Integration*> list_;
    QFile * fstdout_;
    QDebug * d_;
    int flags_;

    static Manager * singleton_;

}; // class Manager

} // namespace intest

/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#endif /* PILE_INTEGRATION_MANAGER_INCLUDE */
