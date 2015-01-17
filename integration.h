/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file         integration.h
  \date         January 2015
  \author       Nicu Tofan

*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef PILE_INTEGRATION_TEST_INCLUDE
#define PILE_INTEGRATION_TEST_INCLUDE

//
//
//
//
/*  INCLUDES    ------------------------------------------------------------ */

#include "integration_config.h"
#include "inprocess.h"
#include "inmanager.h"

#include <QList>
#include <QProcess>
#include <QDateTime>



/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

#ifndef STRINGIFY
#   define STRINGIFY(s) STRINGIFY_HELPER(s)
#   define STRINGIFY_HELPER(s) #s
#endif

#define INTEST(__test_name__) \
    namespace intest {\
    class InTest_ ## __test_name__ : public Integration {\
    public: \
        InTest_ ## __test_name__() : Integration() {} \
        virtual ~InTest_ ## __test_name__() {} \
        virtual QString name() const {return STRINGIFY(__test_name__);} \
        virtual int exec() { \
            Process * proc = NULL; \
            int in_test_exec_result = 0; \
            for (;;) {

#define ENDINTEST(__test_name__) \
            break; } \
            if (proc != NULL) delete proc; \
            return in_test_exec_result; \
        } \
    }; \
    static InTest_ ## __test_name__ __test_name__ ## _static_instance; \
    }

#define INTEST_EXEC(__test_exe__, __test_arg__) \
    proc = runMonitoredProcess (__test_exe__, QStringList(__test_arg__));\
    if (!proc->b_started_) { in_test_exec_result = -1; break;}

#define INTEST_EXECN(__test_exe__, __test_arg__) \
    proc = runMonitoredProcess (__test_exe__, __test_arg__); \
    if (!proc->b_started_) { in_test_exec_result = -1; break;}

namespace intest {
    class Process;
    class Manager;
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

class Integration {
    friend class Manager;
private:

protected:
    Integration();
    virtual ~Integration();
    Process * runMonitoredProcess (
            const QString & s_program_path,
            const QStringList & arg_list,
            const QStringList & input = QStringList(),
            const QString & s_working_path = QString());
    virtual int exec() = 0;
public:
    virtual QString name() const = 0;
};

} // namespace intest

/*  CLASS    =============================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#endif /* PILE_INTEGRATION_TEST_INCLUDE */
