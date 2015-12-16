/** @file
    @brief Header

    @date 2015

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2015 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef INCLUDED_Angles_h_GUID_DDFCCAF8_9ED0_4E85_62F3_173C073B8BB7
#define INCLUDED_Angles_h_GUID_DDFCCAF8_9ED0_4E85_62F3_173C073B8BB7

// Internal Includes
// - none

// Library/third-party includes
// - none

// Standard includes
#include <ostream>

namespace osvr {
namespace util {
    /// @brief Convenience template class for an angle quantity in an
    /// arbitrary system with arbitrary scalar
    template <typename System, typename Y = double>
    struct AngleGeneric
    {
        using Type = AngleGeneric<System, Y>;
        Y _value;

        /// @brief Converts from the specified system to the generic system.
        AngleGeneric(Y val) : _value(val * System::factor()) {};
        AngleGeneric() { _value={}; };

        /// @brief Copies the value from another system.
        template<typename OtherSystem, typename OtherY>
        AngleGeneric(const AngleGeneric<OtherSystem, OtherY>& val)
            : _value((OtherY)val._value) {};

        Type operator+(const Type& other) const
        {
            Type result;
            result._value = _value + other._value;
            return result;
        }

        Type operator-(const Type& other) const
        {
            Type result;
            result._value = _value - other._value;
            return result;
        }

        Type operator*(const Type& other) const
        {
            Type result;
            result._value = _value * other._value;
            return result;
        }

        Type operator/(const Type& other) const
        {
            Type result;
            result._value = _value / other._value;
            return result;
        }

        /// @brief Casts the value from another system.
        template<typename OtherSystem, typename OtherY>
        operator AngleGeneric<OtherSystem, OtherY>()
        {
            AngleGeneric<OtherSystem, OtherY> result;
            result._value = (OtherY)_value;
            return result;
        }

        /// @brief Converts from the generic system to the specified system.
        Y value() const
        {
            return _value / System::factor();
        }

        /// @brief Outputs the value in the specified system.
        friend std::ostream& operator<<(std::ostream & lhs, const AngleGeneric& rhs)
        {
            lhs << rhs.value() << ' ' << System::symbol();
            return lhs;
        }
    };

    /// @brief Alias for an angle in radians with arbitrary scalar type
    struct radians
    {
        static std::string name()   { return("radian"); }
        static std::string symbol() { return("rad"); }
        static double      factor() { return(1.0); }
    };
    template <typename Y>
    using AngleRadians = AngleGeneric<radians, Y>;

    /// @brief Alias for an angle in degrees with arbitrary scalar type
    struct degrees
    {
        static std::string name()   { return("degree"); }
        static std::string symbol() { return("deg"); }
        static double      factor() { return(6.28318530718/360.0); }
    };
    template <typename Y>
    using AngleDegrees = AngleGeneric<degrees, Y>;

    typedef AngleRadians<double> AngleRadiansd;
    typedef AngleDegrees<double> AngleDegreesd;

    /// @brief Default angle type
    typedef AngleRadiansd Angle;

    /// @brief Get the raw scalar value of your angle in radians
    template <typename System, typename Y>
    inline Y getRadians(AngleGeneric<System, Y> const angle) {
        const AngleRadians<Y> ret(angle);
        return ret.value();
    }

    /// @brief Get the raw scalar value of your angle in degrees
    template <typename System, typename Y>
    inline Y getDegrees(AngleGeneric<System, Y> const angle) {
        const AngleDegrees<Y> ret(angle);
        return ret.value();
    }

} // namespace util
} // namespace osvr
#endif // INCLUDED_Angles_h_GUID_DDFCCAF8_9ED0_4E85_62F3_173C073B8BB7
