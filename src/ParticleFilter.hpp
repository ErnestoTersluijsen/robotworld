/*
 * ParticleFilter.hpp
 *
 *  Created on: Oct 26, 2023
 *      Author: ernesto
 */

#ifndef SRC_PARTICLEFILTER_HPP_
#define SRC_PARTICLEFILTER_HPP_

#include "Particle.hpp"

#include <vector>

namespace Model
{

	class ParticleFilter
	{
	public:
		explicit ParticleFilter(int amountOfParticles);

		void update(const PointCloud& pc, wxPoint movement);

		void controlUpdate(wxPoint movement, double amountOfNoise);

		void measurementUpdate(const PointCloud& pc);

		void resample();

		void moveParticles(wxPoint movement);

		std::vector<Particle> getParticles()
		{
			return particles;
		}

	private:
		std::vector<Particle> particles;
	};
} // namespace Model
#endif /* SRC_PARTICLEFILTER_HPP_ */
