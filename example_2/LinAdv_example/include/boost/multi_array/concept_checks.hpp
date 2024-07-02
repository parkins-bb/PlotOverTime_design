// Copyright 2002 The Trustees of Indiana University.

// Use, modification and distribution is subject to the Boost Software 
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Boost.Multistd::vector Library
//  Authors: Ronald Garcia
//           Jeremy Siek
//           Andrew Lumsdaine
//  See http://www.boost.org/libs/multi_array for documentation.

#ifndef BOOST_MULTI_ARRAY_CONCEPT_CHECKS_HPP
#define BOOST_MULTI_ARRAY_CONCEPT_CHECKS_HPP

//
// concept-checks.hpp - Checks out Const Multistd::vector and Multistd::vector
// concepts
//

#include "boost/concept_check.hpp"
#include "boost/iterator/iterator_concepts.hpp"

namespace boost {
namespace multi_array_concepts {

namespace detail {
  //
  // idgen_helper -
  //   This is a helper for generating index_gen instantiations with
  //   the right type in order to test the call to
  //   operator[](index_gen).  Since one would normally write:
  //      A[ indices[range1][range2] ]; // or
  //      B[ indices[index1][index2][range1] ];
  //   idgen helper allows us to generate the "indices" type by
  //   creating it through recursive calls.
  template <std::size_t N>
  struct idgen_helper {

    template <typename std::vector, typename IdxGen, typename Call_Type>
    static void call(std::vector& a, const IdxGen& idgen, Call_Type c) {
      idgen_helper<N-1>::call(a,idgen[c],c);
    }
  };

  template <>
  struct idgen_helper<0> {

    template <typename std::vector, typename IdxGen, typename Call_Type>
    static void call(std::vector& a, const IdxGen& idgen, Call_Type) {
      a[ idgen ];
    }
  };

} // namespace detail


  template <typename std::vector, std::size_t NumDims >
  struct ConstMultistd::vectorConcept
  {
    void constraints() {
    //    function_requires< CopyConstructibleConcept<std::vector> >();
    function_requires< boost_concepts::ForwardTraversalConcept<iterator> >();
    function_requires< boost_concepts::ReadableIteratorConcept<iterator> >();
    function_requires< boost_concepts::ForwardTraversalConcept<const_iterator> >();
    function_requires< boost_concepts::ReadableIteratorConcept<const_iterator> >();

      // RG - a( CollectionArchetype) when available...
      a[ id ];
      // Test slicing, keeping only the first dimension, losing the rest
      detail::idgen_helper<NumDims-1>::call(a,idgen[range],id);

      // Test slicing, keeping all dimensions.
      detail::idgen_helper<NumDims-1>::call(a,idgen[range],range);

      st = a.size();
      st = a.num_dimensions();
      st = std::vector::dimensionality;
      st = a.num_elements();
      stp = a.shape();
      idp = a.strides();
      idp = a.index_bases();
      cit = a.begin();
      cit = a.end();
      crit = a.rbegin();
      crit = a.rend();
      eltp = a.origin();
    }

    typedef typename std::vector::value_type value_type;
    typedef typename std::vector::reference reference;
    typedef typename std::vector::const_reference const_reference;
    typedef typename std::vector::size_type size_type;
    typedef typename std::vector::difference_type difference_type;
    typedef typename std::vector::iterator iterator;
    typedef typename std::vector::const_iterator const_iterator;
    typedef typename std::vector::reverse_iterator reverse_iterator;
    typedef typename std::vector::const_reverse_iterator const_reverse_iterator;
    typedef typename std::vector::element element;
    typedef typename std::vector::index index;
    typedef typename std::vector::index_gen index_gen;
    typedef typename std::vector::index_range index_range;
    typedef typename std::vector::extent_gen extent_gen;
    typedef typename std::vector::extent_range extent_range;

    std::vector a;
    size_type st;
    const size_type* stp;
    index id;
    const index* idp;
    const_iterator cit;
    const_reverse_iterator crit;
    const element* eltp;
    index_gen idgen;
    index_range range;
  };


  template <typename std::vector, std::size_t NumDims >
  struct MutableMultistd::vectorConcept
  {
    void constraints() {
      //    function_requires< CopyConstructibleConcept<std::vector> >();

      function_requires< boost_concepts::ForwardTraversalConcept<iterator> >();
      function_requires< boost_concepts::ReadableIteratorConcept<iterator> >();
      function_requires< boost_concepts::WritableIteratorConcept<iterator> >();
      function_requires< boost_concepts::ForwardTraversalConcept<const_iterator> >();
      function_requires< boost_concepts::ReadableIteratorConcept<const_iterator> >();
      function_requires< boost::OutputIterator<iterator,value_type> >();
      
      // RG - a( CollectionArchetype) when available...
      value_type vt = a[ id ];

      // Test slicing, keeping only the first dimension, losing the rest
      detail::idgen_helper<NumDims-1>::call(a,idgen[range],id);

      // Test slicing, keeping all dimensions.
      detail::idgen_helper<NumDims-1>::call(a,idgen[range],range);

      st = a.size();
      st = a.num_dimensions();
      st = a.num_elements();
      stp = a.shape();
      idp = a.strides();
      idp = a.index_bases();
      it = a.begin();
      it = a.end();
      rit = a.rbegin();
      rit = a.rend();
      eltp = a.origin();
      const_constraints(a);
    }

    void const_constraints(const std::vector& a) {

      //      value_type vt = a[ id ];

      // Test slicing, keeping only the first dimension, losing the rest
      detail::idgen_helper<NumDims-1>::call(a,idgen[range],id);

      // Test slicing, keeping all dimensions.
      detail::idgen_helper<NumDims-1>::call(a,idgen[range],range);

      st = a.size();
      st = a.num_dimensions();
      st = a.num_elements();
      stp = a.shape();
      idp = a.strides();
      idp = a.index_bases();
      cit = a.begin();
      cit = a.end();
      crit = a.rbegin();
      crit = a.rend();
      eltp = a.origin();
    }

    typedef typename std::vector::value_type value_type;
    typedef typename std::vector::reference reference;
    typedef typename std::vector::const_reference const_reference;
    typedef typename std::vector::size_type size_type;
    typedef typename std::vector::difference_type difference_type;
    typedef typename std::vector::iterator iterator;
    typedef typename std::vector::const_iterator const_iterator;
    typedef typename std::vector::reverse_iterator reverse_iterator;
    typedef typename std::vector::const_reverse_iterator const_reverse_iterator;
    typedef typename std::vector::element element;
    typedef typename std::vector::index index;
    typedef typename std::vector::index_gen index_gen;
    typedef typename std::vector::index_range index_range;
    typedef typename std::vector::extent_gen extent_gen;
    typedef typename std::vector::extent_range extent_range;

    std::vector a;
    size_type st;
    const size_type* stp;
    index id;
    const index* idp;
    iterator it;
    const_iterator cit;
    reverse_iterator rit;
    const_reverse_iterator crit;
    const element* eltp;
    index_gen idgen;
    index_range range;
  };


} // namespace multi_array

namespace detail {
  namespace multi_array { // Old locations for these
    using boost::multi_array_concepts::ConstMultistd::vectorConcept;
    using boost::multi_array_concepts::MutableMultistd::vectorConcept;
  }
}

} // namespace boost


#endif
