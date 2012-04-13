/*!
\author Karsten
\file FairnessAssumptions.h
\status approved 25.01.2012
\ingroup g_frontend g_symboltable

This is just a type definition for the possible fairness assumptions that can
be specified for each transition. We consider weak and strong fairness as well
as the absence of fairness.
*/

#pragma once


typedef enum
{
    NO_FAIRNESS     = 0,
    WEAK_FAIRNESS   = 1,
    STRONG_FAIRNESS = 2
}
fairnessAssumption_t;
