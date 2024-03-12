/*
 * Particle.cpp
 *
 *  Created on: Oct 26, 2023
 *      Author: ernesto
 */

#include "Particle.hpp"

#include "LiDAR.hpp"

namespace Model
{
	Particle::Particle(wxPoint point) :
			location(point)
	{
		generatePointCloud();
	}

	void Particle::moveParticle(wxPoint movement)
	{
		location += movement;
		generatePointCloud();
	}

	void Particle::generatePointCloud()
	{
		pointCloud.clear();

		std::vector< WallPtr > walls = RobotWorld::getRobotWorld().getWalls();

		for (int i = 0; i < 360; i += 2)
		{
			double angle = Utils::MathUtils::toRadians(i);
			double distance = noDistance;

			for (std::shared_ptr<Wall> wall : walls)
			{
				wxPoint wallPoint1 = wall->getPoint1();
				wxPoint wallPoint2 = wall->getPoint2();
				wxPoint laserEndpoint { static_cast<int>(location.x + std::cos(angle) * lidarBeamLength), static_cast<int>(location.y + std::sin(angle) * lidarBeamLength) };

				wxPoint interSection = Utils::Shape2DUtils::getIntersection( wallPoint1, wallPoint2, location, laserEndpoint);

				if (interSection != wxDefaultPosition)
				{
					double temp = Utils::Shape2DUtils::distance(location, interSection);
					if (temp < distance || distance == noDistance)
					{
						distance = temp;
					}
				}
			}

			if(distance == noDistance)
			{
				pointCloud.push_back(DistancePercept(wxPoint(noObject,noObject)));
			}
			else
			{
				wxPoint endpoint{	static_cast< int >( std::cos( angle) * distance), static_cast< int >( std::sin( angle) * distance)};
				pointCloud.push_back(DistancePercept(endpoint));
			}
		}
	}

	void Particle::computeWeight(PointCloud pc)
	{
//		double difference = 0;
//
//		for (unsigned i = 0; i < pc.size(); ++i) {
//		    // Controleer of de pc-waarde (-1, -1) is en pas deze aan.
//		    wxPoint currentPoint = pc.at(i).point;
//		    if (currentPoint.x == -1 && currentPoint.y == -1) {
//		        currentPoint.x = 1024;
//		        currentPoint.y = 1024;
//		    }
//
//		    difference += std::abs(Utils::Shape2DUtils::distance(wxPoint(0, 0), pointCloud.at(i).point) - Utils::Shape2DUtils::distance(wxPoint(0, 0), currentPoint));
//		}
//
//		weight *= 1.0 / (1.0 + difference);

		double difference = 0;

		for(unsigned i = 0; i < pc.size(); ++i)
		{
			difference += std::abs(Utils::Shape2DUtils::distance(wxPoint(0 ,0), pointCloud.at(i).point) - Utils::Shape2DUtils::distance(wxPoint(0 ,0), pc.at(i).point));
		}

		weight *= 1.0 / (1.0 + difference);

//	    double weight = 1.0;
//
//	    unsigned numValidMeasurements = 0;
//	    double totalDistanceDifference = 0.0;
//
//	    for (unsigned i = 0; i < pc.size(); ++i) {
//	        const wxPoint& measuredPoint = pc[i].point;
//	        const wxPoint& expectedPoint = pointCloud[i].point;
//
//	        if (measuredPoint.x != -1 && measuredPoint.y != -1 && expectedPoint.x != -1 && expectedPoint.y != -1) {
//	            double measuredDistance = Utils::Shape2DUtils::distance(wxPoint(0, 0), measuredPoint);
//	            double expectedDistance = Utils::Shape2DUtils::distance(wxPoint(0, 0), expectedPoint);
//	            // Bereken het absolute verschil tussen de gemeten en verwachte afstanden.
//	            double distanceDifference = std::abs(measuredDistance - expectedDistance);
//
//	            // Update het totale afstandsverschil en het aantal geldige metingen.
//	            totalDistanceDifference += distanceDifference;
//	            numValidMeasurements++;
//	        }
//	    }
//
//	    double meanDistanceDifference = totalDistanceDifference / numValidMeasurements;
//	    weight = 1.0 / (1.0 + meanDistanceDifference);
//
//
//	    this->weight = weight;
	}



} // namespace Model
