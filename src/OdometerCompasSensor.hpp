/*
 * OdometerCompasSensor.hpp
 *
 *  Created on: Oct 4, 2023
 *      Author: ernesto
 */

#ifndef SRC_ODOMETERCOMPASSENSOR_HPP_
#define SRC_ODOMETERCOMPASSENSOR_HPP_

#include "Config.hpp"

#include "AbstractSensor.hpp"
#include "DistancePercept.hpp"

namespace Model
{
	class Robot;
	typedef std::shared_ptr<Robot> RobotPtr;


	class OdometerCompasSensor: public AbstractSensor
	{
	public:
		explicit OdometerCompasSensor(Robot& aRobot);

		virtual std::shared_ptr< AbstractStimulus > getStimulus() const override;
		virtual std::shared_ptr< AbstractPercept > getPerceptFor( std::shared_ptr< AbstractStimulus > anAbstractStimulus) const override;

		static double compasStddev;
		static double odometerStddev;
	};
} // namespace Model
#endif /* SRC_ODOMETERCOMPASSENSOR_HPP_ */
