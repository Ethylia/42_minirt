#ifndef PARSEOBJ_H
#define PARSEOBJ_H

#include "obj/scene.h"
#include "def.h"

#define ID_A_LIGHT "A"
#define ID_SAMPLES "S"
#define ID_CAMERA "C"
#define ID_SPHERE "sp"
#define ID_PLANE "pl"
#define ID_CYLINDER "cy"
#define ID_MATERIAL "m"
#define ID_TEXTURE "t"

int parseambient(char const* line, t_scene* scene);
int parsesettings(char const* line, t_scene* scene);
int parsecamera(char const* line, t_scene* scene);
int parsesphere(char const* line, t_scene* scene);
int parseplane(char const* line, t_scene* scene);
int parsecylinder(char const* line, t_scene* scene);
int parsematerial(char const* line, t_scene* scene);
int parsetexture(char const* line, t_scene* scene);

int parsecolor(char const** str, float* v);
int parsedouble(char const** str, float* v);
int parsevec(char const** str, float* v);
int parseuint(char const** str, uint* v);
int parsestring(char const** str, char** v);

#endif
