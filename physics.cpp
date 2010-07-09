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

physics.cpp - This file implements the 2D physics simulator.

10/15/96 - checker

*/

#include <assert.h>
#include <stdio.h>
#include <vector>
#include "physics.h"

 real const simulation_world::Kws;			// Hooke's spring constant
 real const simulation_world::Kwd;			// damping constant
 real const simulation_world::Kbs;			// Hooke's spring constant
 real const simulation_world::Kbd ;			// damping constant
 real const simulation_world::Kdl;		// linear damping factor
 real const simulation_world::Kda;		// angular damping factor


//helper func

rigid_body* simulation_world::add_body( real Mass )
{	
	rigid_body* B = new rigid_body();
	rigid_body &Body = *B;

	Body.OneOverMass = r(1) / Mass;

	// integrate over the body to find the moment of inertia

	const float Width = 100.0;  // TODO: parametrize this instead of using const values
	const float Height = 100.0;
	Body.OneOverCMMomentOfInertia = r(1) / ((Mass / r(12)) *
		(Width * Width + Height * Height));

	// 0-out non-vector quantities
	Body.aConfigurations[0].Orientation = r(0);
	Body.aConfigurations[0].AngularVelocity = r(0);
	Body.aConfigurations[0].Torque = r(0);
	aBodies.push_back(B);
	return B;
}



/*----------------------------------------------------------------------------

simulation_world ctor

*/

simulation_world::simulation_world( ) :
	SourceConfigurationIndex(0), TargetConfigurationIndex(1)
{
	WorldSpringAnchor.X = 0.0f;
	WorldSpringAnchor.Y = 0.0f;
}

/*----------------------------------------------------------------------------

simulation_world dtor

*/

simulation_world::~simulation_world( void )
{

}

/*----------------------------------------------------------------------------

Render - render the source configurations

*/

void simulation_world::Render( void )
{

	// draw bodies
	int i = 0;
	for(std::vector<rigid_body*>::iterator it = aBodies.begin(); it!=aBodies.end(); ++it, ++i)
	{
		rigid_body::configuration &config =
			(*it)->aConfigurations[SourceConfigurationIndex];
		
		printf("body %d: position: %0.4f %0.4f orientation: %0.4f \n", 
		i, config.CMPosition.X, config.CMPosition.Y, config.Orientation);
	
	}
}


/*----------------------------------------------------------------------------

Simulate - Integrate forward DeltaTime seconds.

@todo do I need to store the last simulation time?

*/

void simulation_world::Simulate( real DeltaTime )
{
	real CurrentTime = r(0);
	real TargetTime = DeltaTime;

	while(CurrentTime < DeltaTime)
	{
		ComputeForces(SourceConfigurationIndex);
		Integrate(TargetTime-CurrentTime);
		// we made a successful step, so swap configurations
		// to "save" the data for the next step
		CurrentTime = TargetTime;
		TargetTime = DeltaTime;

		SourceConfigurationIndex = SourceConfigurationIndex ? 0 : 1;
		TargetConfigurationIndex = TargetConfigurationIndex ? 0 : 1;
	}
}

/*----------------------------------------------------------------------------

ComputeForces - compute forces for gravity, spring, etc.

*/

void simulation_world::ComputeForces( int ConfigurationIndex )
{
	for(std::vector<rigid_body*>::iterator it = aBodies.begin(); it!=aBodies.end(); ++it)
	{
		rigid_body &Body = **it;
		rigid_body::configuration &Configuration = Body.aConfigurations[ConfigurationIndex];

		// clear forces
		Configuration.Torque = r(0);
		Configuration.CMForce = vector_2(r(0),r(0));
	}

	//	BodySpring
	{
		rigid_body *Body0 = aBodies.at(0);
		rigid_body::configuration &Configuration0 = Body0->aConfigurations[ConfigurationIndex];
		vector_2 Position0 = Configuration0.CMPosition + Configuration0.Orientation * vector_2( 10.0, 10.0);
		vector_2 U0 = Position0 - Configuration0.CMPosition;
		vector_2 VU0 = Configuration0.CMVelocity + Configuration0.AngularVelocity * GetPerpendicular(U0);

		rigid_body *Body1 = aBodies.at(1);
		rigid_body::configuration &Configuration1 = Body1->aConfigurations[ConfigurationIndex];
		vector_2 Position1 = Configuration1.CMPosition + Configuration1.Orientation  * vector_2( 10.0, 10.0);
		vector_2 U1 = Position1 - Configuration1.CMPosition;
		vector_2 VU1 = Configuration1.CMVelocity + Configuration1.AngularVelocity * GetPerpendicular(U1);

		// spring goes from 0 to 1

		vector_2 SpringVector = Position1 - Position0;
		vector_2 Spring = -Kbs * SpringVector;

		vector_2 RelativeVelocity = VU1 - VU0;
		// project velocity onto spring to get damping vector
		// this is basically a Gram-Schmidt projection
		vector_2 DampingForce =
			-Kbd * (DotProduct(RelativeVelocity,SpringVector)/
					DotProduct(SpringVector,SpringVector)) * SpringVector;

		Spring += DampingForce;

		Configuration0.CMForce -= Spring;
		Configuration0.Torque -= PerpDotProduct(U0,Spring);

		Configuration1.CMForce += Spring;
		Configuration1.Torque += PerpDotProduct(U1,Spring) ;
	}
 /*
	// WorldSpring 
	{
		// apply spring to body 0's vertex 0 to anchor

		rigid_body &Body = *aBodies.at(0);
		rigid_body::configuration &Configuration =
				Body.aConfigurations[ConfigurationIndex];
	
		vector_2 Position = Configuration.CMPosition + Configuration.Orientation  * vector_2( 10.0, 0.0);
		vector_2 U = Position - Configuration.CMPosition;
		vector_2 VU = Configuration.CMVelocity + Configuration.AngularVelocity * GetPerpendicular(U);

		vector_2 Spring = -Kws * (Position - WorldSpringAnchor);
		// project velocity onto spring to get damping vector
		// this is basically a Gram-Schmidt projection
		vector_2 DampingForce =
			-Kwd * (DotProduct(VU,Spring)/DotProduct(Spring,Spring)) * Spring;

		Spring += DampingForce;

		Configuration.CMForce += Spring;
		Configuration.Torque += PerpDotProduct(U,Spring);
	}
*/
}	



/*----------------------------------------------------------------------------

Integrate - integrate the rigid body configurations forward in time from
				source config to target config

@todo calculate initial derivatives _once_
@todo use something better than Euler's method

*/

void simulation_world::Integrate( real DeltaTime )
{
	for(std::vector<rigid_body*>::iterator it = aBodies.begin(); it!=aBodies.end(); ++it)
	{
		rigid_body::configuration &Source =
			(*it)->aConfigurations[SourceConfigurationIndex];
		rigid_body::configuration &Target =
			(*it)->aConfigurations[TargetConfigurationIndex];

		Target.CMPosition = Source.CMPosition +
				DeltaTime * Source.CMVelocity;

		Target.Orientation = Source.Orientation +
				DeltaTime * Source.AngularVelocity;

		Target.CMVelocity = Source.CMVelocity +
				(DeltaTime * (*it)->OneOverMass) * Source.CMForce;

		Target.AngularVelocity = Source.AngularVelocity +
				(DeltaTime * (*it)->OneOverCMMomentOfInertia) *
					Source.Torque;
	}
}




