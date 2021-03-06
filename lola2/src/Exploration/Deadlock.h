/*!
\file Deadlock.h
\author Karsten
\status approved 18.04.2012

\brief derives deadlock checking from SimpleProperty
*/

#pragma once

#include <Exploration/SimpleProperty.h>
#include <Stores/Store.h>

class Firelist;
//class Store;

/// derives deadlock checking from SimpleProperty
class Deadlock : public SimpleProperty
{
public:
    Deadlock() {}

private:
    /// prepare for search
    virtual bool initProperty(NetState &ns);

    /// check property in Marking::Current, use after fire. Argument is transition just fired.
    virtual bool checkProperty(NetState &ns, index_t);

    /// check property in Marking::Current, use after backfire. Argument is transition just backfired.
    bool updateProperty(NetState &ns, index_t)
    {
        return false;
    }

    /// copy contructor for use in parallel evaluation
    virtual SimpleProperty *copy()
    {
        return new Deadlock();
    }
};
