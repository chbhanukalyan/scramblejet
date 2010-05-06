# ScRamble - 3D Flight Racer
# Copyright (C) 2009  Bhanu Chetlapalli
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

include ../cmds.mk

OUTPUT = scRamble

LDFLAGS := $(LDFLAGS) -Wall -Werror -g3 -lGL -lGLU -lm -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lxml2
#/usr/lib/libxml2.so

OBJECTS	=			main.o		\

LIBS	=			\
					Game/game.a						\
					Objects/objs.a					\
					RenderingEngine/re.a			\
					GamingClient/gc.a				\
					../Map/map.a						\
					../../vendor/assimp/bin/gcc/libassimp.so	\


$(OUTPUT): $(OBJECTS) RE GC OBJ MAP GAME
	$(LD) -o $(OUTPUT) $(LDFLAGS) $(OBJECTS) $(LIBS)
	
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

OBJ:
	make -C Objects

GAME:
	make -C Game

GC:
	make -C GamingClient

RE:
	make -C RenderingEngine

MAP:
	make -C ../Map

clean:
	make -C RenderingEngine clean
	make -C GamingClient clean
	make -C Game clean
	make -C Objects clean
	make -C ../Map clean
	-rm -f $(OBJECTS) $(OUTPUT)

