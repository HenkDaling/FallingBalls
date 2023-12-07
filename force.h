/*
Authors: Henk Daling, Mark Pattillo, Griffin Roe,
    Joshua Gilpin, David Sobernheim
Assignment Title: Falling Balls
Assignment Description: Create the falling balls game
Due Date: 12/6/23
Date Created: 11/15/23
Date Last Modified: 12/6/23
*/

#include <iostream>

using namespace std;

const static double g_acceleration = 0.0981;

class force{
	private:
		double magnitude;
		double direction;  // radian


	public:
        //precondition: nothing
        //postcondition: force object is created
        //return type: nothing
        //description of function: default constructor
		force();

		//precondition: magnitude and direction
        //postcondition: force object is created
        //return type: nothing
        //description of function: constructor with values
		force(double m, double d);

		//precondition: magnitude
        //postcondition: magnitude is changed
        //return type: nothing
        //description of function: sets magnitude to passed value
		void setMagnitude(double m);

		//precondition: direction
        //postcondition: direction is changed
        //return type: nothing
        //description of function: sets direction to passed value
		void setDirection(double d);

		//precondition: nothing
        //postcondition: returns magnitude
        //return type: double
        //description of function: gets magnitude from object
		double getMagnitude()const;

		//precondition: nothing
        //postcondition: returns direction
        //return type: double
        //description of function: gets direction from object
		double getDirection()const;

		//precondition: other force
        //postcondition: forces are added
        //return type: force object
        //description of function: calls add() function
		force operator+(const force& other) const;

		//precondition: other force
        //postcondition: force is set to other force
        //return type: force
        //description of function: sets one force to another
		force operator=(const force& other);

		//precondition: other force
        //postcondition: forces are added
        //return type: force
        //description of function: adds two forces together
		force add(const force& other) const;

		//precondition: other force
        //postcondition: adds other to force
        //return type: nothing
        //description of function: calls add() function
		void apply(const force& other);

		//precondition: ostream is passed
        //postcondition: displays magnitude and direction
        //return type: nothing
        //description of function: displays force
		void display(ostream& os)const{
			os << getMagnitude() << " " << getDirection() << endl;
		}
};
