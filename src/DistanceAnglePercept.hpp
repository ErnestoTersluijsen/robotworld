/*
 * DistanceAnglePerpect.hpp
 *
 *  Created on: Oct 4, 2023
 *      Author: ernesto
 */

#ifndef SRC_DISTANCEANGLEPERCEPT_HPP_
#define SRC_DISTANCEANGLEPERCEPT_HPP_

#include "Config.hpp"

#include "DistanceStimulus.hpp"
#include "Point.hpp"

#include <limits>

namespace Model
{
	class DistanceAnglePercept : public AbstractPercept
	{
	public:
		DistanceAnglePercept(const double anAngle, const double aDistance) :angle(anAngle), distance(aDistance)
		{
		}

		double angle;
		double distance;

		virtual std::string asString() const override
		{
			return "DistanceAnglePercept: " + std::to_string(angle) + ", " + std::to_string(distance);
		}

		virtual std::string asDebugString() const override
		{
			return asString();
		}
	};
} // namespace Model
#endif /* SRC_DISTANCEANGLEPERCEPT_HPP_ */

