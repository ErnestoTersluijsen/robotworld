/*
 * LiDAR.cpp
 *
 *  Created on: Oct 25, 2023
 *      Author: ernesto
 */

#include "LiDAR.hpp"

#include "Logger.hpp"
#include "Robot.hpp"
#include "RobotWorld.hpp"
#include "Wall.hpp"
#include "Shape2DUtils.hpp"
#include "MathUtils.hpp"

#include <random>

namespace Model
{
	/**
	 *
	 */
	/* static */ double LiDAR::stddev = 10.0;

	/**
	 *
	 */
	/* static */ int LiDAR::angle = 2;

	/**
	 *
	 */
	LiDAR::LiDAR(Robot& aRobot) :
		AbstractSensor( aRobot)
	{
	}
	/**
	 *
	 */
	std::shared_ptr<AbstractStimulus> LiDAR::getStimulus() const
	{
		DistanceStimuli result;

		Robot *robot = dynamic_cast<Robot*>(agent);
		if (robot)
		{
			std::random_device rd{};
			std::mt19937 gen{rd()};
		    std::normal_distribution<> noise{0, LiDAR::stddev};

		    std::vector< WallPtr > walls = RobotWorld::getRobotWorld().getWalls();

		    for(int i = 0; i < 360; i += LiDAR::angle)
		    {
		    	double angle = Utils::MathUtils::toRadians(i);
		    	double distance = noDistance;

		    	wxPoint robotLocation = robot->getPosition();
		    	wxPoint laserEndpoint{static_cast<int>(robotLocation.x + std::cos( angle) * lidarBeamLength),
		    		static_cast<int>(robotLocation.y + std::sin( angle) * lidarBeamLength)};


				for (std::shared_ptr< Wall > wall : walls)
				{
					wxPoint wallPoint1 = wall->getPoint1();
					wxPoint wallPoint2 = wall->getPoint2();

					wxPoint interSection = Utils::Shape2DUtils::getIntersection( wallPoint1, wallPoint2, robotLocation, laserEndpoint);

					if(interSection != wxDefaultPosition)
					{
						double temp = Utils::Shape2DUtils::distance(robotLocation,interSection);
						if(temp < distance || distance == noDistance)
						{
							distance = temp;
						}
					}
				}

				if (distance == noDistance)
				{
					result.stimuli.push_back(DistanceStimulus(angle, noDistance));
				}
				else
				{
					result.stimuli.push_back(DistanceStimulus(angle, distance + noise(gen)));
				}
		    }
		}
		return std::make_shared<DistanceStimuli>(result);
	}
	/**
	 *
	 */
	std::shared_ptr< AbstractPercept > LiDAR::getPerceptFor( std::shared_ptr< AbstractStimulus > anAbstractStimulus) const
	{
		DistancePercepts result;

		Robot* robot = dynamic_cast< Robot* >( agent);
		if (robot)
		{
			DistanceStimuli* distanceStimuli = dynamic_cast< DistanceStimuli* >( anAbstractStimulus.get());

			if(distanceStimuli)
			{
				for(auto distanceStimulus : distanceStimuli->stimuli)
				{
					if(distanceStimulus.distance == noDistance)
					{
						result.pointCloud.push_back(DistancePercept(wxPoint(noObject,noObject)));
					}
					else
					{
						wxPoint endpoint{	static_cast< int >( std::cos( distanceStimulus.angle)*distanceStimulus.distance),
														static_cast< int >( std::sin( distanceStimulus.angle)*distanceStimulus.distance)};

						result.pointCloud.push_back(DistancePercept(endpoint));
					}
				}
			}
		}

		return std::make_shared<DistancePercepts>(result);
	}
	/**
	 *
	 */
	std::string LiDAR::asString() const
	{
		return "LiDAR";
	}
	/**
	 *
	 */
	std::string LiDAR::asDebugString() const
	{
		return asString();
	}

} // namespace Model
