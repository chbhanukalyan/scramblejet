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

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "MapLoader.h"

#include "CamPos.h"
#include "ObjInfo.h"

static void
print_element_names(xmlNode * a_node)
{
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
		if (cur_node->type == XML_ELEMENT_NODE) {
			printf("node type: Element, name: %s\n", cur_node->name);
		}
		if (xmlNodeIsText(cur_node))
			printf("cur_node->val = %s\n", xmlNodeGetContent(cur_node));


		print_element_names(cur_node->children);
	}
}

void readTriplet(xmlNode *pos, float *x, float *y, float *z)
{
	xmlChar *str = xmlNodeGetContent(pos);
	if (sscanf((const char*)str, "%f,%f,%f", x, y ,z) != 3) {
		fprintf(stderr, "Invalid Position format - %s\n", str);
	}
	xmlFree(str);
}

void readFloat(xmlNode *pos, float *f)
{
	xmlChar *str = xmlNodeGetContent(pos);
	if (sscanf((const char*)str, "%f", f) != 1) {
		fprintf(stderr, "Invalid Float format - %s\n", str);
	}
	xmlFree(str);
}

void readInt(xmlNode *pos, int *d)
{
	xmlChar *str = xmlNodeGetContent(pos);
	if (sscanf((const char*)str, "%d", d) != 1) {
		fprintf(stderr, "Invalid Int format - %s\n", str);
	}
	xmlFree(str);
}

void readStr(xmlNode *pos, char *s)
{
	xmlChar *str = xmlNodeGetContent(pos);
	strcpy(s, (const char *)str);
	xmlFree(str);
}

const char * objtypes_arr[OBJTYPE_MAX] = {
	"invalid",	/* OBJTYPE_INVALID */
	"player",	/* OBJTYPE_PLAYER */
	"static",	/* OBJTYPE_STATIC */
};

int lookupObjType(const char *str)
{
	for (int i = 0; i < OBJTYPE_MAX; i++)
		if (!strcmp(str, objtypes_arr[i]))
			return i;
	return OBJTYPE_INVALID;
}

void readObject(xmlNode *node, Map *map)
{
	if (node == NULL)
		return;

	char type[16];
	ObjInfo *o = new ObjInfo;

	node = xmlFirstElementChild(node);
	readStr(node, type);
	o->type = lookupObjType(type);

	node = xmlNextElementSibling(node);
	readInt(node, &o->id);

	node = xmlNextElementSibling(node);
	readTriplet(node, &o->locx, &o->locy, &o->locz);

	node = xmlNextElementSibling(node);
	readStr(node, o->modelfn);

	map->insertObjInfo(o);

	o->dump();
}

void readCameraPos(xmlNode *node, Map *map)
{
	CamPos *c = &map->initCamPos;

	node = xmlFirstElementChild(node);
	readTriplet(node, &c->pointx, &c->pointy, &c->pointz);

	node = xmlNextElementSibling(node);
	readTriplet(node, &c->upx, &c->upy, &c->upz);

	node = xmlNextElementSibling(node);
	readFloat(node, &c->distance);
	node = xmlNextElementSibling(node);
	readFloat(node, &c->angle);
	node = xmlNextElementSibling(node);
	readFloat(node, &c->height);

}

void readSkyBox(xmlNode *node, Map *map)
{
	node = xmlFirstElementChild(node);
	readStr(node, map->skyboxfn);
}

void readMap(xmlNode *node, Map *map)
{
	/* Read the map name */
	node = xmlFirstElementChild(node);
	readStr(node, map->name);

	/* Read the Size of the map */
	node = xmlNextElementSibling(node);
	readTriplet(node, &map->sizex, &map->sizey, &map->sizez);
	printf("Map Size = %f %f %f\n", map->sizex, map->sizey, map->sizez);

	/* Camera Position */
	node = xmlNextElementSibling(node);
	readCameraPos(node, map);

	/* Camera Position */
	node = xmlNextElementSibling(node);
	readSkyBox(node, map);

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


