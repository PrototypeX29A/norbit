/*----------------------------------------------------------------------------

2D Physics Test Program - a cheesy test harness for 2D physics

by Chris Hecker for my Game Developer Magazine articles.  See my homepage
for more information.

NOTE: This is a hacked test program, not a nice example of Windows programming.
physics.cpp the only part of this you should look at!!!

This material is Copyright 1997 Chris Hecker, All Rights Reserved.
It's for you to read and learn from, not to put in your own articles
or books or on your website, etc.  Thank you.

Chris Hecker
checker@d6.com
http://www.d6.com/users/checker

*/

/*----------------------------------------------------------------------------

physics.h - The header for the physics demo.

10/15/96 - checker

*/

#if !defined(PHYSICS_H)
#define PHYSICS_H


#include <vector>
#include "math2d.h"



/*----------------------------------------------------------------------------

Declarations for physics code.

*/

struct rigid_body
{
	real OneOverMass, OneOverCMMomentOfInertia;

	enum { NumberOfConfigurations = 2 };

	struct configuration
	{
		vector_2 CMPosition;
		real Orientation;

		vector_2 CMVelocity;
		real AngularVelocity;

		vector_2 CMForce;
		real Torque;

	} aConfigurations[NumberOfConfigurations];


};

class simulation_world
{
public:

	int SourceConfigurationIndex;
	int TargetConfigurationIndex;

	simulation_world( );

	void Simulate( real DeltaTime );
	void Render( void );
	rigid_body* add_body( real Mass );
	
	~simulation_world( void );


	/*----------------------------------------------------------------------------
	various forces you can add to the system
	@todo need to figure out units here so these numbers mean something
	*/


	// World Spring		
	vector_2 WorldSpringAnchor;
	static real const Kws = 3.0f;			// Hooke's spring constant
	static real const Kwd = 0.5f;	

	//BodySpring
	static real const Kbs = 1.0f;			// Hooke's spring constant
	static real const Kbd = 0.5f;			// damping constant
	

	// Damping
	static real const Kdl = 2.50f;		// linear damping factor
	static real const Kda = 140.0f;		// angular damping factor

	std::vector<rigid_body*> aBodies;
	int NumberOfBodies() { return aBodies.size(); }

private:

	enum collision_state
	{
		Penetrating,
		Colliding,
		Clear
	} CollisionState;

	vector_2 CollisionNormal;
	int CollidingBodyIndex;
	int CollidingCornerIndex;


	void ComputeForces( int ConfigurationIndex );
	void Integrate( real DeltaTime );


};

#endif
