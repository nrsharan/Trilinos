// @HEADER
// *****************************************************************************
//        MueLu: A package for multigrid based preconditioning
//
// Copyright 2012 NTESS and the MueLu contributors.
// SPDX-License-Identifier: BSD-3-Clause
// *****************************************************************************
// @HEADER

#ifndef MUELU_MATLABSMOOTHER_FWD_HPP
#define MUELU_MATLABSMOOTHER_FWD_HPP

#include "MueLu_ConfigDefs.hpp"
#if defined(HAVE_MUELU_MATLAB)

namespace MueLu {
template <class Scalar, class LocalOrdinal, class GlobalOrdinal, class Node>
class MatlabSmoother;
}

#ifndef MUELU_MATLABSMOOTHER_SHORT
#define MUELU_MATLABSMOOTHER_SHORT
#endif

#endif

#endif  // MUELU_MATLABSMOOTHER_FWD_HPP
