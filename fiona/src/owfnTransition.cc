/*****************************************************************************
 * Copyright 2005, 2006, 2007 Peter Massuthe, Daniela Weinberg,              *
 *           Karsten Wolf, Jan Bretschneider                                 *
 *                                                                           *
 * This file is part of Fiona.                                               *
 *                                                                           *
 * Fiona is free software; you can redistribute it and/or modify it          *
 * under the terms of the GNU General Public License as published by the     *
 * Free Software Foundation; either version 2 of the License, or (at your    *
 * option) any later version.                                                *
 *                                                                           *
 * Fiona is distributed in the hope that it will be useful, but WITHOUT      *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for  *
 * more details.                                                             *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with Fiona; if not, write to the Free Software Foundation, Inc., 51       *
 * Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.                      *
 *****************************************************************************/

/*!
 * \file    owfnTransition.cc
 *
 * \brief   functions for Petri net transitions
 *
 * \author  responsible: Daniela Weinberg <weinberg@informatik.hu-berlin.de>
 *
 * \note    This file is part of the tool Fiona and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universität zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/tools4bpel for details.
 *
 */

#include "mynew.h"
#include "owfnTransition.h"
#include "owfn.h"
#include "main.h"
#include "debug.h"
#include "state.h"
#include "options.h"
#include "AnnotatedGraph.h"
#include "GraphFormula.h"

//! \brief constructor
AdjacentPlace::AdjacentPlace(owfnPlace* place, unsigned int multiplicity) :
    place_(place), multiplicity_(multiplicity) {
}

//! \brief returns the owfn place of AdjacentPlace
//! \return owfnPlace
owfnPlace* AdjacentPlace::getOwfnPlace() const {
    return place_;
}


//! \brief returns the Multiplicity of AdjacentPlace
//! \return multiplicity
unsigned int AdjacentPlace::getMultiplicity() const {
    return multiplicity_;
}


//! \brief constructor
owfnTransition::owfnTransition(const std::string& name) :
    Node(name), labelForMatching(GraphFormulaLiteral::TAU), isEnabled_(false),
            isQuasiEnabled_(false), quasiEnabledNr(0), enabledNr(0),
            isConnectedToOtherPort(false), isConnectedToMyPort(false) {
}


//! \brief deconstructor
owfnTransition::~owfnTransition() {
}


//! \brief Adds a owfnPlace to the pre-set of this owfnTransition.
//! \param owfnPlace owfnPlace to be added to this owfnTransition's
//!        pre-set.
//! \param multiplicity Multiplicity of the arc from the given owfnPlace
//!        to this owfnTransition.
void owfnTransition::addPrePlace(owfnPlace* owfnPlace, unsigned int multiplicity) {
    PrePlaces.push_back(AdjacentPlace(owfnPlace, multiplicity));
}


//! \brief returns whether this transition is enabled
//! \return true if so
bool owfnTransition::isEnabled() const {
    return isEnabled_;
}

//! \brief Set enabledness of this owfnTransition.
//! \param isEnabled New value of enabledness.
void owfnTransition::setEnabled(bool isEnabled) {
    isEnabled_ = isEnabled;
}


//! \brief returns whether this transition is quasi enabled
//! \return true if so
bool owfnTransition::isQuasiEnabled() const {
    return isQuasiEnabled_;
}


//! \brief Set quasi enabledness of this owfnTransition.
//! \param isEnabled New value of quasi enabledness.
void owfnTransition::setQuasiEnabled(bool isQuasiEnabled) {
    isQuasiEnabled_ = isQuasiEnabled;
}


#ifdef STUBBORN
//! \brief DESCRIPTON
//! \return DESCRIPTON
bool owfnTransition::prePlaceIsScapegoatForDisabledness(AdjacentPlace prePlace) const {
    return (PN->CurrentMarking[PN->getPlaceIndex(prePlace.getOwfnPlace())] <
            prePlace.getMultiplicity());
}
#endif

//! \brief sets the hashchange
void owfnTransition::set_hashchange() {
    hash_change = 0;
    for (AdjacentPlaces_t::size_type i = 0; i != IncrPlaces.size(); ++i) {
        hash_change += IncrPlaces[i].getMultiplicity()
                *IncrPlaces[i].getOwfnPlace()->hash_factor;
    }
    for (AdjacentPlaces_t::size_type i = 0; i != DecrPlaces.size(); ++i) {
        hash_change -= DecrPlaces[i].getMultiplicity()
                *DecrPlaces[i].getOwfnPlace()->hash_factor;
    }
    hash_change %= HASHSIZE;
}


//! \brief sets initializes various values of the transition
void owfnTransition::initialize() {
    // Create list of Pre-Places for enabling test
    for (unsigned int i = 0; i < getArrivingArcsCount(); ++i) {
        PrePlaces.push_back(AdjacentPlace(getArrivingArc(i)->pl, getArrivingArc(i)->Multiplicity));
    }

    // Create list of places where transition increments marking
    for (unsigned int i = 0; i < getLeavingArcsCount(); i++) {
        //Is oWFNPlace a loop place?
        unsigned int j = 0;
        for (j = 0; j < getArrivingArcsCount(); j++) {
            if ((getLeavingArc(i)->Destination) == (getArrivingArc(j)->Source))
                break;
        }

        if (j < getArrivingArcsCount()) {
            //yes, loop place
            if (getLeavingArc(i)->Multiplicity > getArrivingArc(j)->Multiplicity) {
                // indeed, transition increments place
                IncrPlaces.push_back(AdjacentPlace(getLeavingArc(i)->pl, getLeavingArc(i)->Multiplicity -getArrivingArc(j)->Multiplicity));
            }
        } else {
            // no loop place
            IncrPlaces.push_back(AdjacentPlace(getLeavingArc(i)->pl, getLeavingArc(i)->Multiplicity));
        }
    }

    // Create list of places where transition decrements marking
    for (unsigned int i = 0; i < getArrivingArcsCount(); i++) {
        //Is oWFNPlace a loop place?
        unsigned int j = 0;
        for (j = 0; j < getLeavingArcsCount(); j++) {
            if ((getArrivingArc(i)->Source) == (getLeavingArc(j)->Destination))
                break;
        }

        if (j < getLeavingArcsCount()) {
            //yes, loop place
            if (getArrivingArc(i)->Multiplicity > getLeavingArc(j)->Multiplicity) {
                // indeed, transition decrements place
                DecrPlaces.push_back(AdjacentPlace(getArrivingArc(i)->pl, getArrivingArc(i)->Multiplicity -getLeavingArc(j)->Multiplicity));
            }
        } else {
            // no loop place
            DecrPlaces.push_back(AdjacentPlace(getArrivingArc(i)->pl, getArrivingArc(i)->Multiplicity));
        }
    }

    // Create list of transitions where enabledness can change
    // if this transition fires.
    for (AdjacentPlaces_t::size_type i = 0; i != IncrPlaces.size(); ++i) {
        owfnPlace* incrOwfnPlace = IncrPlaces[i].getOwfnPlace();
        for (unsigned int j = 0; j < incrOwfnPlace->getLeavingArcsCount(); j++) {
            if (!incrOwfnPlace->getLeavingArc(j)->tr->isEnabled()) {
                // not yet in list
                ImproveEnabling.push_back(incrOwfnPlace->getLeavingArc(j)->tr);
            }
        }
    }

    // Create list of transitions where enabledness can change
    // if this transition fires.
    for (AdjacentPlaces_t::size_type i = 0; i != DecrPlaces.size(); ++i) {
        owfnPlace* decrOwfnPlace = DecrPlaces[i].getOwfnPlace();
        for (unsigned int j = 0; j < decrOwfnPlace->getLeavingArcsCount(); j++) {
            if (!decrOwfnPlace->getLeavingArc(j)->tr->isEnabled()) {
                // not yet in list
                ImproveDisabling.push_back(decrOwfnPlace->getLeavingArc(j)->tr);
            }
        }
    }

    set_hashchange();
#ifdef STUBBORN
    stamp = 0;
    NextStubborn = 0;
    instubborn = false;

    // create list of conflicting transitions
    for (unsigned int i = 0; i != PrePlaces.size(); ++i) {
        AdjacentPlace prePlace = PrePlaces[i];
        owfnPlace* preOwfnPlace = prePlace.getOwfnPlace();
        for (unsigned int j = 0; j < preOwfnPlace->getLeavingArcsCount(); j++) {
            if (preOwfnPlace->getLeavingArc(j)->tr != this) {
                if (!preOwfnPlace->getLeavingArc(j)->tr ->isEnabled()) {
                    conflicting.push_back(preOwfnPlace->getLeavingArc(j)->tr);
                }
            }
        }
    }

    mustbeincluded = conflicting;
#endif
}


//! \brief fires the transition in a given owfn and changes its state and marking accordingly
//! \param PN the owfn to be fired in
void owfnTransition::fire(oWFN * PN) {

    trace(TRACE_5, "owfnTransition::fire(oWFN * PN) : start\n");
 
    for (AdjacentPlaces_t::size_type i = 0; i != IncrPlaces.size(); ++i) {
        AdjacentPlace incrPlace = IncrPlaces[i];
        owfnPlace* incrOwfnPlace = incrPlace.getOwfnPlace();
        
        PN->CurrentMarking[PN->getPlaceIndex(incrOwfnPlace)] += incrPlace.getMultiplicity();
#ifdef CHECKCAPACITY
        if(PN->CurrentMarking[PN->getPlaceIndex(incrOwfnPlace)] > incrOwfnPlace->capacity) {
            cerr << "capacity of place " << incrOwfnPlace->name << " exceeded!" << endl;
            throw CapacityException(incrOwfnPlace->name);
            //_exit(4);
        }
#endif
    }
    for (AdjacentPlaces_t::size_type i = 0; i != DecrPlaces.size(); ++i) {
        AdjacentPlace decrPlace = DecrPlaces[i];
        owfnPlace* decrOwfnPlace = decrPlace.getOwfnPlace();

        if (PN->CurrentMarking[PN->getPlaceIndex(decrOwfnPlace)]
                < decrPlace.getMultiplicity()) {
            PN->printCurrentMarking();
            cerr << "marking of place "<< decrOwfnPlace->name<< " is: "
                    << PN->CurrentMarking[PN->getPlaceIndex(decrOwfnPlace)]
                    << " but transition "<< this->name<< " consumes: "
                    << decrPlace.getMultiplicity() << endl;
            cerr << "number of states calculated so far: "<< State::state_count
                    << endl;
            _exit(EC_CAPACITY_EXCEEDED);
        } else {
            PN->CurrentMarking[PN->getPlaceIndex(decrOwfnPlace)] -= decrPlace.getMultiplicity();
        }
    }

    PN->placeHashValue += hash_change;
    PN->placeHashValue %= HASHSIZE;
    for (ImproveDisEnabling_t::size_type i = 0; i != ImproveEnabling.size(); ++i) {
        owfnTransition* transition = ImproveEnabling[i];
        if (!(transition->isEnabled()) || !(transition->isQuasiEnabled())) {
            transition->check_enabled(PN);
        }
    }
    for (ImproveDisEnabling_t::size_type i = 0; i != ImproveDisabling.size(); ++i) {
        owfnTransition* transition = ImproveDisabling[i];
        if (transition->isEnabled() || transition->isQuasiEnabled()) {
            transition->check_enabled(PN);
        }
    }

    // update value of formula after having fired t
    //    for(p = DecrPlaces; *p != NULL; p++) {
    //		unsigned int j;
    //        for(j=0; j < (* p) -> cardprop; j++) {
    //            if ((* p)->proposition != NULL) {
    //	            (* p)->proposition[j] -> update(PN->CurrentMarking[(* p)->index]);
    //			}
    //        }
    //    }
    //    
    //    for(p = IncrPlaces; * p != NULL; p++) {
    //		unsigned int j;
    //        for(j=0; j < (* p) -> cardprop; j++) {
    //            if ((* p)->proposition != NULL) {
    //	            (* p)->proposition[j] -> update(PN->CurrentMarking[(* p)->index]);
    //            }
    //        }
    //    }

    trace(TRACE_5, "owfnTransition::fire(oWFN * PN) : end\n");
}


//void owfnTransition::backfire(oWFN * PN)
//{
//  PN->placeHashValue -= hash_change;
//  PN->placeHashValue %= HASHSIZE;
// 
//}


//! \brief fires the transition in a given owfn backward and changes its state and marking accordingly
//! \param PN the owfn to be fired in
void owfnTransition::backfire(oWFN * PN) {

    /*
     owfnPlace* * p;
     unsigned int * i;
     for(p = IncrPlaces,i = Incr; *p < UINT_MAX; p++,i++)
     {
     PN->CurrentMarking[* p] -= * i;
     }
     for(p = DecrPlaces,i = Decr; * p < UINT_MAX; p++,i++)
     {
     PN->CurrentMarking[* p] += * i;
     }
     PN->placeHashValue -= hash_change;
     PN->placeHashValue %= HASHSIZE;
    */

    for (ImproveDisEnabling_t::size_type i = 0; i != ImproveEnabling.size(); ++i) {
        owfnTransition* transition = ImproveEnabling[i];
        if (transition->isEnabled()) {
            transition->check_enabled(PN);
        }
    }

    for (ImproveDisEnabling_t::size_type i = 0; i != ImproveDisabling.size(); ++i) {
        owfnTransition* transition = ImproveDisabling[i];
        if (!transition->isEnabled()) {
            transition->check_enabled(PN);
        }
    }

    // update value of formula after having fired t
    //    for(p = DecrPlaces,i = Decr; * p != NULL; p++,i++)
    //    {
    //	unsigned int j;
    //        for(j=0; j < (* p) -> cardprop;j++)
    //	{
    //            (* p)->proposition[j] -> update(PN->CurrentMarking[(* p)->index]);
    //        }
    //    }
    //    for(p = IncrPlaces,i = Incr; * p != NULL; p++,i++)
    //    {
    //	unsigned int j;
    //        for(j=0; j < (* p) -> cardprop;j++)
    //	{
    //	  (* p)->proposition[j] -> update(PN->CurrentMarking[(* p)->index]);
    //        }
    //    }
}


//! \brief check whether this transition is (quasi) enabled at the current marking
//!        quasi-enabled means, that this transition activates a sending event (this transition is a receiving transition)
//! \param PN owfn this transition is part of
//! \fn void owfnTransition::check_enabled(oWFN * PN)
void owfnTransition::check_enabled(oWFN * PN) {
    //   	trace(TRACE_5, "owfnTransition::check_enabled(oWFN * PN) : start\n");

    messageSet.clear();

    enabledNr = quasiEnabledNr = 0; // suppose no pre-place has appropriate marking at all
#ifdef STUBBORN
    mustbeincluded = conflicting;
#endif
    for (AdjacentPlaces_t::size_type iPrePlace = 0;
         iPrePlace != PrePlaces.size(); ++iPrePlace) {

        AdjacentPlace prePlace = PrePlaces[iPrePlace];
        owfnPlace* preOwfnPlace = prePlace.getOwfnPlace();

        if (prePlaceIsScapegoatForDisabledness(prePlace)) {
#ifdef STUBBORN
            scapegoat = preOwfnPlace;
            mustbeincluded = preOwfnPlace->PreTransitions;
#endif
            if (preOwfnPlace->type == INPUT) {
                messageSet.insert(PN->getPlaceIndex(preOwfnPlace));
                quasiEnabledNr++; // remember that we have found an input pre-place with no appropriate marking
            }
        } else {
            enabledNr++; // remember that we have found a pre-place with appropriate marking
        }
    }

  //  cout << "BEFORE" << endl;
 //   cout << "current marking: " << PN->getCurrentMarkingAsString() << endl;
 //   cout << "transition " << name << " is quasiEnabled: " << isQuasiEnabled() << " and enabled: " << isEnabled() << endl;
    // [LUHME XV] Check auf Enabledness und Quasi-Enabledness überarbeiten: evtl. kann sich ein Check einsparen lassen.
    // [LUHME XV] Wenn enabled, dann auch quasi-enabled.
    // [LUHME XV] Evtl. zwei Versionen, weil OG keine Quasi-Enabledness braucht.

    if (enabledNr == getArrivingArcsCount()) { // there are as many pre-places appropriatly marked as there are incoming arcs
        if (!isEnabled()) { // transition was not enabled before
            // include transition into list of enabled transitions
            PN->enabledTransitions.add(this);
            setEnabled(true);
        }
        if (isQuasiEnabled()) { // transition was quasi enabled before
            setQuasiEnabled(false);
            PN->quasiEnabledTransitions.remove(this); // delete transition from list of quasi enabled transtions
        }
    } else if ((enabledNr + quasiEnabledNr) == getArrivingArcsCount()) {
        // transition is quasi enabled
        if (!isQuasiEnabled()) { // transition was not quasi enabled before
            // include transition into list of quasi enabled transitions
            PN->quasiEnabledTransitions.add(this);
            setQuasiEnabled(true);
        }
        if (isEnabled()) { // transition was enabled before
            setEnabled(false);
            PN->enabledTransitions.remove(this); // delete transition from list of enabled transtions

        }
    } else { // transition is not enabled at all
        if (isEnabled()) { // transition was enabled before
            setEnabled(false);
            PN->enabledTransitions.remove(this); // delete transition from list of enabled transtions

        }
        if (isQuasiEnabled()) { // transition was quasi enabled before
            setQuasiEnabled(false);
            PN->quasiEnabledTransitions.remove(this); // delete transition from list of quasi enabled transtions
        }
    }
    
//    cout << "AFTER" << endl;
 //  cout << "current marking: " << PN->getCurrentMarkingAsString() << endl;
 //  cout << "transition " << name << "(" << this << ")" << " is quasiEnabled: " << isQuasiEnabled() << " and enabled: " << isEnabled() << endl;

    // trace(TRACE_5, "owfnTransition::check_enabled(oWFN * PN) : end\n");
}


//! \brief sets the label for matching
//! \param label new label
void owfnTransition::setLabelForMatching(const std::string& label) {
    labelForMatching = label;
}


//! \brief returns the label for matching
//! \return label for matching
std::string owfnTransition::getLabelForMatching() const {
    return labelForMatching;
}


//! \brief returns true if there is no tau label for matching
//! \return true if there is no tau label for matching, else false
bool owfnTransition::hasNonTauLabelForMatching() const {
    return getLabelForMatching() != GraphFormulaLiteral::TAU;
}
