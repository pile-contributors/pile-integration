/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file         inmanager.cc
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

#include "inmanager.h"
#include "integration.h"
#include "inprocess.h"

#include <QCoreApplication>

#include <stdio.h>
#include <stdlib.h>

#ifdef PILE_INTEGRATION_WIN32
#include <windows.h>
#endif // PILE_INTEGRATION_WIN32

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

using namespace intest;

#ifdef PILE_INTEGRATION_WIN32

enum ConsoleColor {
    WINCC_BLACK = 0x0,
    WINCC_GRAY = 0x8,
    WINCC_BLUE = 0x1,
    WINCC_LIGHT_BLUE = 0x9,
    WINCC_GREEN = 0x2,
    WINCC_LIGHT_GREEN = 0xA,
    WINCC_AQUA = 0x3,
    WINCC_LIGHT_AQUA = 0xB,
    WINCC_RED = 0x4,
    WINCC_LIGHT_RED = 0xC,
    WINCC_PURPLE = 0x5,
    WINCC_LIGHT_PURPLE = 0xD,
    WINCC_YELLOW = 0x6,
    WINCC_LIGHT_YELLOW = 0xE,
    WINCC_WHITE = 0x7,
    WINCC_BRIGHT_WHITE = 0xF,
};

#define WINCC_COLOR(__back__, __foreg__) ((__back__ << 8) | __foreg__)

#else

#define PINT_COLOR_RED     "\x1b[31m"
#define PINT_COLOR_GREEN   "\x1b[32m"
#define PINT_COLOR_YELLOW  "\x1b[33m"
#define PINT_COLOR_BLUE    "\x1b[34m"
#define PINT_COLOR_MAGENTA "\x1b[35m"
#define PINT_COLOR_CYAN    "\x1b[36m"
#define PINT_COLOR_GREY    "\x1b[37m"
#define PINT_COLOR_RESET   "\x1b[0m"

#endif // PILE_INTEGRATION_WIN32


/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

Manager * Manager::singleton_ = NULL;

#ifdef PILE_INTEGRATION_WIN32

int color_mapping[] = {
    WINCC_COLOR(WINCC_BLACK, WINCC_RED),
    WINCC_COLOR(WINCC_BLACK, WINCC_GREEN),
    WINCC_COLOR(WINCC_BLACK, WINCC_YELLOW),
    WINCC_COLOR(WINCC_BLACK, WINCC_BLUE),
    WINCC_COLOR(WINCC_BLACK, WINCC_PURPLE),
    WINCC_COLOR(WINCC_BLACK, WINCC_LIGHT_AQUA),
    WINCC_COLOR(WINCC_BLACK, WINCC_WHITE),
    WINCC_COLOR(WINCC_BLACK, WINCC_BRIGHT_WHITE)
};

#else // PILE_INTEGRATION_WIN32

const char * color_mapping[] = {
    PINT_COLOR_RED,
    PINT_COLOR_GREEN,
    PINT_COLOR_YELLOW,
    PINT_COLOR_BLUE,
    PINT_COLOR_MAGENTA,
    PINT_COLOR_CYAN,
    PINT_COLOR_GREY,
    PINT_COLOR_RESET
};

#endif // PILE_INTEGRATION_WIN32


/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
Manager::Manager() :
    list_(),
    d_(new QTextStream(stdout)),
    flags_(PRINT_FAILED | PRINT_COLORS),
    sl_exclusion_(),
    sl_inclusion_()
{
    singleton_ = this;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
Manager::~Manager()
{
    d_->flush ();
    delete d_;
    singleton_ = NULL;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Manager::init ()
{
    autocreate ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Manager::end ()
{
    if (singleton_ != NULL) {
        delete singleton_;
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Manager::autocreate ()
{
    if (singleton_ == NULL) {
        new Manager ();
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Manager::add (Integration * value)
{
    autocreate ();
    singleton_->list_.append (value);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int Manager::execAll()
{
    autocreate ();
    endColor ();

    int i_max = singleton_->list_.count ();
    int i_complete_success = 0;
    int i_performed = 0;
    int i_success = 0;
    int i_failure = 0;

    for (int i = 0; i < i_max; ++i) {
        Integration * instance = singleton_->list_.at(i);

        // filter the tests based on user choice, if any
        QString instance_name = instance->name();
        if (singleton_->sl_exclusion_.contains (instance_name))
            continue;
        if (singleton_->sl_inclusion_.count () > 0) {
            if (!singleton_->sl_inclusion_.contains (instance_name))
                continue;
        }

        Manager::startColor (COLOR_CYAN);
        PRNT << "\nTest case " << i+1 << ": " << instance_name << "\n";
        Manager::endColor ();

        // execute this test case
        /*int result = */ instance->exec ();

        // print the results
        instance->printResults ();

        // update the total count
        i_success += instance->successful ();
        i_failure += instance->failed ();
        i_complete_success += instance->failed () == 0 ? 1 : 0;

        // and show nice separators
        if (Manager::hasFlag (PRINT_SEPARATORS)) {
            Manager::startColor (COLOR_GREY);
            PRNT << "[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]\n";
            PRNT << "================================================================================\n";
            Manager::endColor ();
        }

        ++i_performed;
    }

    // Print a global result of all the tests in this program.
    PRNT << "\n"
         << i_performed << " test cases with ";

    if (i_success > 0) Manager::startColor (COLOR_GREEN);
    PRNT << i_success   << " successful";
    if (i_success > 0) Manager::endColor ();

    PRNT << " and ";

    if (i_failure > 0) Manager::startColor (COLOR_RED);
    PRNT << i_failure   << " failed";
    if (i_failure > 0) Manager::endColor ();

    PRNT << " tests\n";

    return i_max - i_complete_success;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Manager::print (const QString & s_value)
{
    PRNT << s_value;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Manager::printLn (const QString & s_value)
{
    PRNT << s_value << "\n";
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Manager::processStarts (Process * proc)
{
    Manager::startColor (COLOR_CYAN);
    if ((singleton_->flags_ & PRINT_COMMAND_LINE) != 0) {
        PRNT << "    program: " << proc->program () << "\n";
        QStringList args = proc->arguments ();
        if (args.count () > 0) {
            bool b_first = true;
            PRNT << "    arguments: ";
            foreach(const QString & s, args) {
                if (b_first) {
                    PRNT << s;
                } else {
                    PRNT << ", " << s;
                }
                b_first = false;
            }
            PRNT << "\n";
        }
    }
    Manager::endColor ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Manager::processEnds (Process * proc)
{
    Manager::startColor (COLOR_GREY);
    if ((singleton_->flags_ & PRINT_STD_ALL) != 0) {
        if (proc->all_output_.isEmpty ()) {
            PRNT << "    (no output)\n";
        } else {
            PRNT << proc->all_output_ << "\n";
        }
    } else {
        if ((singleton_->flags_ & PRINT_STD_OUT) != 0) {
            if (proc->std_out_output_.isEmpty ()) {
                PRNT << "    (no output)\n";
            } else {
                PRNT << proc->std_out_output_ << "\n";
            }
        }
        if ((singleton_->flags_ & PRINT_STD_ERR) != 0) {
            if (proc->std_err_output_.isEmpty ()) {
                PRNT << "    (no error output)\n";
            } else {
                PRNT << proc->std_err_output_ << "\n";
            }
        }
    }
    if ((singleton_->flags_ & PRINT_EXIT_CODE) != 0) {
        PRNT << "    exit code: " << QString::number (proc->exitCode (), 16) << "\n";
    }
    if ((singleton_->flags_ & PRINT_EXIT_STAT) != 0) {
        if (proc->exitStatus () == QProcess::CrashExit) {
            PRNT << "    crashed exit status\n";
        } else {
            PRNT << "    normal exit status\n";
        }
    }
    if ((singleton_->flags_ & PRINT_TIME) != 0) {
        PRNT << "    run time: " << proc->runDuration ()<< "s\n";
    }
    if ((singleton_->flags_ & PRINT_ERROR_CODE_LIST) != 0) {
        if (proc->errors_.count () == 0) {
            PRNT << "    (no error codes)\n";
        } else {
            PRNT << "    error codes: ";
            bool b_first = true;
            foreach (QProcess::ProcessError e, proc->errors_) {
                QString s_err_name;
                switch (e) {
                case QProcess::FailedToStart: s_err_name = "Failed To Start"; break;
                case QProcess::Crashed: s_err_name = "Crashed"; break;
                case QProcess::Timedout: s_err_name = "Timedout"; break;
                case QProcess::ReadError: s_err_name = "ReadError"; break;
                case QProcess::WriteError: s_err_name = "WriteError"; break;
                case QProcess::UnknownError: s_err_name = "Unknown Error"; break;
                }
                if (b_first) {
                    PRNT << s_err_name;
                } else {
                    PRNT << ", " << s_err_name;
                }
                b_first = false;
            }
            PRNT << "\n";
        }
    }
    if ((singleton_->flags_ & PRINT_STATUS_LIST) != 0) {
        if (proc->states_.count () == 0) {
            PRNT << "    (no status codes)\n";
        } else {
            PRNT << "    state list: ";
            bool b_first = true;
            foreach (QProcess::ProcessState e, proc->states_) {
                QString s_stat_name;
                switch (e) {
                case QProcess::NotRunning: s_stat_name = "Not Running"; break;
                case QProcess::Starting: s_stat_name = "Starting"; break;
                case QProcess::Running: s_stat_name = "Running"; break;
                }
                if (b_first) {
                    PRNT << s_stat_name;
                } else {
                    PRNT << ", " << s_stat_name;
                }
                b_first = false;
            }
            PRNT << "\n";
        }
    }
    Manager::endColor ();
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Manager::startColor (intest::Color color)
{
    if (hasFlag (PRINT_COLORS)) {
        singleton_->d_->flush ();
        delete singleton_->d_;
        singleton_->d_ = new QTextStream (stdout);
#ifdef PILE_INTEGRATION_WIN32
        HANDLE  hConsole;
        hConsole = GetStdHandle (STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute (hConsole, color_mapping[color]);
        hConsole = GetStdHandle (STD_ERROR_HANDLE);
        SetConsoleTextAttribute (hConsole, color_mapping[color]);
#else // PILE_INTEGRATION_WIN32
        PRNT << color_mapping[color];
#endif // PILE_INTEGRATION_WIN32
    }
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Manager::endColor ()
{
    if (hasFlag (PRINT_COLORS)) {
        singleton_->d_->flush ();
        delete singleton_->d_;
        singleton_->d_ = new QTextStream(stdout);
#ifdef PILE_INTEGRATION_WIN32
        HANDLE  hConsole;
        hConsole = GetStdHandle (STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute (hConsole, color_mapping[COLOR_RESET]);
        hConsole = GetStdHandle (STD_ERROR_HANDLE);
        SetConsoleTextAttribute (hConsole, color_mapping[COLOR_RESET]);
#else // PILE_INTEGRATION_WIN32
        PRNT << COLOR_RESET;
#endif // PILE_INTEGRATION_WIN32
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
