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

OUTPUT = re.a

CC = g++

CFLAGS = -Wall -Werror -g3

AR	=	ar

OBJECTS	=			RenderingEngine.o	\
					Texture.o		\
					TextObject.o	\
					TTFFont.o		\
					Camera.o		\
					SkyBox.o		\
					StaticModel.o	\


$(OUTPUT): $(OBJECTS)
	$(AR) rcs $(OUTPUT) $(OBJECTS)

StaticModel.o: StaticModel.cpp StaticModel.h
	$(CC) $(CFLAGS) -I../../vendor/assimp/include/ -c StaticModel.cpp -o StaticModel.o
	
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f $(OBJECTS) $(OUTPUT)

