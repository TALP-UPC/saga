# Saga - Un transcriptor fonético para el idioma español
#
# Copyright (C) 1993-2009  Albino Nogueiras Rodríguez y José B. Mariño
#        TALP - Universitat Politècnica de Catalunya, ESPAÑA
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

OBJ 		=	$(PWD)/obj
LIB 		=	$(PWD)/lib
BIN 		=	$(PWD)/bin

TODO 		:	$(OBJ) $(LIB) $(BIN) Util/... src/...

Util/...	:
				@echo "---> `pwd`/$(@D) :	"
				@cd $(@D) ; $(MAKE)

src/...	:
				@echo "---> `pwd`/$(@D) :	"
				@cd $(@D) ; $(MAKE)

$(OBJ)	:
				mkdir -p $(OBJ)

$(LIB)	:
				mkdir -p $(LIB)

$(BIN)	:
				mkdir -p $(BIN)

