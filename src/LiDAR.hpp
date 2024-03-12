/*
 * LiDAR.hpp
 *
 *  Created on: Oct 23, 2023
 *      Author: ernesto
 */

#ifndef SRC_LIDAR_HPP_
#define SRC_LIDAR_HPP_

#include "Config.hpp"

#include "AbstractSensor.hpp"
#include "DistancePercepts.hpp"

namespace Model
{
	/**
	 * Compile time configurable length of the laser beam
	 */
	const short int lidarBeamLength = 1024;

	class Robot;
	typedef std::shared_ptr<Robot> RobotPtr;

	/**
	 *
	 */
	class LiDAR : public AbstractSensor
	{
		public:
			/**
			 *
			 */
			explicit LiDAR(Robot& aRobot);
			/**
			 *
			 */
			virtual std::shared_ptr<AbstractStimulus> getStimulus() const override;
			/**
			 *
			 */
			virtual std::shared_ptr<AbstractPercept> getPerceptFor(std::shared_ptr<AbstractStimulus> anAbstractStimulus) const override;
			/**
			 *
			 */
			static void setStdDev(double aStdDev)
			{
				LiDAR::stddev = aStdDev;
			}
			/**
			 *
			 */
			static double getStdDev()
			{
				return stddev;
			}
			/**
			 * @name Debug functions
			 */
			//@{
			/**
			 * Returns a 1-line description of the object
			 */
			virtual std::string asString() const override;
			/**
			 * Returns a description of the object with all data of the object usable for debugging
			 */
			virtual std::string asDebugString() const override;
			//@}
		protected:
		private:
			static double stddev;
			static int angle;
	};

} // namespace Model

#endif /* SRC_LIDAR_HPP_ */
