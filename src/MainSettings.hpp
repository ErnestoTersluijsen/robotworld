#ifndef MAINSETTINGS_HPP_
#define MAINSETTINGS_HPP_

#include "Config.hpp"

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

		private:
			bool drawOpenSet;
			unsigned long speed;
			unsigned long worldNumber;

			bool useKalmanFilter;
			bool useParticleFilter;
	};

} /* namespace Application */

#endif /* SRC_MAINSETTINGS_HPP_ */
