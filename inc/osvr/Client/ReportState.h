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

#ifndef INCLUDED_ReportState_h_GUID_452FA875_E51C_4A88_5A58_E28CB6F40B86
#define INCLUDED_ReportState_h_GUID_452FA875_E51C_4A88_5A58_E28CB6F40B86

// Internal Includes
#include <osvr/Util/ClientCallbackTypesC.h>

// Library/third-party includes
// - none

// Standard includes
// - none

namespace osvr {
namespace client {
    namespace traits {

        /// @brief Helper traits struct for reportState(), to access the state
        /// member of the various report types.
        template <typename ReportType> struct ReportStateGetter {
            static typename StateType<ReportType>::type const &
            apply(ReportType const &r) {
                return r.state;
            }
            static typename StateType<ReportType>::type apply(ReportType &r) {
                return r.state;
            }
        };

        // Template specialization to handle OSVR_PositionReport
        template <> struct ReportStateGetter<OSVR_PositionReport> {
            static OSVR_PositionState const &
            apply(OSVR_PositionReport const &r) {
                return r.xyz;
            }
            static OSVR_PositionState apply(OSVR_PositionReport &r) {
                return r.xyz;
            }
        };

        // Template specialization to handle OSVR_PoseReport
        template <> struct ReportStateGetter<OSVR_PoseReport> {
            static OSVR_PoseState const &apply(OSVR_PoseReport const &r) {
                return r.pose;
            }
            static OSVR_PoseState apply(OSVR_PoseReport &r) { return r.pose; }
        };

        // Template specialization to handle OSVR_OrientationReport
        template <> struct ReportStateGetter<OSVR_OrientationReport> {
            static OSVR_OrientationState const &
            apply(OSVR_OrientationReport const &r) {
                return r.rotation;
            }
            static OSVR_OrientationState apply(OSVR_OrientationReport &r) {
                return r.rotation;
            }
        };
    } // namespace traits

    /// @brief Generic const accessor for the "state" member of a report.
    template <typename ReportType>
    typename traits::StateType<ReportType>::type const &
    reportState(ReportType const &r) {
        return traits::ReportStateGetter<ReportType>::apply(r);
    }

    /// @brief Generic accessor for the "state" member of a report.
    template <typename ReportType>
    typename traits::StateType<ReportType>::type &reportState(ReportType &r) {
        return traits::ReportStateGetter<ReportType>::apply(r);
    }
} // namespace client
} // namespace osvr

#endif // INCLUDED_ReportState_h_GUID_452FA875_E51C_4A88_5A58_E28CB6F40B86
