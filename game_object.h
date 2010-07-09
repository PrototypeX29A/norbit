#include "physics.h"

class shape
{
public:
	void draw();
	void set_object(Object *object);
	
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
	void set_position(float x, float y, float z);
	void DrawObject(Object *object);
	
private:
	shape *object_shape;
	rigid_body *body;
	float posx, posy, posz;
};


