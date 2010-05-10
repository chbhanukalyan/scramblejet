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


#include "DebugGL.h"

struct capgl {
	GLint cap;
	const char *name;
};

static const struct capgl strArr[] = {
        { GL_ALPHA_TEST ,"GL_ALPHA_TEST"},                                           
        { GL_AUTO_NORMAL ,"GL_AUTO_NORMAL"},                                         
        { GL_BLEND ,"GL_BLEND"},                                                     
        { GL_CLIP_PLANE0 ,"GL_CLIP_PLANE0"},                                         
        { GL_CLIP_PLANE1 ,"GL_CLIP_PLANE1"},                                         
        { GL_CLIP_PLANE2 ,"GL_CLIP_PLANE2"},                                         
        { GL_CLIP_PLANE3 ,"GL_CLIP_PLANE3"},                                         
        { GL_COLOR_ARRAY ,"GL_COLOR_ARRAY"},                                         
        { GL_COLOR_LOGIC_OP ,"GL_COLOR_LOGIC_OP"},                                   
        { GL_COLOR_MATERIAL ,"GL_COLOR_MATERIAL"},                                   
        { GL_COLOR_SUM ,"GL_COLOR_SUM"},                                             
        { GL_COLOR_TABLE ,"GL_COLOR_TABLE"},                                         
        { GL_CONVOLUTION_1D ,"GL_CONVOLUTION_1D"},                                   
        { GL_CONVOLUTION_2D ,"GL_CONVOLUTION_2D"},                                   
        { GL_CULL_FACE ,"GL_CULL_FACE"},                                             
        { GL_DEPTH_TEST ,"GL_DEPTH_TEST"},                                           
        { GL_DITHER ,"GL_DITHER"},                                                   
        { GL_EDGE_FLAG_ARRAY ,"GL_EDGE_FLAG_ARRAY"},                                 
        { GL_FOG ,"GL_FOG"},                                                         
        { GL_FOG_COORD_ARRAY ,"GL_FOG_COORD_ARRAY"},                                 
        { GL_HISTOGRAM ,"GL_HISTOGRAM"},                                             
        { GL_INDEX_ARRAY ,"GL_INDEX_ARRAY"},                                         
        { GL_INDEX_LOGIC_OP ,"GL_INDEX_LOGIC_OP"},                                   
        { GL_LIGHT0 ,"GL_LIGHT0"},                                                   
        { GL_LIGHT1 ,"GL_LIGHT1"},                                                   
        { GL_LIGHT2 ,"GL_LIGHT2"},                                                   
        { GL_LIGHT3 ,"GL_LIGHT3"},                                                   
        { GL_LIGHT4 ,"GL_LIGHT4"},                                                   
        { GL_LIGHT5 ,"GL_LIGHT5"},                                                   
        { GL_LIGHTING ,"GL_LIGHTING"},                                               
        { GL_LINE_SMOOTH ,"GL_LINE_SMOOTH"},                                         
        { GL_LINE_STIPPLE ,"GL_LINE_STIPPLE"},                                       
        { GL_MAP1_COLOR_4 ,"GL_MAP1_COLOR_4"},                                       
        { GL_MAP1_INDEX ,"GL_MAP1_INDEX"},                                           
        { GL_MAP1_NORMAL ,"GL_MAP1_NORMAL"},                                         
        { GL_MAP1_TEXTURE_COORD_1 ,"GL_MAP1_TEXTURE_COORD_1"},                       
        { GL_MAP1_TEXTURE_COORD_2 ,"GL_MAP1_TEXTURE_COORD_2"},                       
        { GL_MAP1_TEXTURE_COORD_3 ,"GL_MAP1_TEXTURE_COORD_3"},                       
        { GL_MAP1_TEXTURE_COORD_4 ,"GL_MAP1_TEXTURE_COORD_4"},                       
        { GL_MAP2_COLOR_4 ,"GL_MAP2_COLOR_4"},                                       
        { GL_MAP2_INDEX ,"GL_MAP2_INDEX"},                                           
        { GL_MAP2_NORMAL ,"GL_MAP2_NORMAL"},                                         
        { GL_MAP2_TEXTURE_COORD_1 ,"GL_MAP2_TEXTURE_COORD_1"},                       
        { GL_MAP2_TEXTURE_COORD_2 ,"GL_MAP2_TEXTURE_COORD_2"},                       
        { GL_MAP2_TEXTURE_COORD_3 ,"GL_MAP2_TEXTURE_COORD_3"},                       
        { GL_MAP2_TEXTURE_COORD_4 ,"GL_MAP2_TEXTURE_COORD_4"},                       
        { GL_MAP2_VERTEX_3 ,"GL_MAP2_VERTEX_3"},                                     
        { GL_MAP2_VERTEX_4 ,"GL_MAP2_VERTEX_4"},
        { GL_MINMAX ,"GL_MINMAX"},
        { GL_MULTISAMPLE ,"GL_MULTISAMPLE"},
        { GL_NORMAL_ARRAY ,"GL_NORMAL_ARRAY"},
        { GL_NORMALIZE ,"GL_NORMALIZE"},
        { GL_POINT_SMOOTH ,"GL_POINT_SMOOTH"},
        { GL_POINT_SPRITE ,"GL_POINT_SPRITE"},
        { GL_POLYGON_SMOOTH ,"GL_POLYGON_SMOOTH"},
        { GL_POLYGON_OFFSET_FILL ,"GL_POLYGON_OFFSET_FILL"},
        { GL_POLYGON_OFFSET_LINE ,"GL_POLYGON_OFFSET_LINE"},
        { GL_POLYGON_OFFSET_POINT ,"GL_POLYGON_OFFSET_POINT"},
        { GL_POLYGON_STIPPLE ,"GL_POLYGON_STIPPLE"},
        { GL_POST_COLOR_MATRIX_COLOR_TABLE ,"GL_POST_COLOR_MATRIX_COLOR_TABLE"},
        { GL_POST_CONVOLUTION_COLOR_TABLE ,"GL_POST_CONVOLUTION_COLOR_TABLE"},
        { GL_RESCALE_NORMAL ,"GL_RESCALE_NORMAL"},
        { GL_SAMPLE_ALPHA_TO_COVERAGE ,"GL_SAMPLE_ALPHA_TO_COVERAGE"},
        { GL_SAMPLE_ALPHA_TO_ONE ,"GL_SAMPLE_ALPHA_TO_ONE"},
        { GL_SAMPLE_COVERAGE ,"GL_SAMPLE_COVERAGE"},
        { GL_SCISSOR_TEST ,"GL_SCISSOR_TEST"},
        { GL_SECONDARY_COLOR_ARRAY ,"GL_SECONDARY_COLOR_ARRAY"},
        { GL_SEPARABLE_2D ,"GL_SEPARABLE_2D"},
        { GL_STENCIL_TEST ,"GL_STENCIL_TEST"},
        { GL_TEXTURE_1D ,"GL_TEXTURE_1D"},
        { GL_TEXTURE_2D ,"GL_TEXTURE_2D"},
        { GL_TEXTURE_3D ,"GL_TEXTURE_3D"},
        { GL_TEXTURE_COORD_ARRAY ,"GL_TEXTURE_COORD_ARRAY"},
        { GL_TEXTURE_CUBE_MAP ,"GL_TEXTURE_CUBE_MAP"},
        { GL_TEXTURE_GEN_Q ,"GL_TEXTURE_GEN_Q"},
        { GL_TEXTURE_GEN_R ,"GL_TEXTURE_GEN_R"},
        { GL_TEXTURE_GEN_S ,"GL_TEXTURE_GEN_S"},
        { GL_TEXTURE_GEN_T ,"GL_TEXTURE_GEN_T"},
        { GL_VERTEX_ARRAY ,"GL_VERTEX_ARRAY"},
        { GL_VERTEX_PROGRAM_POINT_SIZE ,"GL_VERTEX_PROGRAM_POINT_SIZE"},
        { GL_VERTEX_PROGRAM_TWO_SIDE ,"GL_VERTEX_PROGRAM_TWO_SIDE"},
};

void DebugGL::dump_gl_state(const char *s)
{
	printf("%s {\n", s);
	for (unsigned int i = 0; i < sizeof(strArr)/sizeof(strArr[0]); i++) {
		if (glIsEnabled(strArr[i].cap))
			printf("\t%s\n", strArr[i].name);
	}
	printf("} %s\n", s);
}

