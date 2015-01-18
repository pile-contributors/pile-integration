/* ========================================================================= */
/* ------------------------------------------------------------------------- */
/*!
  \file         inapplication.h
  \date         January 2015
  \author       Nicu Tofan

*/
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#ifndef PILE_INTEGRATION_APPLICATION_INCLUDE
#define PILE_INTEGRATION_APPLICATION_INCLUDE

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
#include <QString>
#include <QStringList>

/*  INCLUDES    ============================================================ */
//
//
//
//
/*  DEFINITIONS    --------------------------------------------------------- */

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

void terminate_with_error (int error_code);
void prcess_print_arguments (const QString & arg);
void prcess_exclude_arguments (const QString & arg);
void prcess_only_arguments (const QString & arg);
void prcess_hide_arguments (const QString & arg);

/*  FUNCTIONS    =========================================================== */
//
//
//
//
/* ------------------------------------------------------------------------- */
/* ========================================================================= */
#endif /* PILE_INTEGRATION_APPLICATION_INCLUDE */
