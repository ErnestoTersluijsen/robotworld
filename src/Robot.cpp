#include "Robot.hpp"

#include "Client.hpp"
#include "CommunicationService.hpp"
#include "Goal.hpp"
#include "KalmanFilter.hpp"
#include "LaserDistanceSensor.hpp"
#include "OdometerCompasSensor.hpp"
#include "LiDAR.hpp"
#include "Logger.hpp"
#include "MainApplication.hpp"
#include "MathUtils.hpp"
#include "Message.hpp"
#include "MessageTypes.hpp"
#include "RobotWorld.hpp"
#include "Server.hpp"
#include "Shape2DUtils.hpp"
#include "Wall.hpp"
#include "WayPoint.hpp"
#include "DistanceAnglePercept.hpp"

#include <chrono>
#include <ctime>
#include <sstream>
#include <thread>

namespace Model
{
	/**
	 *
	 */
	Robot::Robot() : Robot("", wxDefaultPosition)
	{
	}
	/**
	 *
	 */
	Robot::Robot( const std::string& aName) : Robot(aName, wxDefaultPosition)
	{
	}
	/**
	 *
	 */
	Robot::Robot(	const std::string& aName,
					const wxPoint& aPosition) :
								name( aName),
								size( wxDefaultSize),
								position( aPosition),
								front( 0, 0),
								speed( 0.0),
								acting(false),
								driving(false),
								communicating(false),
								particleFilter(1000),
								kalmanFilter(),
								previousDeltaX(0),
								previousDeltaY(0)
	{
		std::shared_ptr< AbstractSensor > laserSensor = std::make_shared<LaserDistanceSensor>( *this);
		attachSensor( laserSensor);

		std::shared_ptr< AbstractSensor > odometerCompasSensor = std::make_shared<OdometerCompasSensor>( *this);
		attachSensor( odometerCompasSensor);

		std::shared_ptr< AbstractSensor > lidar = std::make_shared<LiDAR>( *this);
		attachSensor( lidar);

		// We use the real position for starters, not an estimated position.
		startPosition = position;
	}
	/**
	 *
	 */
	Robot::~Robot()
	{
		if(driving)
		{
			Robot::stopDriving();
		}
		if(acting)
		{
			Robot::stopActing();
		}
		if(communicating)
		{
			stopCommunicating();
		}
	}
	/**
	 *
	 */
	void Robot::setName( const std::string& aName,
						 bool aNotifyObservers /*= true*/)
	{
		name = aName;
		if (aNotifyObservers == true)
		{
			notifyObservers();
		}
	}
	/**
	 *
	 */
	wxSize Robot::getSize() const
	{
		return size;
	}
	/**
	 *
	 */
	void Robot::setSize(	const wxSize& aSize,
							bool aNotifyObservers /*= true*/)
	{
		size = aSize;
		if (aNotifyObservers == true)
		{
			notifyObservers();
		}
	}
	/**
	 *
	 */
	void Robot::setPosition(	const wxPoint& aPosition,
								bool aNotifyObservers /*= true*/)
	{
		position = aPosition;
		if (aNotifyObservers == true)
		{
			notifyObservers();
		}
	}
	/**
	 *
	 */
	BoundedVector Robot::getFront() const
	{
		return front;
	}
	/**
	 *
	 */
	void Robot::setFront(	const BoundedVector& aVector,
							bool aNotifyObservers /*= true*/)
	{
		front = aVector;
		if (aNotifyObservers == true)
		{
			notifyObservers();
		}
	}
	/**
	 *
	 */
	// cppcheck-suppress unusedFunction
	float Robot::getSpeed() const
	{
		return speed;
	}
	/**
	 *
	 */
	void Robot::setSpeed( float aNewSpeed,
						  bool aNotifyObservers /*= true*/)
	{
		speed = aNewSpeed;
		if (aNotifyObservers == true)
		{
			notifyObservers();
		}
	}
	/**
	 *
	 */
	void Robot::startActing()
	{
		acting = true;
		std::thread newRobotThread( [this]{	startDriving();});
		robotThread.swap( newRobotThread);
	}
	/**
	 *
	 */
	void Robot::stopActing()
	{
		acting = false;
		driving = false;
		robotThread.join();
	}
	/**
	 *
	 */
	void Robot::startDriving()
	{
		driving = true;

		goal = RobotWorld::getRobotWorld().getGoal( "Goal");
		calculateRoute(goal);

		drive();
	}
	/**
	 *
	 */
	void Robot::stopDriving()
	{
		driving = false;
	}
	/**
	 *
	 */
	void Robot::startCommunicating()
	{
		if(!communicating)
		{
			communicating = true;

			std::string localPort = "12345";
			if (Application::MainApplication::isArgGiven( "-local_port"))
			{
				localPort = Application::MainApplication::getArg( "-local_port").value;
			}

			if(Messaging::CommunicationService::getCommunicationService().isStopped())
			{
				TRACE_DEVELOP( "Restarting the Communication service");
				Messaging::CommunicationService::getCommunicationService().restart();
			}

			server = std::make_shared<Messaging::Server>(	static_cast<unsigned short>(std::stoi(localPort)),
															toPtr<Robot>());
			Messaging::CommunicationService::getCommunicationService().registerServer( server);
		}
	}
	/**
	 *
	 */
	void Robot::stopCommunicating()
	{
		if(communicating)
		{
			communicating = false;

			std::string localPort = "12345";
			if (Application::MainApplication::isArgGiven( "-local_port"))
			{
				localPort = Application::MainApplication::getArg( "-local_port").value;
			}

			Messaging::Client c1ient( 	"localhost",
										static_cast<unsigned short>(std::stoi(localPort)),
										toPtr<Robot>());
			Messaging::Message message( Messaging::StopCommunicatingRequest, "stop");
			c1ient.dispatchMessage( message);
		}
	}
	/**
	 *
	 */
	wxRegion Robot::getRegion() const
	{
		wxPoint translatedPoints[] = { getFrontRight(), getFrontLeft(), getBackLeft(), getBackRight() };
		return wxRegion( 4, translatedPoints); // @suppress("Avoid magic numbers")
	}
	/**
	 *
	 */
	bool Robot::intersects( const wxRegion& aRegion) const
	{
		wxRegion region = getRegion();
		region.Intersect( aRegion);
		return !region.IsEmpty();
	}
	/**
	 *
	 */
	wxPoint Robot::getFrontLeft() const
	{
		// x and y are pointing to top left now
		int x = position.x - (size.x / 2);
		int y = position.y - (size.y / 2);

		wxPoint originalFrontLeft( x, y);
		double angle = Utils::Shape2DUtils::getAngle( front) + 0.5 * Utils::PI;

		wxPoint frontLeft( static_cast<int>((originalFrontLeft.x - position.x) * std::cos( angle) - (originalFrontLeft.y - position.y) * std::sin( angle) + position.x),
						 static_cast<int>((originalFrontLeft.y - position.y) * std::cos( angle) + (originalFrontLeft.x - position.x) * std::sin( angle) + position.y));

		return frontLeft;
	}
	/**
	 *
	 */
	wxPoint Robot::getFrontRight() const
	{
		// x and y are pointing to top left now
		int x = position.x - (size.x / 2);
		int y = position.y - (size.y / 2);

		wxPoint originalFrontRight( x + size.x, y);
		double angle = Utils::Shape2DUtils::getAngle( front) + 0.5 * Utils::PI;

		wxPoint frontRight( static_cast<int>((originalFrontRight.x - position.x) * std::cos( angle) - (originalFrontRight.y - position.y) * std::sin( angle) + position.x),
						  static_cast<int>((originalFrontRight.y - position.y) * std::cos( angle) + (originalFrontRight.x - position.x) * std::sin( angle) + position.y));

		return frontRight;
	}
	/**
	 *
	 */
	wxPoint Robot::getBackLeft() const
	{
		// x and y are pointing to top left now
		int x = position.x - (size.x / 2);
		int y = position.y - (size.y / 2);

		wxPoint originalBackLeft( x, y + size.y);

		double angle = Utils::Shape2DUtils::getAngle( front) + 0.5 * Utils::PI;

		wxPoint backLeft( static_cast<int>((originalBackLeft.x - position.x) * std::cos( angle) - (originalBackLeft.y - position.y) * std::sin( angle) + position.x),
						static_cast<int>((originalBackLeft.y - position.y) * std::cos( angle) + (originalBackLeft.x - position.x) * std::sin( angle) + position.y));

		return backLeft;
	}
	/**
	 *
	 */
	wxPoint Robot::getBackRight() const
	{
		// x and y are pointing to top left now
		int x = position.x - (size.x / 2);
		int y = position.y - (size.y / 2);

		wxPoint originalBackRight( x + size.x, y + size.y);

		double angle = Utils::Shape2DUtils::getAngle( front) + 0.5 * Utils::PI;

		wxPoint backRight( static_cast<int>((originalBackRight.x - position.x) * std::cos( angle) - (originalBackRight.y - position.y) * std::sin( angle) + position.x),
						 static_cast<int>((originalBackRight.y - position.y) * std::cos( angle) + (originalBackRight.x - position.x) * std::sin( angle) + position.y));

		return backRight;
	}
	/**
	 *
	 */
	void Robot::handleNotification()
	{
		//	std::unique_lock<std::recursive_mutex> lock(robotMutex);

		static int update = 0;
		if ((++update % 200) == 0) // @suppress("Avoid magic numbers")
		{
			notifyObservers();
		}
	}
	/**
	 *
	 */
	void Robot::handleRequest( Messaging::Message& aMessage)
	{
		FUNCTRACE_TEXT_DEVELOP(aMessage.asString());

		switch(aMessage.getMessageType())
		{
			case Messaging::StopCommunicatingRequest:
			{
				aMessage.setMessageType(Messaging::StopCommunicatingResponse);
				aMessage.setBody("StopCommunicatingResponse");
				// Handle the request. In the limited context of this works. I am not sure
				// whether this works OK in a real application because the handling is time sensitive,
				// i.e. 2 async timers are involved:
				// see CommunicationService::stopServer and Server::stopHandlingRequests
				Messaging::CommunicationService::getCommunicationService().stopServer(12345,true); // @suppress("Avoid magic numbers")

				break;
			}
			case Messaging::EchoRequest:
			{
				aMessage.setMessageType(Messaging::EchoResponse);
				aMessage.setBody( "Messaging::EchoResponse: " + aMessage.asString());
				break;
			}
			default:
			{
				TRACE_DEVELOP(__PRETTY_FUNCTION__ + std::string(": default not implemented"));
				break;
			}
		}
	}
	/**
	 *
	 */
	void Robot::handleResponse( const Messaging::Message& aMessage)
	{
		FUNCTRACE_TEXT_DEVELOP(aMessage.asString());

		switch(aMessage.getMessageType())
		{
			case Messaging::StopCommunicatingResponse:
			{
				//Messaging::CommunicationService::getCommunicationService().stop();
				break;
			}
			case Messaging::EchoResponse:
			{
				break;
			}
			default:
			{
				TRACE_DEVELOP(__PRETTY_FUNCTION__ + std::string( ": default not implemented, ") + aMessage.asString());
				break;
			}
		}
	}
	/**
	 *
	 */
	std::string Robot::asString() const
	{
		std::ostringstream os;

		os << "Robot " << name << " at (" << position.x << "," << position.y << ")";

		return os.str();
	}
	/**
	 *
	 */
	std::string Robot::asDebugString() const
	{
		std::ostringstream os;

		os << "Robot:\n";
		os << AbstractAgent::asDebugString();
		os << "Robot " << name << " at (" << position.x << "," << position.y << ")\n";

		return os.str();
	}
	/**
	 *
	 */
	void Robot::drive()
	{
		try
		{
			for (std::shared_ptr< AbstractSensor > sensor : sensors)
			{
				sensor->setOn();
			}
		
			// The runtime value always wins!!
			speed = static_cast<float>(Application::MainApplication::getSettings().getSpeed());

			// Compare a float/double with another float/double: use epsilon...
			if (std::fabs(speed - 0.0) <= std::numeric_limits<float>::epsilon())
			{
				setSpeed(10.0, false); // @suppress("Avoid magic numbers")
			}

			// We use the real position for starters, not an estimated position.
			startPosition = position;

			positions.clear();
			positions.push_back(startPosition);

			predictedPositions.clear();

			wxPoint prevPosition = startPosition;

			bool startPositionSet = false;

			unsigned pathPoint = 0;
			while (position.x > 0 && position.x < 1024 && position.y > 0 && position.y < 1024 && pathPoint < path.size()) // @suppress("Avoid magic numbers")
			{
				// Do the update
				const PathAlgorithm::Vertex& vertex = path[pathPoint+=static_cast<unsigned int>(speed)];
				front = BoundedVector( vertex.asPoint(), position);
				position.x = vertex.x;
				position.y = vertex.y;

				// Do the measurements / handle all percepts
				// TODO There are race conditions here:
				//			1. size() is not atomic
				//			2. any percepts added after leaving the while will not be used during the belief update
				while(perceptQueue.size() > 0)
				{
					std::optional< std::shared_ptr< AbstractPercept >> percept = perceptQueue.dequeue();
					if(percept)
					{
						// We cannot dereference the percept in typeid() because clang-tidy gives a warning:
						// warning: expression with side effects will be evaluated despite being used as an operand to 'typeid'
						const AbstractPercept& tempAbstractPercept{*percept.value().get()};

						if( typeid(tempAbstractPercept) == typeid(DistancePercept)) // single percept, this comes from the laser
						{
							DistancePercept* distancePercept = dynamic_cast<DistancePercept*>(percept.value().get());
							currentRadarPointCloud.push_back(*distancePercept);
						}
						else if(typeid(tempAbstractPercept) == typeid(DistanceAnglePercept))
						{
							DistanceAnglePercept* distanceAnglePercept = dynamic_cast<DistanceAnglePercept*>(percept.value().get());
							//TODO implement kalman here you idiot

							if(!startPositionSet)
							{
								kalmanFilter.setStateVector(position.x, position.y);
								startPositionSet = true;
							}

							if(distanceAnglePercept->distance != noDistance && distanceAnglePercept->angle != noAngle)
							{
								double deltaX = distanceAnglePercept->distance * std::cos(distanceAnglePercept->angle);
								double deltaY = distanceAnglePercept->distance * std::sin(distanceAnglePercept->angle);


								if(previousDeltaX != 0 && previousDeltaY != 0) // previous delta X & Y is zero when the robot has moved to its first point
								{
									predictPositionKalman(deltaX, deltaY);
								}

								previousDeltaX = deltaX;
								previousDeltaY = deltaY;
							}
						}
						else if (typeid(tempAbstractPercept) == typeid(DistancePercepts))
						{
							DistancePercepts* distancePercepts = dynamic_cast<DistancePercepts*>(percept.value().get());
							lidarPointCloud = distancePercepts->pointCloud;

							particleFilter.update(lidarPointCloud, position - prevPosition);

							prevPosition = position;
						}
						else
						{
							Application::Logger::log(std::string("Unknown type of percept:") + typeid(tempAbstractPercept).name());
						}
					}else
					{
						Application::Logger::log("Huh??");
					}
				}

				// Update the belief

				// Stop on arrival or collision
				if (arrived(goal) || collision())
				{
					Application::Logger::log(__PRETTY_FUNCTION__ + std::string(": arrived or collision"));
					driving = false;
				}

				notifyObservers();

				// If there is no sleep_for here the robot will immediately be on its destination....
				std::this_thread::sleep_for( std::chrono::milliseconds( 100)); // @suppress("Avoid magic numbers")

				// this should be the last thing in the loop
				if(driving == false)
				{
					break;
				}
			} // while

			for (std::shared_ptr< AbstractSensor > sensor : sensors)
			{
				sensor->setOff();
			}
		}
		catch (std::exception& e)
		{
			Application::Logger::log( __PRETTY_FUNCTION__ + std::string(": ") + e.what());
			std::cerr << __PRETTY_FUNCTION__ << ": " << e.what() << std::endl;
		}
		catch (...)
		{
			Application::Logger::log( __PRETTY_FUNCTION__ + std::string(": unknown exception"));
			std::cerr << __PRETTY_FUNCTION__ << ": unknown exception" << std::endl;
		}
	}
	/**
	 *
	 */
	void Robot::calculateRoute(GoalPtr aGoal)
	{
		path.clear();
		if (aGoal)
		{
			// Turn off logging if not debugging AStar
			Application::Logger::setDisable();

			front = BoundedVector( aGoal->getPosition(), position);
			//handleNotificationsFor( astar);
			path = astar.search( position, aGoal->getPosition(), size);
			//stopHandlingNotificationsFor( astar);

			Application::Logger::setDisable( false);
		}
	}
	/**
	 *
	 */
	bool Robot::arrived(GoalPtr aGoal)
	{
		if (aGoal && intersects( aGoal->getRegion()))
		{
			return true;
		}
		return false;
	}
	/**
	 *
	 */
	bool Robot::collision()
	{
		wxPoint frontLeft = getFrontLeft();
		wxPoint frontRight = getFrontRight();
		wxPoint backLeft = getBackLeft();
		wxPoint backRight = getBackRight();

		const std::vector< WallPtr >& walls = RobotWorld::getRobotWorld().getWalls();
		for (WallPtr wall : walls)
		{
			if (Utils::Shape2DUtils::intersect( frontLeft, frontRight, wall->getPoint1(), wall->getPoint2()) 	||
				Utils::Shape2DUtils::intersect( frontLeft, backLeft, wall->getPoint1(), wall->getPoint2())		||
				Utils::Shape2DUtils::intersect( frontRight, backRight, wall->getPoint1(), wall->getPoint2()))
				// cppcheck-suppress useStlAlgorithm
			{
				return true;
			}
		}
		const std::vector< RobotPtr >& robots = RobotWorld::getRobotWorld().getRobots();
		for (RobotPtr robot : robots)
		{
			if ( getObjectId() == robot->getObjectId())
			{
				continue;
			}
			if(intersects(robot->getRegion()))
			{
				return true;
			}
		}
		return false;
	}

	void Robot::predictPositionKalman(double x, double y)
	{
		Matrix<double, 2, 2> A { { 1.0, 0.0 }, { 0.0, 1.0 } };
		Matrix<double, 2, 2> B { { 1.0, 0.0 }, { 0.0, 1.0 } };
		Matrix<double, 2, 2> C { { 1.0, 0.0 }, { 0.0, 1.0 } };

		Matrix<double, 2, 1> update { { {previousDeltaX} }, { {previousDeltaY} } };
		Matrix<double, 2, 2> Q { { 1.0, 0.0 }, { 0.0, 1.0 } };

		Matrix<double, 2, 1> measurement { {{ kalmanFilter.getStateVector().at(0, 0) + x }}, {{ kalmanFilter.getStateVector().at(1, 0) + y }} };

		kalmanFilter.executeKalmanFilter(A, B, C, update, Q, measurement);

		predictedPositions.push_back(wxPoint(static_cast<int>(kalmanFilter.getStateVector().at(0, 0)), static_cast<int>(kalmanFilter.getStateVector().at(1, 0))));
	}
} // namespace Model
