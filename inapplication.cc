/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file         inapplication.cc
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

#include "inapplication.h"
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

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

static QString s_print_option ("--print:");
static QString s_hide_option  ("--hide:");
static QString s_exclude_option ("--exclude:");
static QString s_only_option  ("--only:");

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
            print_flags = print_flags | intest::PRINT_SEPARATORS;

        } else if (s_print_arg == "success") {
            print_flags = print_flags | intest::PRINT_SUCCESSFUL;
        } else if (s_print_arg == "failure") {
            print_flags = print_flags | intest::PRINT_FAILED;

        } else if (s_print_arg == "colors") {
            print_flags = print_flags | intest::PRINT_COLORS;

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
            print_flags = print_flags & ~intest::PRINT_SEPARATORS;

        } else if (s_hide_arg == "success") {
            print_flags = print_flags & ~intest::PRINT_SUCCESSFUL;
        } else if (s_hide_arg == "failure") {
            print_flags = print_flags & ~intest::PRINT_FAILED;

        } else if (s_hide_arg == "colors") {
            print_flags = print_flags & ~intest::PRINT_COLORS;

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
    AttachConsole (ATTACH_PARENT_PROCESS);
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

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
