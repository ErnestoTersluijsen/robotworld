/*
 * ParticleFilter.cpp
 *
 *  Created on: Oct 26, 2023
 *      Author: ernesto
 */

#include "ParticleFilter.hpp"

#include "Particle.hpp"
#include <random>

namespace Model
{

	ParticleFilter::ParticleFilter(int amountOfParticles)
	{
		std::random_device rd{};
		std::mt19937 gen{rd()};
		std::uniform_int_distribution<> random(0, 1024);

		for(int i = 0; i < amountOfParticles; ++i)
		{
			particles.push_back(Particle(wxPoint(random(gen), random(gen))));
		}
	}

	void ParticleFilter::moveParticles(wxPoint movement)
	{
		for (auto &particle : particles)
		{
			particle.moveParticle(movement);
		}
	}

	void ParticleFilter::update(PointCloud pc, wxPoint movement)
	{
		controlUpdate(movement, 2);
		measurementUpdate(pc);
		resample();

	}

	void ParticleFilter::controlUpdate(wxPoint movement, double amountOfNoise)
	{
        std::random_device rd{};
        std::mt19937 gen{rd()};
        std::normal_distribution<> noise(0, amountOfNoise);

		for(Particle& particle : particles)
		{
			particle.moveParticle(movement + wxPoint(static_cast<int>(noise(gen)), static_cast<int>(noise(gen))));
		}
	}

	void ParticleFilter::measurementUpdate(PointCloud pc)
	{
		for(auto &particle : particles)
		{
			particle.computeWeight(pc);
		}

	}

	void ParticleFilter::resample()
	{
		std::vector<Particle> resample;

        std::random_device rd{};
        std::mt19937 gen{rd()};
        double totalWeight = 0.0;

        for(Particle& particle : particles)
        {
        	totalWeight += particle.getWeight();
        }

        std::vector<double> weights;
        for(Particle& particle : particles)
        {
        	weights.push_back(particle.getWeight() / totalWeight);
        }

        std::discrete_distribution<int> dist(weights.begin(), weights.end());

        for(unsigned long int i = 0; i < particles.size(); ++i)
        {
        	int index = dist(gen);
        	resample.push_back(particles.at(index));
        }

        for (Particle& particle : resample) {
            particle.setWeight(1.0);
        }

        particles = resample;
	}



} // namespace Model
