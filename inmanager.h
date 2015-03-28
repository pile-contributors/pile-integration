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
#include "pile_color.h"

#include <QList>
#include <QProcess>
#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QTextStream>

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
    NO_FLAG             = 0x00000000,

    PRINT_STD_OUT       = 0x00000001,
    PRINT_STD_ERR       = 0x00000002,
    PRINT_STD_ALL       = 0x00000004,
    PRINT_EXIT_CODE     = 0x00000008,
    PRINT_EXIT_STAT     = 0x00000010,
    PRINT_TIME          = 0x00000020,
    PRINT_COMMAND_LINE  = 0x00000040,
    PRINT_ERROR_CODE_LIST = 0x00000080,
    PRINT_STATUS_LIST   = 0x00000100,

    PRINT_SUCCESSFUL    = 0x00000200,
    PRINT_FAILED        = 0x00000400,

    PRINT_COLORS        = 0x00000800,
    PRINT_SEPARATORS    = 0x00000800,

    ALL_FLAGS =
        PRINT_STD_OUT | PRINT_STD_ERR |
        PRINT_EXIT_CODE | PRINT_EXIT_STAT | PRINT_TIME |
        PRINT_COMMAND_LINE | PRINT_ERROR_CODE_LIST | PRINT_STATUS_LIST |
        PRINT_SUCCESSFUL | PRINT_FAILED | PRINT_COLORS
};

class Manager {

public:

    //! Constructor.
    Manager();

    //! Destructor.
    ~Manager();

public:

    //! The singleton.
    static Manager *
    singleton () {
        return singleton_;
    }

    //! Start-up.
    static void
    init ();

    //! End-up.
    static void
    end ();

    //! The printer.
    static QTextStream &
    d () {
        return *(singleton_->d_);
    }

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

    //! Tell if a flag is set.
    static bool
    hasFlag (
            int flag_value) {
        autocreate ();
        return (singleton_->flags_ & flag_value) == flag_value;
    }

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


    //! Add a test case to the exclusion list.
    static void
    excludeTest (
            const QString & s_value) {
        singleton_->sl_exclusion_.append (s_value);
    }

    //! Add a specific test case to be executes.
    static void
    addSpecificTest (
            const QString & s_value) {
        singleton_->sl_inclusion_.append (s_value);
    }

    //! Starts printing with a color.
    static void
    startColor (
            intest::Color color);

    //! Resets printing with color.
    static void
    endColor ();

private:

    //! Start-up.
    static void
    autocreate();

private:
    QList<Integration*> list_;
    QTextStream * d_;
    int flags_;

    QStringList sl_exclusion_;
    QStringList sl_inclusion_;

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
