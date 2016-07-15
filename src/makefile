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

include make_defs

FUENTES1	=	SagaCom.c	
HEADERS1	=	Saga.h
OBJETOS1	=	$(OBJ)/$(FUENTES1:.c=.o)

FUENTES2	=	SagaAcc.c	
HEADERS2	=	Saga.h ExcAcc.h
OBJETOS2	=	$(OBJ)/$(FUENTES2:.c=.o)

FUENTES3	=	SagaDic.c	
HEADERS3	=	Saga.h
OBJETOS3	=	$(OBJ)/$(FUENTES3:.c=.o)

FUENTES4	=	SagaExt.c	
HEADERS4	=	Saga.h
OBJETOS4	=	$(OBJ)/$(FUENTES4:.c=.o)

FUENTES5	=	SagaSil.c	
HEADERS5	=	Saga.h
OBJETOS5	=	$(OBJ)/$(FUENTES5:.c=.o)

FUENTES6	=	SagaTrn.c	
HEADERS6	=	Saga.h
OBJETOS6	=	$(OBJ)/$(FUENTES6:.c=.o)

LIB_SAGA	=	$(OBJETOS1) $(OBJETOS2) $(OBJETOS3) $(OBJETOS4) $(OBJETOS5) $(OBJETOS6)

FUENTES7	=	Saga.c
HEADERS7	=	Saga.h ExcAcc.h
OBJETOS7	=	$(OBJ)/$(FUENTES7:.c=.o)
OBJT7		=	$(OBJETOS7) $(LIB_SAGA)
LIBR7		=	$(LIBVEU)
PROG7		=	$(BIN)/$(FUENTES7:.c=)

TODO 		:	$(LIB_SAGA) $(PROG7)

Util/...	:
				@echo "---> `pwd`/$(@D) :	"
				@cd $(@D) ; $(MAKE)

include	make_todo
