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

#include <stk_unit_test_utils/GetMeshSpec.hpp>

namespace stk
{
namespace unit_test_util
{

namespace simple_fields {

std::string get_mesh_spec(unsigned dim) {
  return stk::unit_test_util::get_mesh_spec(dim);
}

std::string get_mesh_spec(const std::string &optionName)
{
  return stk::unit_test_util::get_mesh_spec(optionName);
}

std::vector<double> get_many_block_coordinates(unsigned numBlocks)
{
  return stk::unit_test_util::get_many_block_coordinates(numBlocks);
}

void get_block_proc_distribution(unsigned numBlocks, unsigned numProc, std::vector<unsigned>& procs)
{
  stk::unit_test_util::get_block_proc_distribution(numBlocks, numProc, procs);
}

std::string get_many_block_mesh_desc(unsigned numBlocks, unsigned numProc)
{
  return stk::unit_test_util::get_many_block_mesh_desc(numBlocks, numProc);
}

} // namespace simple_fields

}
}
