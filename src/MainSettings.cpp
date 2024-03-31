#include "MainSettings.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace Application
{
	/**
	 *
	 */
	MainSettings::MainSettings() : drawOpenSet(true), speed(10), worldNumber(0), useKalmanFilter(false), useParticleFilter(false)
	{
		readConfig("config.xml");
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

	double MainSettings::getCompasStddev() const
	{
		return compasStddev;
	}

	void MainSettings::setCompasStddev(double aCompasStddev)
	{
		compasStddev = aCompasStddev;
	}

	double MainSettings::getLidarStddev() const
	{
		return lidarStddev;
	}

	void MainSettings::setLidarStddev(double aLidarStddev)
	{
		lidarStddev = aLidarStddev;
	}

	double MainSettings::getOdometerStddev() const
	{
		return odometerStddev;
	}

	void MainSettings::setOdometerStddev(double anOdometerStddev)
	{
		odometerStddev = anOdometerStddev;
	}


	void MainSettings::readConfig(const std::string& filename)
	{
		boost::property_tree::ptree pt;
		boost::property_tree::read_xml(filename, pt);

		compasStddev = pt.get<double>("root.compas.stddev");
		odometerStddev = pt.get<double>("root.odometer.stddev");

		lidarStddev = pt.get<double>("root.LiDAR.stddev");
	}
} /* namespace Application */
