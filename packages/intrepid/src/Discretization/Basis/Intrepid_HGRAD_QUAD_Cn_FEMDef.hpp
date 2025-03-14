#ifndef INTREPID_HGRAD_QUAD_CN_FEMDEF_HPP
#define INTREPID_HGRAD_QUAD_CN_FEMDEF_HPP
// @HEADER
// ************************************************************************
//
//                           Intrepid Package
//                 Copyright (2007) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Pavel Bochev  (pbboche@sandia.gov)
//                    Denis Ridzal  (dridzal@sandia.gov), or
//                    Kara Peterson (kjpeter@sandia.gov)
//
// ************************************************************************
// @HEADER

/** \file   Intrepid_HGRAD_QUAD_Cn_FEMDef.hpp
    \brief  Definition file for the Intrepid::HGRAD_QUAD_Cn_FEM class.
    \author Created by R. Kirby.
 */

using Teuchos::Array;
using Teuchos::RCP;

namespace Intrepid {
  template<class Scalar, class ArrayScalar>
  Basis_HGRAD_QUAD_Cn_FEM<Scalar,ArrayScalar>::Basis_HGRAD_QUAD_Cn_FEM( const int orderx , const int ordery,
									const ArrayScalar &pts_x ,
									const ArrayScalar &pts_y ): 
    ptsx_( pts_x.dimension(0) , 1 ) ,
    ptsy_( pts_y.dimension(0) , 1 )
  {
    Array<Array<RCP<Basis<Scalar,ArrayScalar> > > > bases(1);
    bases[0].resize(2);
    bases[0][0] = Teuchos::rcp( new Basis_HGRAD_LINE_Cn_FEM<Scalar,ArrayScalar>( orderx , pts_x ) );
    bases[0][1] = Teuchos::rcp( new Basis_HGRAD_LINE_Cn_FEM<Scalar,ArrayScalar>( ordery , pts_y ) );
    this->setBases( bases );

    this->basisCardinality_ = (orderx+1)*(ordery+1);
    if (orderx > ordery) {
      this->basisDegree_ = orderx;
    }
    else {
      this->basisDegree_ = ordery;
    }
    this -> basisCellTopology_ = shards::CellTopology(shards::getCellTopologyData<shards::Quadrilateral<4> >() );
    this -> basisType_         = BASIS_FEM_FIAT;
    this -> basisCoordinates_  = COORDINATES_CARTESIAN;
    this -> basisTagsAreSet_   = false;

    for (int i=0;i<pts_x.dimension(0);i++)
      {
	ptsx_(i,0) = pts_x(i,0);
      }

    for (int i=0;i<pts_y.dimension(0);i++)
      {
	ptsy_(i,0) = pts_y(i,0);
      }

  }

  template<class Scalar, class ArrayScalar>
  Basis_HGRAD_QUAD_Cn_FEM<Scalar,ArrayScalar>::Basis_HGRAD_QUAD_Cn_FEM( const int order,
									const EPointType &pointType ):
    ptsx_( order+1 , 1 ) ,
    ptsy_( order+1 , 1 )
  {
    Array<Array<RCP<Basis<Scalar,ArrayScalar> > > > bases(1);
    bases[0].resize(2);
    bases[0][0] = Teuchos::rcp( new Basis_HGRAD_LINE_Cn_FEM<Scalar,ArrayScalar>( order , pointType ) );
    bases[0][1] = Teuchos::rcp( new Basis_HGRAD_LINE_Cn_FEM<Scalar,ArrayScalar>( order , pointType ) );
    this->setBases( bases );

    this->basisCardinality_ = (order+1)*(order+1);
    this->basisDegree_ = order;
    this -> basisCellTopology_ = shards::CellTopology(shards::getCellTopologyData<shards::Quadrilateral<4> >() );
    this -> basisType_         = BASIS_FEM_FIAT;
    this -> basisCoordinates_  = COORDINATES_CARTESIAN;
    this -> basisTagsAreSet_   = false;

    // fill up the pt arrays with calls to the lattice
    EPointType pt = (pointType==POINTTYPE_EQUISPACED)?pointType:POINTTYPE_WARPBLEND;
    PointTools::getLattice<Scalar,FieldContainer<Scalar> >( ptsx_ ,
							    shards::CellTopology(shards::getCellTopologyData<shards::Line<2> >()) ,
							    order ,
							    0 ,
							    pt );
	  
    for (int i=0;i<order+1;i++)
      {
	ptsy_(i,0) = ptsx_(i,0);
      }
  }

  template<class Scalar, class ArrayScalar>
  void Basis_HGRAD_QUAD_Cn_FEM<Scalar,ArrayScalar>::initializeTags()
  {
    // Basis-dependent initializations
    int tagSize  = 4;        // size of DoF tag, i.e., number of fields in the tag
    int posScDim = 0;        // position in the tag, counting from 0, of the subcell dim 
    int posScOrd = 1;        // position in the tag, counting from 0, of the subcell ordinal
    int posDfOrd = 2;        // position in the tag, counting from 0, of DoF ordinal relative to the subcell
  
    // An array with local DoF tags assigned to the basis functions, in the order of their local enumeration 

    std::vector<int> tags( tagSize * this->getCardinality() );

    // temporarily just put everything on the cell itself
    for (int i=0;i<this->getCardinality();i++) {
       tags[tagSize*i] = 2;
       tags[tagSize*i+1] = 0;
       tags[tagSize*i+2] = i;
       tags[tagSize*i+3] = this->getCardinality();
     }

    Basis<Scalar,ArrayScalar> &xBasis_ = *this->getBases()[0][0];
    Basis<Scalar,ArrayScalar> &yBasis_ = *this->getBases()[0][1];

    // now let's try to do it "right"
    // let's get the x and y bases and their dof
    const std::vector<std::vector<int> >& xdoftags = xBasis_.getAllDofTags();
    const std::vector<std::vector<int> >& ydoftags = yBasis_.getAllDofTags();

    std::map<int,std::map<int,int> > total_dof_per_entity;
    std::map<int,std::map<int,int> > current_dof_per_entity;

    for (int i=0;i<4;i++) {
      total_dof_per_entity[0][i] = 0;
      current_dof_per_entity[0][i] = 0;
    }
    for (int i=0;i<4;i++) {
      total_dof_per_entity[1][i] = 0;
      current_dof_per_entity[1][i] = 0;
    }
    total_dof_per_entity[2][0] = 0;
    current_dof_per_entity[2][0] = 0;
    
    // let's tally the total degrees of freedom on each entity
    for (int j=0;j<yBasis_.getCardinality();j++) {
      const int ydim = ydoftags[j][0];
      const int yent = ydoftags[j][1];
      for (int i=0;i<xBasis_.getCardinality();i++) {
	const int xdim = xdoftags[i][0];
	const int xent = xdoftags[i][1];
	int dofdim;
	int dofent;
	ProductTopology::lineProduct2d( xdim , xent , ydim , yent , dofdim , dofent );
	total_dof_per_entity[dofdim][dofent] += 1;
      }
    }

    int tagcur = 0;
    for (int j=0;j<yBasis_.getCardinality();j++) {
      const int ydim = ydoftags[j][0];
      const int yent = ydoftags[j][1];
      for (int i=0;i<xBasis_.getCardinality();i++) {
	const int xdim = xdoftags[i][0];
	const int xent = xdoftags[i][1];
	int dofdim;
	int dofent;
	ProductTopology::lineProduct2d( xdim , xent , ydim , yent , dofdim , dofent );
	tags[4*tagcur] = dofdim;
	tags[4*tagcur+1] = dofent;
	tags[4*tagcur+2] = current_dof_per_entity[dofdim][dofent];
	current_dof_per_entity[dofdim][dofent]++;
	tags[4*tagcur+3] = total_dof_per_entity[dofdim][dofent];
	tagcur++;
      }
    }

    setOrdinalTagData(this -> tagToOrdinal_,
		      this -> ordinalToTag_,
		      &(tags[0]),
		      this -> basisCardinality_,
		      tagSize,
		      posScDim,
		      posScOrd,
		      posDfOrd);

  }

  template<class Scalar, class ArrayScalar>
  void Basis_HGRAD_QUAD_Cn_FEM<Scalar,ArrayScalar>::getValues( ArrayScalar &outputValues ,
							       const ArrayScalar &inputPoints ,
							       const EOperator operatorType ) const 
  {
#ifdef HAVE_INTREPID_DEBUG
    getValues_HGRAD_Args<Scalar, ArrayScalar>(outputValues,
					      inputPoints,
					      operatorType,
					      this -> getBaseCellTopology(),
					      this -> getCardinality() );
#endif

    FieldContainer<Scalar> xInputPoints(inputPoints.dimension(0),1);
    FieldContainer<Scalar> yInputPoints(inputPoints.dimension(0),1);

    const Basis<Scalar,ArrayScalar> &xBasis_ = *this->bases_[0][0];
    const Basis<Scalar,ArrayScalar> &yBasis_ = *this->bases_[0][1];

    for (int i=0;i<inputPoints.dimension(0);i++) {
      xInputPoints(i,0) = inputPoints(i,0);
      yInputPoints(i,0) = inputPoints(i,1);
    }

    switch (operatorType) {
    case OPERATOR_VALUE:
      {
	FieldContainer<Scalar> xBasisValues(xBasis_.getCardinality(),xInputPoints.dimension(0));
	FieldContainer<Scalar> yBasisValues(yBasis_.getCardinality(),yInputPoints.dimension(0));

	xBasis_.getValues(xBasisValues,xInputPoints,OPERATOR_VALUE);
	yBasis_.getValues(yBasisValues,yInputPoints,OPERATOR_VALUE);

	int bfcur = 0;
	for (int j=0;j<yBasis_.getCardinality();j++) {
	  for (int i=0;i<xBasis_.getCardinality();i++) {
	    for (int k=0;k<inputPoints.dimension(0);k++) {
	      outputValues(bfcur,k) = xBasisValues(i,k) * yBasisValues(j,k);
	    }
	    bfcur++;
	  }
	}
      }
      break;
    case OPERATOR_GRAD:
    case OPERATOR_D1:
      {
	FieldContainer<Scalar> xBasisValues(xBasis_.getCardinality(),xInputPoints.dimension(0));
	FieldContainer<Scalar> yBasisValues(yBasis_.getCardinality(),yInputPoints.dimension(0));
	FieldContainer<Scalar> xBasisDerivs(xBasis_.getCardinality(),xInputPoints.dimension(0),1);
	FieldContainer<Scalar> yBasisDerivs(yBasis_.getCardinality(),yInputPoints.dimension(0),1);

	xBasis_.getValues(xBasisValues,xInputPoints,OPERATOR_VALUE);
	yBasis_.getValues(yBasisValues,yInputPoints,OPERATOR_VALUE);
	xBasis_.getValues(xBasisDerivs,xInputPoints,OPERATOR_D1);
	yBasis_.getValues(yBasisDerivs,yInputPoints,OPERATOR_D1);	

	// there are two multiindices: I need the (1,0) and (0,1) derivatives
	int bfcur = 0;

	for (int j=0;j<yBasis_.getCardinality();j++) {
	  for (int i=0;i<xBasis_.getCardinality();i++) {
	    for (int k=0;k<inputPoints.dimension(0);k++) {
	      outputValues(bfcur,k,0) = xBasisDerivs(i,k,0) * yBasisValues(j,k);
	      outputValues(bfcur,k,1) = xBasisValues(i,k) * yBasisDerivs(j,k,0);
	    }
	    bfcur++;
	  }
	}
      }
      break;
    case OPERATOR_D2:
    case OPERATOR_D3:
    case OPERATOR_D4:
    case OPERATOR_D5: 
    case OPERATOR_D6:
    case OPERATOR_D7:
    case OPERATOR_D8:
    case OPERATOR_D9:
    case OPERATOR_D10:
      {
	FieldContainer<Scalar> xBasisValues(xBasis_.getCardinality(),xInputPoints.dimension(0));
	FieldContainer<Scalar> yBasisValues(yBasis_.getCardinality(),yInputPoints.dimension(0));

	Teuchos::Array<int> partialMult;

	for (int d=0;d<getDkCardinality(operatorType,2);d++) {
	  getDkMultiplicities( partialMult , d , operatorType , 2 );
	  if (partialMult[0] == 0) {
	    xBasisValues.resize(xBasis_.getCardinality(),xInputPoints.dimension(0));
	    xBasis_.getValues( xBasisValues , xInputPoints, OPERATOR_VALUE );
	  }
	  else {
	    xBasisValues.resize(xBasis_.getCardinality(),xInputPoints.dimension(0),1);
	    EOperator xop = (EOperator) ( (int) OPERATOR_D1 + partialMult[0] - 1 );
	    xBasis_.getValues( xBasisValues , xInputPoints, xop );
	    xBasisValues.resize(xBasis_.getCardinality(),xInputPoints.dimension(0));
	  }
	  if (partialMult[1] == 0) {
	    yBasisValues.resize(yBasis_.getCardinality(),yInputPoints.dimension(0));
	    yBasis_.getValues( yBasisValues , yInputPoints, OPERATOR_VALUE );
	  }
	  else {
	    yBasisValues.resize(yBasis_.getCardinality(),yInputPoints.dimension(0),1);
	    EOperator yop = (EOperator) ( (int) OPERATOR_D1 + partialMult[1] - 1 );
	    yBasis_.getValues( yBasisValues , yInputPoints, yop );
	    yBasisValues.resize(yBasis_.getCardinality(),yInputPoints.dimension(0));
	  }


	  int bfcur = 0;
	  for (int j=0;j<yBasis_.getCardinality();j++) {
	    for (int i=0;i<xBasis_.getCardinality();i++) {
	      for (int k=0;k<inputPoints.dimension(0);k++) {
		outputValues(bfcur,k,d) = xBasisValues(i,k) * yBasisValues(j,k);
	      }
	      bfcur++;
	    }
	  }
	}
      }
      break;
    case OPERATOR_CURL:
      {
	FieldContainer<Scalar> xBasisValues(xBasis_.getCardinality(),xInputPoints.dimension(0));
	FieldContainer<Scalar> yBasisValues(yBasis_.getCardinality(),yInputPoints.dimension(0));
	FieldContainer<Scalar> xBasisDerivs(xBasis_.getCardinality(),xInputPoints.dimension(0),1);
	FieldContainer<Scalar> yBasisDerivs(yBasis_.getCardinality(),yInputPoints.dimension(0),1);

	xBasis_.getValues(xBasisValues,xInputPoints,OPERATOR_VALUE);
	yBasis_.getValues(yBasisValues,yInputPoints,OPERATOR_VALUE);
	xBasis_.getValues(xBasisDerivs,xInputPoints,OPERATOR_D1);
	yBasis_.getValues(yBasisDerivs,yInputPoints,OPERATOR_D1);	

	// there are two multiindices: I need the (1,0) and (0,1) derivatives
	int bfcur = 0;

	for (int j=0;j<yBasis_.getCardinality();j++) {
	  for (int i=0;i<xBasis_.getCardinality();i++) {
	    for (int k=0;k<inputPoints.dimension(0);k++) {
	      outputValues(bfcur,k,0) = xBasisValues(i,k) * yBasisDerivs(j,k,0);
	      outputValues(bfcur,k,1) = -xBasisDerivs(i,k,0) * yBasisValues(j,k);
	    }
	    bfcur++;
	  }
	}
      }
      break;      
    default:
        TEUCHOS_TEST_FOR_EXCEPTION( true , std::invalid_argument,
                            ">>> ERROR (Basis_HGRAD_QUAD_Cn_FEM): Operator type not implemented");
        break;
    }
  }

  template<class Scalar,class ArrayScalar>
  void Basis_HGRAD_QUAD_Cn_FEM<Scalar, ArrayScalar>::getValues(ArrayScalar&           outputValues,
							       const ArrayScalar &    inputPoints,
							       const ArrayScalar &    cellVertices,
							       const EOperator        operatorType) const {
    TEUCHOS_TEST_FOR_EXCEPTION( (true), std::logic_error,
			">>> ERROR (Basis_HGRAD_QUAD_Cn_FEM): FEM Basis calling an FVD member function");
  }

  template<class Scalar,class ArrayScalar>
  void Basis_HGRAD_QUAD_Cn_FEM<Scalar, ArrayScalar>::getDofCoords( ArrayScalar & dofCoords ) const
  {
    int cur = 0;
    for (int j=0;j<ptsy_.dimension(0);j++)
      {
	for (int i=0;i<ptsx_.dimension(0);i++)
	  {
	    dofCoords(cur,0) = ptsx_(i,0);
	    dofCoords(cur,1) = ptsy_(j,0);
	    cur++;
	  }
      }
  }

  
}// namespace Intrepid

#endif

#if defined(Intrepid_SHOW_DEPRECATED_WARNINGS)
#ifdef __GNUC__
#warning "The Intrepid package is deprecated"
#endif
#endif

