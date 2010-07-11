#include "../game_object.h"
extern "C" {
#include "object.h"
}

class obj_shape : public shape 
{
public:
	~obj_shape();
	obj_shape(string *name);
	virtual void draw();
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
