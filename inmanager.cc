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

Manager * Manager::singleton_ = NULL;

static QString s_print_option ("--print:");
static QString s_hide_option  ("--hide:");
static QString s_exclude_option ("--exclude:");
static QString s_only_option  ("--only:");

static bool print_test_separators = false;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
void terminate_with_error (int error_code)
{
    Manager::end ();
    exit (error_code);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void prcess_print_arguments (const QString & arg)
{
    // example: --print:std_err
    // example: --print:separators
    // example: --print:command_line,error_code_list,exit_code,exit_stat,status_list,std_all,std_err,std_out,time
    int print_flags = intest::Manager::flags ();
    QStringList sl_print =
            arg.mid (s_print_option.length ())
               .toLower ().split (",");
    foreach(const QString & s_print_arg, sl_print) {
        if (s_print_arg == "all") {
            print_flags = intest::ALL_FLAGS;
        } else if (s_print_arg == "std_out") {
            print_flags = print_flags | intest::PRINT_STD_OUT;
        } else if (s_print_arg == "std_err") {
            print_flags = print_flags | intest::PRINT_STD_ERR;
        } else if (s_print_arg == "std_all") {
            print_flags = print_flags | intest::PRINT_STD_ALL;
        } else if (s_print_arg == "exit_code") {
            print_flags = print_flags | intest::PRINT_EXIT_CODE;
        } else if (s_print_arg == "exit_stat") {
            print_flags = print_flags | intest::PRINT_EXIT_STAT;
        } else if (s_print_arg == "time") {
            print_flags = print_flags | intest::PRINT_TIME;
        } else if (s_print_arg == "command_line") {
            print_flags = print_flags | intest::PRINT_COMMAND_LINE;
        } else if (s_print_arg == "error_code_list") {
            print_flags = print_flags | intest::PRINT_ERROR_CODE_LIST;
        } else if (s_print_arg == "status_list") {
            print_flags = print_flags | intest::PRINT_STATUS_LIST;
        } else if (s_print_arg == "separators") {
            print_test_separators = true;
        } else {
            intest::Manager::printLn (
                        QString("Invalid argument to --print: %1")
                        .arg (s_print_arg));
            terminate_with_error (-1);
        }
    }
    intest::Manager::setFlags (print_flags);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void prcess_exclude_arguments (const QString & arg)
{
    QStringList sl_exclude =
            arg.mid (s_exclude_option.length ())
               .toLower ().split (",");

    foreach (const QString & s_one_arg, sl_exclude) {
        Manager::excludeTest (s_one_arg);
    }

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void prcess_only_arguments (const QString & arg)
{
    QStringList sl_only =
            arg.mid (s_only_option.length ())
               .toLower ().split (",");

    foreach (const QString & s_one_arg, sl_only) {
        Manager::addSpecificTest (s_one_arg);
    }

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void prcess_hide_arguments (const QString & arg)
{
    // example: --hide:std_err
    // example: --hide:separators
    // example: --hide:command_line,error_code_list,exit_code,exit_stat,status_list,std_all,std_err,std_out,time
    int print_flags = intest::Manager::flags ();
    QStringList sl_hide =
            arg.mid (s_hide_option.length ())
               .toLower ().split (",");
    foreach(const QString & s_hide_arg, sl_hide) {
        if (s_hide_arg == "all") {
            print_flags = intest::NO_FLAG;
        } else if (s_hide_arg == "std_out") {
            print_flags = print_flags & ~intest::PRINT_STD_OUT;
        } else if (s_hide_arg == "std_err") {
            print_flags = print_flags & ~intest::PRINT_STD_ERR;
        } else if (s_hide_arg == "std_all") {
            print_flags = print_flags & ~intest::PRINT_STD_ALL;
        } else if (s_hide_arg == "exit_code") {
            print_flags = print_flags & ~intest::PRINT_EXIT_CODE;
        } else if (s_hide_arg == "exit_stat") {
            print_flags = print_flags & ~intest::PRINT_EXIT_STAT;
        } else if (s_hide_arg == "time") {
            print_flags = print_flags & ~intest::PRINT_TIME;
        } else if (s_hide_arg == "command_line") {
            print_flags = print_flags & ~intest::PRINT_COMMAND_LINE;
        } else if (s_hide_arg == "error_code_list") {
            print_flags = print_flags & ~intest::PRINT_ERROR_CODE_LIST;
        } else if (s_hide_arg == "status_list") {
            print_flags = print_flags & ~intest::PRINT_STATUS_LIST;
        } else if (s_hide_arg == "separators") {
            print_test_separators = false;
        } else {
            intest::Manager::printLn (
                        QString("Invalid argument to --hide: %1")
                        .arg (s_hide_arg));
            terminate_with_error (-1);
        }
    }
    intest::Manager::setFlags (print_flags);
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
int			main				(int argc, char *argv[])
{
    QCoreApplication app (argc, argv);
    bool b_first = true;
    foreach(const QString & arg, QCoreApplication::arguments ()) {
        if (b_first) {
            b_first = false;
        } else if (arg.startsWith (s_print_option)) {
            prcess_print_arguments (arg);
        } else if (arg.startsWith (s_hide_option)) {
            prcess_hide_arguments (arg);

        } else if (arg.startsWith (s_exclude_option)) {
            prcess_exclude_arguments (arg);
        } else if (arg.startsWith (s_only_option)) {
            prcess_only_arguments (arg);

        } else if (arg.startsWith ("-")) {
            intest::Manager::printLn (
                        QString("Invalid argument: %1")
                        .arg (arg));
            terminate_with_error (-1);
        } else {
            Manager::addSpecificTest (arg);
        }
    }

    QCoreApplication::setOrganizationName (PILE_INTEGRATION_ORGANIZATION);
    QCoreApplication::setOrganizationDomain (PILE_INTEGRATION_DOMAIN);
    QCoreApplication::setApplicationName (PILE_INTEGRATION_APPNAME);

    int result = Manager::execAll ();
    Manager::end ();
    app.quit ();
    return result;
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
Manager::Manager() :
    list_(),
    fstdout_(new QFile()),
    d_(NULL),
    flags_(PRINT_FAILED),
    sl_exclusion_(),
    sl_inclusion_()
{

    singleton_ = this;
    if (!fstdout_->open (stdout, QIODevice::WriteOnly)) {
        qDebug() << fstdout_->errorString ();
        d_ = new QDebug(QtDebugMsg);
    } else {
        d_ = new QDebug(fstdout_);
    }
    d_->noquote ().nospace ();

}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
Manager::~Manager()
{
    //foreach(Integration * iter, list_) {
    //delete iter;
    //}
    delete d_;
    fstdout_->flush ();
    fstdout_->close ();
    delete fstdout_;
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

    if (print_test_separators)
        PRNT << "[][][][][][][][][][][][][][][][][][][][][][]\n";

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

        PRNT << "\nTest case " << i+1 << ": " << instance_name << "\n";

        // execute this test case
        /*int result = */ instance->exec ();

        // print the results
        instance->printResults ();

        // update the total count
        i_success += instance->successful ();
        i_failure += instance->failed ();
        i_complete_success += instance->failed () == 0 ? 1 : 0;

        // and show nice separators
        if (print_test_separators)
            PRNT << "[][][][][][][][][][][][][][][][][][][][][][]\n";

        ++i_performed;
    }

    // Print a global result of all the tests in this program.
    PRNT << "\n"
         << i_performed << " test cases with "
         << i_success   << " successful and "
         << i_failure   << " failed tests\n";

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
}
/* ========================================================================= */

/* ------------------------------------------------------------------------- */
void Manager::processEnds (Process * proc)
{
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


}
/* ========================================================================= */


/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
