/*
 * ScRamble - 3D Flight Racer
 * Copyright (C) 2010  Bhanu Chetlapalli
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../types.h"
#include "StaticModel.h"

// assimp include files.
#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"


// the global Assimp scene object
static void recursive_render (struct aiScene *scene, struct aiScene *sc, const struct aiNode* nd);
#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)


StaticModel::StaticModel(const char *id)
	: Renderable(id)
{
	pscene_min = new aiVector3D;
	pscene_max = new aiVector3D;
	pscene_center = new aiVector3D;

	lightson = true;
}

StaticModel::~StaticModel()
{
	delete (aiVector3D*)pscene_min;
	delete (aiVector3D*)pscene_max;
	delete (aiVector3D*)pscene_center;
}

static void get_bounding_box (struct aiScene* scene, struct aiVector3D* min, struct aiVector3D* max);

int StaticModel::load(const char *fn)
{
	struct aiScene* scene = NULL;

	// we are taking one of the postprocessing presets to avoid
	// writing 20 single postprocessing flags here.
	scene = (struct aiScene*)aiImportFile(fn,aiProcessPreset_TargetRealtime_Quality);
	if (scene == NULL) {
		fprintf(stderr, "Unable to load model:%s\n", fn);
		return -1;
	}

	if (scene) {
		get_bounding_box(scene, (aiVector3D*)pscene_min, (aiVector3D*)pscene_max);
		((aiVector3D*)pscene_center)->x = (((aiVector3D*)pscene_min)->x + ((aiVector3D*)pscene_max)->x) / 2.0f;
		((aiVector3D*)pscene_center)->y = (((aiVector3D*)pscene_min)->y + ((aiVector3D*)pscene_max)->y) / 2.0f;
		((aiVector3D*)pscene_center)->z = (((aiVector3D*)pscene_min)->z + ((aiVector3D*)pscene_max)->z) / 2.0f;
	}

	strncpy(this->fn, fn, 255);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	if (lightson) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);    // Uses default lighting parameters
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	}

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

	list = glGenLists(1);
	printf("MODEL IS LIST NO: %d\n", list),
	glNewList(list, GL_COMPILE);

		// now begin at the root node of the imported data and traverse
		// the scenegraph by multiplying subsequent local transforms
		// together on GL's matrix stack.
	recursive_render(scene, scene, scene->mRootNode);
	glEndList();


	// cleanup - calling 'aiReleaseImport' is important, as the library 
	// keeps internal resources until the scene is freed again. Not 
	// doing so can cause severe resource leaking.
	aiReleaseImport(scene);

	return 0;
}

void StaticModel::render(Camera *c)
{
//	float tmp;

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	if (lightson) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);    // Uses default lighting parameters
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	}

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);

	// scale the whole asset to fit into our view frustum 
//	tmp = ((aiVector3D*)pscene_max)->x-((aiVector3D*)pscene_min)->x;
//	tmp = aisgl_max(((aiVector3D*)pscene_max)->y - ((aiVector3D*)pscene_min)->y,tmp);
//	tmp = aisgl_max(((aiVector3D*)pscene_max)->z - ((aiVector3D*)pscene_min)->z,tmp);
//	tmp = 1.f / tmp;
//	glScalef(tmp, tmp, tmp);

        // center the model
	glTranslatef( -((aiVector3D*)pscene_center)->x, -((aiVector3D*)pscene_center)->y, -((aiVector3D*)pscene_center)->z );

	glCallList(list);
	
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_NORMALIZE);

	glDisable(GL_CULL_FACE);

}


// ----------------------------------------------------------------------------
static void get_bounding_box_for_node (struct aiScene* scene, const struct aiNode* nd, 
	struct aiVector3D* min, 
	struct aiVector3D* max, 
	struct aiMatrix4x4* trafo
){
	struct aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo,&nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			struct aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp,trafo);

			min->x = aisgl_min(min->x,tmp.x);
			min->y = aisgl_min(min->y,tmp.y);
			min->z = aisgl_min(min->z,tmp.z);

			max->x = aisgl_max(max->x,tmp.x);
			max->y = aisgl_max(max->y,tmp.y);
			max->z = aisgl_max(max->z,tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		get_bounding_box_for_node(scene, nd->mChildren[n],min,max,trafo);
	}
	*trafo = prev;
}

// ----------------------------------------------------------------------------
static void get_bounding_box (struct aiScene* scene, struct aiVector3D* min, struct aiVector3D* max)
{
	struct aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z =  1e10f;
	max->x = max->y = max->z = -1e10f;
	get_bounding_box_for_node(scene, scene->mRootNode,min,max,&trafo);
}

// ----------------------------------------------------------------------------

static void color4_to_float4(const struct aiColor4D *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

static void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

// ----------------------------------------------------------------------------
static void apply_material(const struct aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	struct aiColor4D diffuse;
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
	if((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glEnable(GL_CULL_FACE);
	else 
		glDisable(GL_CULL_FACE);
}

// ----------------------------------------------------------------------------

// Can't send color down as a pointer to aiColor4D because AI colors are ABGR.
static void Color4f(const struct aiColor4D *color)
{
	glColor4f(color->r, color->g, color->b, color->a);
}

// ----------------------------------------------------------------------------
static void recursive_render (struct aiScene* scene, struct aiScene *sc, const struct aiNode* nd)
{
	int i;
	unsigned int n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = scene->mMeshes[nd->mMeshes[n]];

		apply_material(sc->mMaterials[mesh->mMaterialIndex]);

		if(mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}

		if(mesh->mColors[0] != NULL) {
			glEnable(GL_COLOR_MATERIAL);
		} else {
			glDisable(GL_COLOR_MATERIAL);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < (int)face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if(mesh->mColors[0] != NULL)
					Color4f(&mesh->mColors[0][index]);
				if(mesh->mNormals != NULL) 
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(scene, sc, nd->mChildren[n]);
	}

	glPopMatrix();
}

