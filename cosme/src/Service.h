/*****************************************************************************\
 Cosme -- Checking Simulation, Matching, and Equivalence

 Copyright (c) 2010 Andreas Lehmann

 Cosme is free software: you can redistribute it and/or modify it under the
 terms of the GNU Affero General Public License as published by the Free
 Software Foundation, either version 3 of the License, or (at your option)
 any later version.

 Cosme is distributed in the hope that it will be useful, but WITHOUT ANY
 WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for
 more details.

 You should have received a copy of the GNU Affero General Public License
 along with Cosme.  If not, see <http://www.gnu.org/licenses/>.
\*****************************************************************************/

#ifndef _SERVICE_H
#define _SERVICE_H

#include <config.h>
#include <map>
#include <set>
#include <sstream>
#include <iostream>
#include "types.h"
#include "BitSetC.h"
#include "Label.h"

class ServiceMarking {
    public:

        ServiceMarking(const std::multimap<label_id_t, std::pair<label_id_t, og_service_index_t> > &successors,
                       unsigned is_final);
        ~ServiceMarking();

        inline label_index_t outDegree() {
            return this->mOutDegree;
        }
        inline og_service_index_t successor(label_index_t pos) {
            assert(pos < this->mOutDegree);
            return this->mSuccessors[pos];
        }
        inline label_id_t label(label_index_t pos) {
            assert(pos <= this->mOutDegree);
            return this->mLabels[pos];
        }
        inline unsigned isFinal() {
            return this->mIsFinal;
        }
        std::string toString() const;

        BitSetC* mEnabledTransitions;

    private:

        label_index_t mOutDegree;
        og_service_index_t* mSuccessors;
        label_id_t* mLabels;
        unsigned mIsFinal;

};

class Service {
    public:
        Service(const std::map<og_service_index_t, ServiceMarking*> &markings,
                const std::set<label_id_t> &interface);
        ~Service();

        void finalize();

        inline ServiceMarking* marking(og_service_index_t pos) {
            assert(pos < this->mSize);
            return this->mMarkings[pos];
        }
        void calculateBitSets(Label& interface);
        inline void clean() {
            for (og_service_index_t i = 0; i < this->mSize; i++) {
                delete this->mMarkings[i]->mEnabledTransitions;
                this->mMarkings[i]->mEnabledTransitions = NULL;
            }
        }
        inline og_service_index_t size() const {
            return this->mSize;
        }
        std::string toString() const;

    private:
        ServiceMarking** mMarkings;
        og_service_index_t mSize;
        std::set<label_id_t> mInterface;

};

#endif
