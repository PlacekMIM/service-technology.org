
#ifndef _testsstunit_h
#define _testsstunit_h

#include "ap.h"
#include "ialglib.h"

#include "trlinsolve.h"


/*************************************************************************
Main unittest subroutine
*************************************************************************/
bool testsst(bool silent);


/*************************************************************************
Silent unit test
*************************************************************************/
bool testsstunit_test_silent();


/*************************************************************************
Unit test
*************************************************************************/
bool testsstunit_test();


#endif
