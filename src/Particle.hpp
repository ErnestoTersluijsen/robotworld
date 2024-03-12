/*
 * Particle.hpp
 *
 *  Created on: Oct 26, 2023
 *      Author: ernesto
 */

#ifndef SRC_PARTICLE_HPP_
#define SRC_PARTICLE_HPP_

#include "Logger.hpp"
#include "RobotWorld.hpp"
#include "Wall.hpp"
#include "Shape2DUtils.hpp"
#include "MathUtils.hpp"
#include "DistancePercepts.hpp"

namespace Model
{

	class Particle
	{

	public:
		explicit Particle(wxPoint point);

		void moveParticle(wxPoint movement);

		void generatePointCloud();

		void computeWeight(PointCloud pc);

		wxPoint getLocation() const
		{
			return location;
		}

		void setLocation(wxPoint point)
		{
			location = point;
		}

		const PointCloud& getPointCloud() const
		{
			return pointCloud;
		}

		void setPointCloud(const PointCloud &pc)
		{
			pointCloud = pc;
		}

		double getWeight() const
		{
			return weight;
		}

		void setWeight(double value)
		{
			weight = value;
		}

	private:
		wxPoint location;
		
		PointCloud pointCloud;

		double weight;
	};

} // namespace Model
#endif /* SRC_PARTICLE_HPP_ */
