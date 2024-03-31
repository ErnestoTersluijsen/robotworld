#ifndef MAINSETTINGS_HPP_
#define MAINSETTINGS_HPP_

#include "Config.hpp"

#include <string>

namespace Application
{

	/*
	 *
	 */
	class MainSettings
	{
		public:
			/**
			 *
			 */
			MainSettings();
			/**
			 *
			 */
			virtual ~MainSettings();

			void readConfig(const std::string& filename);

			/**
			 *
			 */
			bool getDrawOpenSet() const;
			/**
			 *
			 */
			void setDrawOpenSet( bool aDrawOpenSet);
			/**
			 *
			 */
			unsigned long getSpeed() const;
			/**
			 *
			 */
			void setSpeed( unsigned long aSpeed);
			/**
			 *
			 */
			unsigned long getWorldNumber() const;
			/**
			 *
			 */
			void setWorldNumber( unsigned long aWorldNumber);


			bool getUseKalmanFilter() const;

			void setUseKalmanFilter( bool aUseKalmanFilter);

			bool getUseParticleFilter() const;

			void setUseParticleFilter( bool aUseKalmanFilter);

			double getCompasStddev() const;

			void setCompasStddev(double aCompasStddev);

			double getLidarStddev() const;

			void setLidarStddev(double aLidarStddev);

			double getOdometerStddev() const;

			void setOdometerStddev(double anOdometerStddev);

		private:
			bool drawOpenSet;
			unsigned long speed;
			unsigned long worldNumber;

			bool useKalmanFilter;
			bool useParticleFilter;

			double compasStddev;
			double odometerStddev;

			double lidarStddev;
	};

} /* namespace Application */

#endif /* SRC_MAINSETTINGS_HPP_ */
