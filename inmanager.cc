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

#define PRNT (*(singleton_->d_))

/*  DEFINITIONS    ========================================================= */
//
//
//
//
/*  DATA    ---------------------------------------------------------------- */

Manager * Manager::singleton_ = NULL;

/*  DATA    ================================================================ */
//
//
//
//
/*  FUNCTIONS    ----------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
int			main				(int argc, char *argv[])
{
    QCoreApplication app (argc, argv);
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
    flags_(NO_FLAG)
{
    singleton_ = this;
    if (!fstdout_->open (stdout, QIODevice::WriteOnly)) {
        qDebug() << fstdout_->errorString ();
        d_ = new QDebug(QtDebugMsg);
    } else {
        d_ = new QDebug(fstdout_);
    }
    d_->noquote ().nospace ();

    flags_ = ALL_FLAGS;
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

    PRNT << "[][][][][][][][][][][][][][][][][][][][][][]\n";
    int i_max = singleton_->list_.count ();
    int i_success = 0;
    for (int i = 0; i < i_max; ++i) {
        Integration * instance = singleton_->list_.at(i);
        PRNT << "Test " << i+1 << ": " << instance->name() << "\n";

        int result = instance->exec ();

        if (result == 0) {
            ++i_success;
            PRNT << "     successful\n";
        } else {
            PRNT << "     failed\n";
        }
    }
    PRNT << "[][][][][][][][][][][][][][][][][][][][][][]\n";
    return i_max - i_success;
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
