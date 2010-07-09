#include <string>

#include "object.h"
#include "physics.h"
#include "object.h"

using namespace std;

class shape
{
public:
	shape();
	virtual ~shape();
	//shape(string *name);	
	virtual void draw() = 0;
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
	void draw();
	void set_shape(shape *shape);
	void set_rigid_body(rigid_body*);
	void set_position(float x, float y, float z);
	void DrawObject(Object *object);
	
	static void set_simulation_world(simulation_world *w);
	void apply_force(vector_2& F, vector_2& Pl);

private:
	shape *object_shape;
	static simulation_world * world;
	rigid_body *body;
	//float posx, posy, posz;
	float posx();
	float posy();
	float posz();
};


