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
		force();
		force(double m, double d);

		void setMagnitude(double m);
		void setDirection(double d);
		double getMagnitude()const;
		double getDirection()const;

		force operator+(const force& other) const;
		force operator=(const force& other);
		force add(const force& other) const;
		void apply(const force& other);

		void display(ostream& os)const{
			os << getMagnitude() << " " << getDirection() << endl;
		}
};
