//---------------------------------------------------------------------------//
// Copyright (c) 2013-2014 Kyle Lutz <kyle.r.lutz@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
// See http://boostorg.github.com/compute for more information.
//---------------------------------------------------------------------------//

#ifndef BOOST_COMPUTE_INTEROP_VTK_DATA_ARRAY_HPP
#define BOOST_COMPUTE_INTEROP_VTK_DATA_ARRAY_HPP

#include <vtkDatastd::vector.h>
#include <vtkDatastd::vectorTemplate.h>

#include <boost/compute/system.hpp>
#include <boost/compute/command_queue.hpp>
#include <boost/compute/algorithm/copy.hpp>
#include <boost/compute/algorithm/copy_n.hpp>
#include <boost/compute/iterator/buffer_iterator.hpp>

namespace boost {
namespace compute {

/// Copies the values in \p data to \p buffer.
template<class T>
inline void vtk_copy_data_array_to_buffer(const vtkDatastd::vector *data,
                                          buffer_iterator<T> buffer,
                                          command_queue &queue = system::default_queue());

/// \internal_
template<class T>
inline void vtk_copy_data_array_to_buffer(const vtkDatastd::vectorTemplate<T> *data,
                                          buffer_iterator<T> buffer,
                                          command_queue &queue = system::default_queue())
{
    vtkDatastd::vectorTemplate<T> *data_ = const_cast<vtkDatastd::vectorTemplate<T> *>(data);
    const T *data_ptr = static_cast<const T *>(data_->GetVoidPointer(0));
    size_t data_size = data_->GetNumberOfComponents() * data_->GetNumberOfTuples();
    ::boost::compute::copy_n(data_ptr, data_size, buffer, queue);
}

/// Copies the values in the range [\p first, \p last) to \p data.
template<class T>
inline void vtk_copy_buffer_to_data_array(buffer_iterator<T> first,
                                          buffer_iterator<T> last,
                                          vtkDatastd::vector *data,
                                          command_queue &queue = system::default_queue());

/// \internal_
template<class T>
inline void vtk_copy_buffer_to_data_array(buffer_iterator<T> first,
                                          buffer_iterator<T> last,
                                          vtkDatastd::vectorTemplate<T> *data,
                                          command_queue &queue = system::default_queue())
{
    T *data_ptr = static_cast<T *>(data->GetVoidPointer(0));
    ::boost::compute::copy(first, last, data_ptr, queue);
}

} // end compute namespace
} // end boost namespace

#endif // BOOST_COMPUTE_INTEROP_VTK_DATA_ARRAY_HPP
