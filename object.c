/* GL_shadow: an implementation of the stencil shadow volume technique
 * Copyright (C) 2005 Angelo "Encelo" Theodorou
 * 
 * object.c: simple and DIRTY Wavefront OBJ loader
 */   


#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include "object.h"


/* Loads a 3D object in OBJ format */
Object *LoadObject(char *objfile)
{
	FILE *fd;
	Object *obj;
	char buf[128];
	
	char namebuf[128];
	int nv, nt, nn, nf;
	char texbuf[128];
	char matfile[128];
	char material[128], mat[128];
	Point *vertices, *texcoords, *normals;
	int fv[3], ft[3], fn[3];
	int i;
	int mat_flag = 0;
	
	
/* ----- Processing the .obj file ----- */
	fd = fopen(objfile, "r");
	if (fd == NULL) {
		printf("Unable to open %s\n", objfile);
		return NULL;
	}

	obj = (Object *)malloc(sizeof(Object));

	nv = nt = nn = nf = 0;
	/* Element count */
	while(fgets(buf, 128, fd) != NULL) {
		switch(buf[0]) {
			case 'm':
				if (strncmp(buf, "mtllib", 6) == 0) {
					sscanf(buf, "mtllib %s\n", matfile);
				}
			case 'o':
				sscanf(buf, "o %s\n", namebuf);
				break;
			case 'v':
				switch(buf[1]) {
					case ' ':
						nv++;
						break;
					/* Number of texcoords and normal vectors is equal to 3 times the number of faces */
					/* This is true only for Blender 2.37a OBJ importer script, not for 2.40 one */
				}
				break;
			case 'u':
				if (strncmp(buf, "usemat", 6) == 0) {
					sscanf(buf, "usemat %s\n", texbuf);
				}
				if (strncmp(buf, "usemtl", 6) == 0) {
					sscanf(buf, "usemtl %s\n", material);
				}
				break;
			case 'f':
				nf++;
				break;
		}
	}

	
/* ----- Object initialization ----- */
	/* Name */
	obj->Name = (char*) malloc(sizeof(char) * strlen(namebuf));
	strncpy(obj->Name, namebuf, strlen(namebuf));
	(obj->Name)[strlen(namebuf)] = '\0'; /* Terminating the string, '\n' becomes '\0' */
	/* Texture */
	obj->TexId = LoadTexture(texbuf);
	/* nFaces */
	obj->nFaces = nf;
	/* Faces */
	obj->Faces = (Face*)malloc(sizeof(Face) * nf);
	/* nVertices */
	obj->nVertices = nv;
	/* Vertices */
	obj->Vertices = (Point *)malloc(sizeof(Point) * nv);
	/* TexCoords */
	obj->TexCoords = (Point *)malloc(sizeof(Point) * 3 * nf);
	/* Normals */
	obj->Normals = (Point *)malloc(sizeof(Point) * 3 * nf);

	
	rewind(fd);
	nv = nt = nn = nf = 0;
	while(fgets(buf, 128, fd) != NULL) {
		switch(buf[0]) {
			case 'v':
				switch(buf[1]) {
					case ' ':
						sscanf(buf, "v %f %f %f\n", &obj->Vertices[nv].x, &obj->Vertices[nv].y, &obj->Vertices[nv].z);
						nv++;
						break;
					case 't':
						sscanf(buf, "vt %f %f %f\n", &obj->TexCoords[nt].x, &obj->TexCoords[nt].y, &obj->TexCoords[nt].z);
						nt++;
						break;
					case 'n':
						sscanf(buf, "vn %f %f %f\n", &obj->Normals[nn].x,  &obj->Normals[nn].y,  &obj->Normals[nn].z);
						nn++;
						break;
				}
				break;
			case 'f':
				sscanf(buf, "f %u/%u/%u %u/%u/%u %u/%u/%u \n", &fv[0], &ft[0], &fn[0],
					&fv[1], &ft[1], &fn[1],
					&fv[2], &ft[2], &fn[2]);
				for(i = 0; i < 3; i++) {
					fv[i]--; ft[i]--; fn[i]--; /* Face indices starts from 1 in Obj format */
					obj->Faces[nf].VertIdx[i] = fv[i];
					obj->Faces[nf].TexIdx[i] = ft[i];
					obj->Faces[nf].NorIdx[i] = fn[i];
					obj->Faces[nf].NeighIdx[i] = -1; /* Connectivity will be calculated later */
				}
				nf++;
				break;
		}
	}

	fclose(fd);

	
/* ----- Processing the .mtl file ----- */
	fd = fopen(matfile, "r");
	if (fd == NULL) {
		printf("Unable to open %s\n", matfile);
		return NULL;
	}

	while(fgets(buf, 128, fd) != NULL) {
		/* Search the correct material */
		if (strncmp(buf, "newmtl", 6) == 0) {
			sscanf(buf, "newmtl %s", mat);
			if (strncmp(mat, material, strlen(material)) == 0) {
				mat_flag = 1;
				continue;
			}
			else {
				mat_flag = 0;
				continue;
			}
		}

	if (mat_flag == 1)
		switch(buf[0]){
			case 'K':
				switch(buf[1]){
					case 'd':
						sscanf(buf, "Kd %f %f %f\n", &obj->Diffuse[0], &obj->Diffuse[1], &obj->Diffuse[2]);
						break;
					case 'a':
						sscanf(buf, "Ka %f %f %f\n", &obj->Ambient[0], &obj->Ambient[1], &obj->Ambient[2]);
						break;
					case 's':
						sscanf(buf, "Ks %f %f %f\n", &obj->Specular[0], &obj->Specular[1], &obj->Specular[2]);
						break;
				}
				break;
			case 'd':
				sscanf(buf, "d %f\n", &obj->Diffuse[3]);
				obj->Ambient[3] = obj->Specular[3] = obj->Diffuse[3];
				break;
		}
	}

	return obj;
}


/* Simple code to load a texture and return its id */
GLuint LoadTexture(char *tex_name)
{
	GLuint tex_num;
		
	SDL_Surface *tex_img;
	glGenTextures(1, &tex_num);

	if(tex_img = (SDL_Surface *) ::IMG_Load(tex_name)) {
        glBindTexture(GL_TEXTURE_2D, tex_num);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		if ((tex_img->format)->Amask)
	        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_img->w, tex_img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_img->pixels);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_img->w, tex_img->h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_img->pixels);
        SDL_FreeSurface (tex_img);
    }
	else
		printf("Texture not found!\n");
	printf("Texture loaded %s!\n", tex_name);
	return tex_num;
}


/* Edge connectivity */
void SetConnectivity(Object *obj)
{
	int nf1, nf2;
   	int e1, e2;
	int v1a, v1b, v2a, v2b;
	Face *face1, *face2;
	
	for(nf1=0; nf1 < obj->nFaces; nf1++){ /* For each face in the object */
		face1 = &obj->Faces[nf1];
		for(e1=0; e1<3; e1++){ /* For each edge of the face */
			if(face1->NeighIdx[e1] == -1){ /* If connectivity is not yet set for this edge */
				for(nf2=0; nf2 < obj->nFaces; nf2++){ /* Checking connectivity against any other face */
					face2 = &obj->Faces[nf2];
					if (nf2 == nf1) /* Skipping checking connectivity against the same face we are checking on */
						continue;
					for(e2=0; e2<3; e2++){
						v1a = face1->VertIdx[e1];
						v1b = face1->VertIdx[(e1+1)%3];
						v2a = face2->VertIdx[e2];
						v2b = face2->VertIdx[(e2+1)%3];
						if((v1a == v1b && v2a == v2b) || (v1a == v2b && v2a == v1b)){
							face1->NeighIdx[e1] = nf2;
							face2->NeighIdx[e2] = nf1;
							break;
						}
					}
				}
			}
		}
	}
}


/* Calculates the equation of the plane on which a face is laying on */
void CalculatePlane(Object *obj, int nf)
{
	Point v1, v2, v3;
	Face *face;

	face = &obj->Faces[nf];
	v1 = obj->Vertices[face->VertIdx[0]];
	v2 = obj->Vertices[face->VertIdx[1]];
	v3 = obj->Vertices[face->VertIdx[2]];
	
	face->pEq.a = v1.y*(v2.z-v3.z) + v2.y*(v3.z-v1.z) + v3.y*(v1.z-v2.z);
	face->pEq.b = v1.z*(v2.x-v3.x) + v2.z*(v3.x-v1.x) + v3.z*(v1.x-v2.x);
	face->pEq.c = v1.x*(v2.y-v3.y) + v2.x*(v3.y-v1.y) + v3.x*(v1.y-v2.y);
	face->pEq.d = -(v1.x*(v2.y*v3.z-v3.y*v2.z)
		+ v2.x*(v3.y*v1.z-v1.y*v3.z)
		+ v3.x*(v1.y*v2.z-v2.y*v1.z));
}


Object *InitObject(char *objfile)
{
	int nf;
	Object *obj;
		
	if((obj = LoadObject(objfile)) != NULL){
		SetConnectivity(obj);

		for(nf=0; nf < obj->nFaces; nf++)
			CalculatePlane(obj, nf);
	}

	return obj;
}


void FreeObject(Object *obj)
{
	glDeleteTextures(1, &(obj->TexId));
	free(obj->Normals);
	free(obj->TexCoords);
	free(obj->Vertices);
	free(obj->Faces);
	free(obj->Name);
}


void DrawObject(Object *obj)
{
	int nf, i;
	Face *face;
	Point *n, *t, *v;
	
	glBindTexture(GL_TEXTURE_2D, obj->TexId);
	glBegin(GL_TRIANGLES);
	for(nf=0; nf < obj->nFaces; nf++) {
		face = &obj->Faces[nf];
		for(i=0; i<3; i++) {
			n = &obj->Normals[face->NorIdx[i]];
			t = &obj->TexCoords[face->TexIdx[i]];
			v = &obj->Vertices[face->VertIdx[i]];
			
			glNormal3f(n->x, n->y, n->z);
			glTexCoord3f(t->x, t->y, t->z);
			glVertex3f(v->x, v->y, v->z);
		}
	}
	glEnd();
}
