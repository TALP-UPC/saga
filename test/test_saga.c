/*
 * test.c
 * 
 * Copyright 2016 Sergio Oller <sergioller@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * 
 */

#include "cutest.h"
#include "Saga.h"

void test_initialize_engine(void) {
	SagaEngine engine;
	SagaEngine_Initialize(&engine);
	SagaEngine_InputFromText(&engine, "hola", "ISO-8859-15");
	SagaEngine_Refresh(&engine);
	SagaEngine_Clear(&engine);
	return;
}

TEST_LIST = {
    {"Initialize engine from text", test_initialize_engine},
    {0}
};
