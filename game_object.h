#include <string>

extern "C" 
{
#include "object.h"
}
#include "physics.h"

using namespace std;

#if !defined(GO_H)
#define GO_H

class shape
{
public:
	shape();
	virtual ~shape();
	//shape(string *name);	
	virtual void draw() = 0 ; 
//	void set_object(Object *object);
		
private:
	Object *object;
	GLuint TexId;
	int nFaces;
	Face *Faces;          /* Array of faces */
	int nVertices;
	Point *Vertices;       /* Array of vertex coordinates */
	Point *TexCoords;     /* Array of texture coordinates */
	Point *Normals;       /* Array of normal vectors */
};

class game_object
{
public:
	void draw() ;
	void set_shape(shape *shape);
	void set_rigid_body(rigid_body*);
	void set_position(float x, float y, float z);
	void DrawObject(Object *object) ;
	
	static void set_simulation_world(simulation_world *w);
	void apply_force(vector_2 const & F, vector_2 const & Pl);
	float posx();
	float posy();
	float posz();

private:
	shape *object_shape;
	static simulation_world * world;
	rigid_body *body;
};

	

#endif
