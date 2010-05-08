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

OUTPUT = scRambleSrv

LDFLAGS := $(LDFLAGS) -lm -lxml2

OBJECTS	=			main.o		\

LIBS	=			\
					GamingEngine/ge.a			\
					../Map/map.a				\
					Objects/objs.a				\
					GamingServer/gs.a			\

$(OUTPUT): $(OBJECTS) GE GS OBJ MAP
	$(LD) -o $(OUTPUT) $(LDFLAGS) $(OBJECTS) $(LIBS)
	
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

OBJ:
	make -C Objects

GE:
	make -C GamingEngine

GS:
	make -C GamingServer

MAP:
	make -C ../Map

clean:
	make -C GamingEngine clean
	make -C GamingServer clean
	make -C Objects clean
	make -C ../Map clean
	-rm -f $(OBJECTS) $(OUTPUT)

