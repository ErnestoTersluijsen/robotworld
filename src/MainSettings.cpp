#include "MainSettings.hpp"

namespace Application
{
	/**
	 *
	 */
	MainSettings::MainSettings() : drawOpenSet(true), speed(10), worldNumber(0), useKalmanFilter(false), useParticleFilter(false)
	{
	}
	/**
	 *
	 */
	MainSettings::~MainSettings()
	{
	}
	/**
	 *
	 */
	bool MainSettings::getDrawOpenSet() const
	{
		return drawOpenSet;
	}
	/**
	 *
	 */
	void MainSettings::setDrawOpenSet( bool aDrawOpenSet)
	{
		drawOpenSet = aDrawOpenSet;
	}
	/**
	 *
	 */
	unsigned long MainSettings::getSpeed() const
	{
		return speed;
	}
	/**
	 *
	 */
	void MainSettings::setSpeed( unsigned long aSpeed)
	{
		speed = aSpeed;
	}
	/**
	 *
	 */
	unsigned long MainSettings::getWorldNumber() const
	{
		return worldNumber;
	}
	/**
	 *
	 */
	void MainSettings::setWorldNumber( unsigned long aWorldNumber)
	{
		worldNumber = aWorldNumber;
	}

	bool MainSettings::getUseKalmanFilter() const
	{
		return useKalmanFilter;
	}

	void MainSettings::setUseKalmanFilter( bool aUseKalmanFilter)
	{
		useKalmanFilter = aUseKalmanFilter;
	}

	bool MainSettings::getUseParticleFilter() const
	{
		return useParticleFilter;
	}

	void MainSettings::setUseParticleFilter( bool aUseParticleFilter)
	{
		useParticleFilter = aUseParticleFilter;
	}
} /* namespace Application */
