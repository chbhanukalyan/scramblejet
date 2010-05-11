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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cassert>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "MapLoader.h"

#include "CamPos.h"
#include "ObjInfo.h"

static void readTriplet(xmlNode *pos, const char *name, float *x, float *y, float *z)
{
	assert(!strcmp(name, (const char*)pos->name));
	xmlChar *str = xmlNodeGetContent(pos);
	if (sscanf((const char*)str, "%f,%f,%f", x, y ,z) != 3) {
		fprintf(stderr, "Invalid Position format - %s\n", str);
	}
	xmlFree(str);
}

static void readFloat(xmlNode *pos, const char *name, float *f)
{
	assert(!strcmp(name, (const char*)pos->name));
	xmlChar *str = xmlNodeGetContent(pos);
	if (sscanf((const char*)str, "%f", f) != 1) {
		fprintf(stderr, "Invalid Float format - %s\n", str);
	}
	xmlFree(str);
}

static void readInt(xmlNode *pos, const char *name, int *d)
{
	assert(!strcmp(name, (const char*)pos->name));
	xmlChar *str = xmlNodeGetContent(pos);
	if (sscanf((const char*)str, "%d", d) != 1) {
		fprintf(stderr, "Invalid Int format - %s\n", str);
	}
	xmlFree(str);
}

static void readStr(xmlNode *pos, const char *name, char *s)
{
	assert(!strcmp(name, (const char*)pos->name));
	xmlChar *str = xmlNodeGetContent(pos);
	strcpy(s, (const char *)str);
	xmlFree(str);
}

const char * objtypes_arr[OBJTYPE_MAX] = {
	"invalid",	/* OBJTYPE_INVALID */
	"player",	/* OBJTYPE_PLAYER */
	"static",	/* OBJTYPE_STATIC */
};

static int lookupObjType(const char *str)
{
	for (int i = 0; i < OBJTYPE_MAX; i++)
		if (!strcmp(str, objtypes_arr[i]))
			return i;
	return OBJTYPE_INVALID;
}

static void readObject(xmlNode *node, Map *map)
{
	if (node == NULL)
		return;

	char type[16];
	ObjInfo *o = new ObjInfo;

	node = xmlFirstElementChild(node);
	readStr(node, "type", type);
	o->type = lookupObjType(type);

	node = xmlNextElementSibling(node);
	readInt(node, "id", &o->id);

	node = xmlNextElementSibling(node);
	readTriplet(node, "location", &o->locx, &o->locy, &o->locz);

	node = xmlNextElementSibling(node);
	readTriplet(node, "direction", &o->dirx, &o->diry, &o->dirz);

	node = xmlNextElementSibling(node);
	readStr(node, "model", o->modelfn);


	node = xmlNextElementSibling(node);
	readFloat(node, "initialvelocity", &o->initialVelocity);

	node = xmlNextElementSibling(node);
	readFloat(node, "accel", &o->accel);

	node = xmlNextElementSibling(node);
	readFloat(node, "maxspeed", &o->maxspeed);

	node = xmlNextElementSibling(node);
	readFloat(node, "minspeed", &o->minspeed);

	node = xmlNextElementSibling(node);
	readFloat(node, "minanglerot", &o->minanglerot);

	node = xmlNextElementSibling(node);
	readFloat(node, "maxanglerot", &o->maxanglerot);

	map->insertObjInfo(o);

	o->dump();
}

static void readCameraPos(xmlNode *node, Map *map)
{
	CamPos *c = &map->initCamPos;

	node = xmlFirstElementChild(node);
	readTriplet(node, "look", &c->pointx, &c->pointy, &c->pointz);

	node = xmlNextElementSibling(node);
	readTriplet(node, "up", &c->upx, &c->upy, &c->upz);

	node = xmlNextElementSibling(node);
	readFloat(node, "distance", &c->distance);
	node = xmlNextElementSibling(node);
	readFloat(node, "angle", &c->angle);
	node = xmlNextElementSibling(node);
	readFloat(node, "height", &c->height);

}

static void readTerrain(xmlNode *node, Map *map)
{
	node = xmlFirstElementChild(node);
	readStr(node, "file", map->terrainfn);
}

static void readSkyBox(xmlNode *node, Map *map)
{
	node = xmlFirstElementChild(node);
	readStr(node, "dir", map->skyboxfn);
}

static void readMap(xmlNode *node, Map *map)
{
	/* Read the map name */
	node = xmlFirstElementChild(node);
	readStr(node, "name", map->name);

	/* Read the Size of the map */
	node = xmlNextElementSibling(node);
	readTriplet(node, "size", &map->sizex, &map->sizey, &map->sizez);
	printf("Map Size = %f %f %f\n", map->sizex, map->sizey, map->sizez);

	/* Camera Position */
	node = xmlNextElementSibling(node);
	readCameraPos(node, map);

	/* Skybox Position */
	node = xmlNextElementSibling(node);
	readSkyBox(node, map);

	/* Terrain Position */
	node = xmlNextElementSibling(node);
	readTerrain(node, map);

	/* Read List of Objects */
	while (node) {
		node = xmlNextElementSibling(node);
		readObject(node, map);
	}
}

Map *loadMap(const char *fn)
{
	int err = -1;
	Map *map = NULL;
	map = new Map;

	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;

	/*
	 * this initialize the library and check potential ABI mismatches
	 * between the version it was compiled for and the actual shared
	 * library used.
	 */
	LIBXML_TEST_VERSION

	/*parse the file and get the DOM */
	doc = xmlReadFile(fn, NULL, 0);

	if (doc == NULL) {
		fprintf(stderr, "Could not parse Map file %s\n", fn);
		goto out;
	}

	strcpy(map->fn, fn);

	/*Get the root element node */
	root_element = xmlDocGetRootElement(doc);

	/* Get the name */
	readMap(root_element, map);

	err = 0;

 out:
	/*free the document */
	if (doc) xmlFreeDoc(doc);

	/*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();

	if (err) {
		delete map;
		map = NULL;
	}
	return map;
}


