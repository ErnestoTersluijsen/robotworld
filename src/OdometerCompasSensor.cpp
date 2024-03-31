/*
 * OdometerCompasSensor.cpp
 *
 *  Created on: Oct 4, 2023
 *      Author: ernesto
 */

#include "OdometerCompasSensor.hpp"

#include "DistanceAnglePercept.hpp"
#include "MainApplication.hpp"
#include "Robot.hpp"
#include "Shape2DUtils.hpp"
#include "MathUtils.hpp"

#include <random>

namespace Model
{
	/* static */ double OdometerCompasSensor::compasStddev = 2.0;
	/* static */ double OdometerCompasSensor::odometerStddev = 0.1;

	OdometerCompasSensor::OdometerCompasSensor(Robot& aRobot) : AbstractSensor( aRobot)
	{
		Application::MainSettings& mainSettings = Application::MainApplication::getSettings();
		OdometerCompasSensor::compasStddev = mainSettings.getCompasStddev();
		OdometerCompasSensor::odometerStddev = mainSettings.getOdometerStddev();
	}

	std::shared_ptr< AbstractStimulus > OdometerCompasSensor::getStimulus() const
	{
		Robot* robot = dynamic_cast<Robot*>(agent);
		if(robot)
		{
			std::random_device rd{};
			std::mt19937 gen{rd()};
			std::normal_distribution<double> compasNoise{0.0, OdometerCompasSensor::compasStddev};
			std::normal_distribution<double> odometerNoise{0.0, OdometerCompasSensor::odometerStddev};

			wxPoint currentPos = robot->getPosition();
			double angle = Utils::Shape2DUtils::getAngle( robot->getFront()) + Utils::MathUtils::toRadians(compasNoise(gen));

			if(robot->getPositions().size() > 0)
			{
				wxPoint previousPos = robot->getPositions().back();

				// only make a new stimulus if the robot has actually moved, else return invalid stimulus
				if(currentPos != previousPos)
				{
					double distance = Utils::Shape2DUtils::distance(currentPos, previousPos);
					distance += distance * odometerNoise(gen);
					robot->addPosition(currentPos);
					return std::make_shared< DistanceStimulus >( angle, distance);
				}
				return std::make_shared< DistanceStimulus >( noAngle,noDistance);

			}
			robot->addPosition(currentPos);
		}
		return std::make_shared< DistanceStimulus >( noAngle,noDistance);
	}

	std::shared_ptr< AbstractPercept > OdometerCompasSensor::getPerceptFor( std::shared_ptr< AbstractStimulus > anAbstractStimulus) const
	{
		DistanceStimulus* distanceStimulus = dynamic_cast< DistanceStimulus* >( anAbstractStimulus.get());
		if(distanceStimulus)
		{
			if(distanceStimulus->distance == noDistance || distanceStimulus->angle == noAngle)
			{
				return std::make_shared<DistanceAnglePercept>( noAngle, noDistance);
			}
			return std::make_shared<DistanceAnglePercept>( distanceStimulus->angle, distanceStimulus->distance);
		}
		return std::make_shared<DistanceAnglePercept>( noAngle, noDistance);
	}



} // namespace Model
