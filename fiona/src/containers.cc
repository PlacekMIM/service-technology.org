/*****************************************************************************
 * Copyright 2005, 2006, 2007 Peter Massuthe, Daniela Weinberg,              *
 *           Jan Bretschneider                                               *
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
 * \file    containers.cc
 *
 * \brief   functions for containers used at different locations
 *
 * \author  responsible: Daniela Weinberg <weinberg@informatik.hu-berlin.de>
 *
 * \note    This file is part of the tool Fiona and was created during the
 *          project "Tools4BPEL" at the Humboldt-Universität zu Berlin. See
 *          http://www.informatik.hu-berlin.de/top/tools4bpel for details.
 */

#include "containers.h"
#include "AnnotatedGraphNode.h"

//! \brief 'left' is less than 'right', if the reachGraphStateSet of 'left' is
//!        lexicographical less than 'right's reachGraphStateSet. Consequently,
//!        'left' and 'right' are equal iff both their reachGraphStateSets contain
//!        the same states.
//! \param left left node
//! \param right right node
//! \returns true left is less than right.
//! \returns false left is greater or equal to right.
bool CompareGraphNodes::operator()(AnnotatedGraphNode const* left, AnnotatedGraphNode const* right) {
    trace(TRACE_5, "CompareGraphNodes::operator()() : start\n");
    trace(TRACE_5, "CompareGraphNodes::operator()() : end\n");

    return (left->reachGraphStateSet < right->reachGraphStateSet);
}
