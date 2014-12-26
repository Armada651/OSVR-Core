/** @file
    @brief Header

    @date 2014

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2014 Sensics, Inc.
//
// All rights reserved.
//
// (Final version intended to be licensed under
// the Apache License, Version 2.0)

#ifndef INCLUDED_Transform_h_GUID_8BF4BBD8_CDC1_48BC_DC27_BFDA42A3212E
#define INCLUDED_Transform_h_GUID_8BF4BBD8_CDC1_48BC_DC27_BFDA42A3212E

// Internal Includes
#include <osvr/Transform/Util.h>

// Library/third-party includes
#include <osvr/Util/EigenCoreGeometry.h>

// Standard includes
// - none

namespace osvr {
/// @brief Handles spatial transformations
namespace transform {

    /// @brief Spatial transformation, consisting of both pre and post
    /// components.
    class Transform {
      public:
        Transform()
            : m_pre(Eigen::Matrix4d::Identity()),
              m_post(Eigen::Matrix4d::Identity()) {}

        template <typename T1, typename T2>
        Transform(T1 const &pre_matrix, T2 const &post_matrix)
            : m_pre(pre_matrix), m_post(post_matrix) {}

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

        template <typename T> void concatPre(T const &xform) { m_pre *= xform; }

        template <typename T> void concatPost(T const &xform) {
            m_post = (xform * m_post).eval();
        }

        /// @brief Update this transformation by the application of another
        /// transformation around it.
        void transform(Transform const &other) {
            concatPre(other.m_pre);
            concatPost(other.m_post);
        }

        /// @brief Apply the transformation to a matrix representing a pose.
        Eigen::Matrix4d transform(Eigen::Matrix4d const &input) const {
            return m_post * input * m_pre;
        }

        Eigen::Matrix4d const &getPre() const { return m_pre; }

        Eigen::Matrix4d const &getPost() const { return m_post; }

      private:
        Eigen::Matrix4d m_pre;
        Eigen::Matrix4d m_post;
    };

    template <typename T>
    inline Eigen::Matrix4d rotate(double degrees, T const &axis) {
        return Eigen::Isometry3d(
                   Eigen::AngleAxisd(degreesToRadians(degrees), axis)).matrix();
    }

} // namespace transform
} // namespace osvr

#endif // INCLUDED_Transform_h_GUID_8BF4BBD8_CDC1_48BC_DC27_BFDA42A3212E
