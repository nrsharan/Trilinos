// @HEADER
// *****************************************************************************
//                           Intrepid2 Package
//
// Copyright 2007 NTESS and the Intrepid2 contributors.
// SPDX-License-Identifier: BSD-3-Clause
// *****************************************************************************
// @HEADER

/** \file test_01.hpp
    \brief  Unit tests for the Intrepid2::HGRAD_HEX_Cn_FEM class.
    \author Created by P. Bochev, D. Ridzal, K. Peterson, Kyungjoo Kim
 */


#include "Intrepid2_config.h"
#include "Kokkos_Random.hpp"
#ifdef HAVE_INTREPID2_DEBUG
#define INTREPID2_TEST_FOR_DEBUG_ABORT_OVERRIDE_TO_CONTINUE
#endif

#include "Intrepid2_Types.hpp"
#include "Intrepid2_Utils.hpp"

#include "Intrepid2_HGRAD_HEX_Cn_FEM.hpp"

namespace Intrepid2 {

  namespace Test {

    // This code provides an example to use serial interface of high order elements
    template<typename OutValueType, typename PointValueType, typename DeviceType>
    int HGRAD_HEX_Cn_FEM_Test02(const bool verbose) {
      using DeviceSpaceType = typename DeviceType::execution_space;
      Kokkos::print_configuration(std::cout, false);

      int errorFlag = 0;

      try { 
        // for higher orders in certain environments, this test can take a while to run in ctest.  So we reduce the number of points as we go beyond 10th order. Also, @mperego is replacing this test, so for now we just restrict to the 10 orders we supported until recently.
        for (int order=1;order<10;++order) {
          Basis_HGRAD_HEX_Cn_FEM<DeviceType,OutValueType,PointValueType> basis(order);
          
          // problem setup 
          //   let's say we want to evaluate 1000 points in parallel. output values are stored in outputValuesA and B.
          //   A is compuated via serial interface and B is computed with top-level interface.
          const int npts = 1000, ndim = 3;
          Kokkos::DynRankView<OutValueType,DeviceType> outputValuesA("outputValuesA", basis.getCardinality(), npts);
          Kokkos::DynRankView<OutValueType,DeviceType> outputValuesB("outputValuesB", basis.getCardinality(), npts);
          
          Kokkos::View<PointValueType**,DeviceType> inputPointsViewToUseRandom("inputPoints", npts, ndim);
          Kokkos::DynRankView<PointValueType,DeviceType> inputPoints (inputPointsViewToUseRandom.data(),  npts, ndim);
          
          // random values between (-1,1) x (-1,1)
          Kokkos::Random_XorShift64_Pool<DeviceType> random(13718);
          Kokkos::fill_random(inputPointsViewToUseRandom, random, 1.0);
          
          // compute setup
          //   we need vinv and workspace
          const auto vinv = basis.getVandermondeInverse();
          
          // worksize 
          //   workspace per thread is required for serial interface. 
          //   parallel_for with range policy would be good to use stack workspace 
          //   as team policy only can create shared memory 
          //   this part would be tricky as the max size should be determined at compile time
          //   let's think about this and find out the best practice. for now I use the following.
          constexpr int worksize = (Parameters::MaxOrder+1)*4;
          
          // if you use team policy, worksize can be gathered from the basis object and use 
          // kokkos shmem_size APIs to create workspace per team or per thread.
          //const auto worksize_for_teampolicy = basis.getWorksizePerPoint(OPERATOR_VALUE);
          
          // extract point range to be evaluated in each thread
          typedef Kokkos::pair<int,int> range_type;
          
          // parallel execution with serial interface
          Kokkos::RangePolicy<DeviceSpaceType> policy(0, npts);
          Kokkos::parallel_for(policy, KOKKOS_LAMBDA(int i) {
              // we evaluate a single point 
              const range_type pointRange = range_type(i,i+1);
              
              // out (# dofs, # pts), input (# pts, # dims)
              auto output = Kokkos::subview(outputValuesA, Kokkos::ALL(), pointRange);
              auto input  = Kokkos::subview(inputPoints,   pointRange, Kokkos::ALL());
              
              // wrap static workspace with a view; serial interface has a template view interface.
              // either view or dynrankview with a right size is okay.
              OutValueType workbuf[worksize];
              Kokkos::View<OutValueType*,Kokkos::AnonymousSpace> work(&workbuf[0], worksize);
              
              // evaluate basis using serial interface
              Impl::Basis_HGRAD_HEX_Cn_FEM
                ::Serial<OPERATOR_VALUE>::getValues(output, input, work, vinv);
            });
          
          // evaluation using high level interface
          basis.getValues(outputValuesB, inputPoints, OPERATOR_VALUE);
          
          // compare 
          const auto outputValuesA_Host = Kokkos::create_mirror_view(outputValuesA); Kokkos::deep_copy(outputValuesA_Host, outputValuesA);
          const auto outputValuesB_Host = Kokkos::create_mirror_view(outputValuesB); Kokkos::deep_copy(outputValuesB_Host, outputValuesB);
          
          double sum = 0, diff = 0;
          for (size_t i=0;i<outputValuesA_Host.extent(0);++i)
            for (size_t j=0;j<outputValuesA_Host.extent(1);++j) {
              sum += std::abs(outputValuesB_Host(i,j));
              diff += std::abs(outputValuesB_Host(i,j) - outputValuesA_Host(i,j));
              if (verbose && (diff/sum > 1.0e-9)) {
                std::cout << " order = " << order
                          << " i = " << i << " j = " << j 
                          << " val A = " << outputValuesA_Host(i,j) 
                          << " val B = " << outputValuesB_Host(i,j) 
                          << " diff  = " << (outputValuesA_Host(i,j) - outputValuesB_Host(i,j)) 
                          << std::endl;
              }
            }
          if (diff/sum > 1.0e-9) {
            errorFlag = -1;
          }
        }
      } catch (std::exception &err) {
        std::cout << "UNEXPECTED ERROR !!! ----------------------------------------------------------\n";
        std::cout << err.what() << '\n';
        std::cout << "-------------------------------------------------------------------------------" << "\n\n";
        errorFlag = -1000;
      };
      
      if (errorFlag != 0)
        std::cout << "End Result: TEST FAILED\n";
      else
        std::cout << "End Result: TEST PASSED\n";
      
      return errorFlag;
    }
  }
}
