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
#include <QRegExp>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

/**
 * @def STRINGIFY
 * @brief Convert a variable to a string.
 */
#ifndef STRINGIFY
#   define STRINGIFY(s) STRINGIFY_HELPER(s)
#   define STRINGIFY_HELPER(s) #s
#endif

#if defined(TARGET_COMPILER_MSVC)
#   define __func__ __FUNCTION__
#endif



/**
 * @def ENDINTEST
 * @brief Boilerplate code to be inserted before the test.
 */
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

/**
 * @def ENDINTEST
 * @brief Boilerplate code to be inserted after the test.
 */
#define ENDINTEST(__test_name__) \
            EXIT_INTEST; \
            end_of_intest: \
            break; } \
            if (proc != NULL) delete proc; \
            return in_test_exec_result; \
        } \
    }; \
    static InTest_ ## __test_name__ __test_name__ ## _static_instance; \
    }


/**
 * @def EXIT_INTEST
 * @brief Exits current test.
 */
#define EXIT_INTEST goto end_of_intest;


/**
 * @def POST_RUN_CHECK
 * @brief Checks the exit status and breaks execution if program did not run.
 * @internal
 */
#define POST_RUN_CHECK \
    if (!proc->b_started_) { in_test_exec_result = -1; break;}


/**
 * @def INTEST_EXEC0
 * @brief Runs a program without arguments.
 */
#define INTEST_EXEC0(__test_exe__) \
    proc = runMonitoredProcess (__test_exe__, QStringList());\
    POST_RUN_CHECK


/**
 * @def INTEST_EXEC
 * @brief Runs a program with a single argument.
 */
#define INTEST_EXEC(__test_exe__, __test_arg__) \
    proc = runMonitoredProcess (__test_exe__, QStringList(__test_arg__));\
    POST_RUN_CHECK


/**
 * @def INTEST_EXECN
 * @brief Runs a program with multiple arguments.
 */
#define INTEST_EXECN(__test_exe__, __test_arg__) \
    proc = runMonitoredProcess (__test_exe__, __test_arg__); \
    POST_RUN_CHECK


/**
 * @def DURATION
 * @brief Can be used in code to compute the duration of a test in seconds.
 */
#define DURATION (proc->durationInSeconds ())


/**
 * @def MILI_DURATION
 * @brief Can be used in code to compute the duration of a test in miliseconds.
 */
#define MILI_DURATION (proc->durationInMiliSeconds ())


/**
 * @def PROG_STD_ERR
 * @brief The string generated through stderr channel.
 */
#define PROG_STD_ERR (proc->std_err_output_)


/**
 * @def PROG_STD_OUT
 * @brief The string generated through stdout channel.
 */
#define PROG_STD_OUT (proc->std_out_output_)


/**
 * @def PROG_OUT
 * @brief The string generated through stderr and stdout channel.
 */
#define PROG_OUT (proc->all_output_)


/**
 * @def EXIT_CODE
 * @brief The exit code (integer) returned by tested program.
 */
#define EXIT_CODE (proc->exitCode ())


/**
 * @def EXIT_STATUS
 * @brief Exit status may be either QProcess::NormalExit or QProcess::CrashExit.
 */
#define EXIT_STATUS (proc->exitStatus ())


/**
 * @def HAS_ERROR(__value__)
 * @brief Tell if a QProcess::ProcessError occured.
 */
#define HAS_ERROR (proc->errors_.contains (__value__))


/**
 * @def HAS_STATE(__value__)
 * @brief Tell if a QProcess::ProcessState occured.
 */
#define HAS_STATE (proc->states_.contains (__value__))


/**
 * @def REGEX_MATCH
 * @brief Tell if a QProcess::ProcessState occured.
 */
#define REGEX_MATCH(__pattern__, __value__) \
    (QRegExp(__pattern__).indexIn (__value__) != -1)




/**
 * @def FATAL
 * @brief Indicate a fatal condition (terminate current test).
 */
#define FATAL true


/**
 * @def NON_FATAL
 * @brief Indicate a non-fatal condition (does not terminate current test).
 */
#define NON_FATAL false


/**
 * @def PRNT
 * @brief Used to print messages.
 */
#define PRNT (Manager::singleton()->d ())



// googletest defines this, too
#ifndef EXPECT_TRUE


/**
 * @def EXPECT_TRUE
 * @brief Performs a boolean test; the execution continues whatever the result is.
 */
#define EXPECT_TRUE(condition) \
    evaluateBoolean(condition,   NON_FATAL, \
        "EXPECT_TRUE (" STRINGIFY(condition) ")", __FILE__, __LINE__)


/**
 * @def EXPECT_FALSE
 * @brief Performs a boolean test; the execution continues whatever the result is.
 */
#define EXPECT_FALSE(condition)  \
    evaluateBoolean(!(condition), NON_FATAL, \
        "EXPECT_FALSE (" STRINGIFY(condition) ")", __FILE__, __LINE__)


/**
 * @def EXPECT_EQ
 * @brief Performs a boolean test; the execution continues whatever the result is.
 */
#define EXPECT_EQ(value_1, value_2)  \
    evaluateBoolean((value_1) == (value_2), NON_FATAL, \
        "EXPECT_EQ (" STRINGIFY(value_1) "," STRINGIFY(value_2) ")", \
        __FILE__, __LINE__)


/**
 * @def EXPECT_NE
 * @brief Performs a boolean test; the execution continues whatever the result is.
 */
#define EXPECT_NE(value_1, value_2)  \
    evaluateBoolean((value_1) != (value_2), NON_FATAL, \
        "EXPECT_NE (" STRINGIFY(value_1) "," STRINGIFY(value_2) ")", \
        __FILE__, __LINE__)


/**
 * @def EXPECT_REGEX
 * @brief Searches the string for the pattern; the execution continues whatever the result is.
 */
#define EXPECT_REGEX(__pattern__, __value__)  \
    evaluateBoolean(REGEX_MATCH(__pattern__, __value__), NON_FATAL, \
        "EXPECT_REGEX (" STRINGIFY(__pattern__) "," STRINGIFY(__value__) ")", \
        __FILE__, __LINE__)


/**
 * @def EXPECT_NREGEX
 * @brief Searches the string for the pattern; the execution continues whatever the result is.
 */
#define EXPECT_NREGEX(__pattern__, __value__)  \
    evaluateBoolean(!REGEX_MATCH(__pattern__, __value__), NON_FATAL, \
        "EXPECT_NREGEX (" STRINGIFY(__pattern__) "," STRINGIFY(__value__) ")", \
        __FILE__, __LINE__)


/**
 * @def ASSERT_TRUE
 * @brief Performs a boolean test; the execution breaks if the test fails.
 */
#define ASSERT_TRUE(condition) \
    if (!evaluateBoolean(condition, FATAL, \
        "ASSERT_TRUE (" STRINGIFY(condition) ")", __FILE__, __LINE__)


/**
 * @def ASSERT_FALSE
 * @brief Performs a boolean test; the execution breaks if the test fails.
 */
#define ASSERT_FALSE(condition)  \
    evaluateBoolean(!(condition), FATAL, \
        "ASSERT_FALSE (" STRINGIFY(condition) ")", __FILE__, __LINE__)


/**
 * @def ASSERT_EQ
 * @brief Performs a boolean test; the execution breaks if the test fails.
 */
#define ASSERT_EQ(value_1, value_2)  \
    evaluateBoolean((value_1) == (value_2), FATAL, \
        "ASSERT_EQ (" STRINGIFY(value_1) "," STRINGIFY(value_2) ")", __FILE__, __LINE__)


/**
 * @def ASSERT_NE
 * @brief Performs a boolean test; the execution breaks if the test fails.
 */
#define ASSERT_NE(value_1, value_2)  \
    evaluateBoolean((value_1) != (value_2), FATAL, \
        "ASSERT_NE (" STRINGIFY(value_1) "," STRINGIFY(value_2) ")", __FILE__, __LINE__)



/**
 * @def ASSERT_REGEX
 * @brief Searches the string for the pattern; the execution breaks if the test fails.
 */
#define ASSERT_REGEX(__pattern__, __value__)  \
    evaluateBoolean(REGEX_MATCH(__pattern__, __value__), FATAL, \
        "ASSERT_REGEX (" STRINGIFY(__pattern__) "," STRINGIFY(__value__) ")", \
        __FILE__, __LINE__)


/**
 * @def ASSERT_NREGEX
 * @brief Searches the string for the pattern; the execution breaks if the test fails.
 */
#define ASSERT_NREGEX(__pattern__, __value__)  \
    evaluateBoolean(!REGEX_MATCH(__pattern__, __value__), FATAL, \
        "ASSERT_NREGEX (" STRINGIFY(__pattern__) "," STRINGIFY(__value__) ")", \
        __FILE__, __LINE__)


#endif // EXPECT_TRUE


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

//! Base class for all test classes that wrap the tests.
class Integration {

    friend class Manager;

private:

    int successful_tests_; /**< Will be updated as the test progresses */
    int failed_tests_; /**< Will be updated as the test progresses */

protected:

    //! Constructor.
    Integration();

    //! Destructor.
    virtual ~Integration();

    //! Starts the associated process.
    Process *
    runMonitoredProcess (
            const QString & s_program_path,
            const QStringList & arg_list,
            const QStringList & input = QStringList(),
            const QString & s_working_path = QString());

    //! Actual test function that must be implemented by all test classes.
    virtual int
    exec() = 0;

    //! Used by various macros to report the result of the check.
    bool
    evaluateBoolean (
            bool condition,
            bool fatal,
            const char * s_condition,
            const char * file,
            int line);

public:

    //! The name of this test.
    virtual QString
    name() const = 0;

    //! Number of good tests.
    int
    successful () const {
        return successful_tests_;
    }

    //! Number of failed tests.
    int
    failed () const {
        return failed_tests_;
    }

    //! Print the results of the test case.
    void
    printResults () const;

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
