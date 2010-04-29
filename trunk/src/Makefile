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

OUTPUT = scRamble

CC = g++

CFLAGS = -Wall -Werror -g3

LD = g++

LDFLAGS = -Wall -Werror -g3 -lGL -lGLU -lm -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

OBJECTS	=			main.o		\

LIBS	=			RenderingEngine/re.a			\


$(OUTPUT): $(OBJECTS) RE
	$(LD) -o $(OUTPUT) $(LDFLAGS) $(OBJECTS) $(LIBS)
	
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

RE:
	make -C RenderingEngine

clean:
	make -C RenderingEngine clean
	-rm -f $(OBJECTS) $(OUTPUT)

