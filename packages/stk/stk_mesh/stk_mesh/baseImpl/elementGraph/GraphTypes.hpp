// Copyright 2002 - 2008, 2010, 2011 National Technology Engineering
// Solutions of Sandia, LLC (NTESS). Under the terms of Contract
// DE-NA0003525 with NTESS, the U.S. Government retains certain rights
// in this software.
//
 // Redistribution and use in source and binary forms, with or without
 // modification, are permitted provided that the following conditions are
 // met:
 // 
 //     * Redistributions of source code must retain the above copyright
 //       notice, this list of conditions and the following disclaimer.
 // 
 //     * Redistributions in binary form must reproduce the above
 //       copyright notice, this list of conditions and the following
 //       disclaimer in the documentation and/or other materials provided
 //       with the distribution.
 // 
//     * Neither the name of NTESS nor the names of its contributors
//       may be used to endorse or promote products derived from this
//       software without specific prior written permission.
//
 // THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 // "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 // LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 // A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 // OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 // SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 // LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 // DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 // THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 // (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 // OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef PACKAGES_STK_STK_MESH_STK_MESH_BASEIMPL_ELEMENTGRAPH_GRAPHTYPES_HPP_
#define PACKAGES_STK_STK_MESH_STK_MESH_BASEIMPL_ELEMENTGRAPH_GRAPHTYPES_HPP_

#include "stk_mesh/base/Types.hpp"
#include "stk_mesh/base/EntityKey.hpp"

namespace stk
{
namespace mesh
{
struct GraphEdge;

namespace impl
{

using LocalId = int64_t;
static constexpr LocalId INVALID_LOCAL_ID = std::numeric_limits<impl::LocalId>::max();

using PartOrdinals = std::vector<stk::mesh::PartOrdinal>;

struct ParallelSideInfoValue
{
  ConnectivityOrdinal sideOrdinal;
  PartOrdinals sidesetPartOrdinals;
  Permutation sidePermutation{INVALID_PERMUTATION};
  EntityKey sideKey;

  bool operator<(const ParallelSideInfoValue& rhs) const
  {
    return this->sideOrdinal < rhs.sideOrdinal;
  }

  bool operator<(const ConnectivityOrdinal ordinal) const
  {
    return this->sideOrdinal < ordinal;
  }

  bool operator!=(const ParallelSideInfoValue& rhs) const
  {
    return this->sideOrdinal != rhs.sideOrdinal;
  }

  bool operator!=(const ConnectivityOrdinal ordinal) const
  {
    return this->sideOrdinal != ordinal;
  }
};

struct ParallelPartInfoValue
{
  PartOrdinals elementPartOrdinals;
  std::vector<ParallelSideInfoValue> sideInfo;

  const ParallelSideInfoValue* get_parallel_side_info(const GraphEdge& edge) const;
  ParallelSideInfoValue* get_parallel_side_info(const GraphEdge& edge);
};

using ParallelPartInfo = std::map<LocalId, ParallelPartInfoValue>;
using ParallelSelectedInfo = std::map<LocalId, bool>;

}
}
}

#endif /* PACKAGES_STK_STK_MESH_STK_MESH_BASEIMPL_ELEMENTGRAPH_GRAPHTYPES_HPP_ */
