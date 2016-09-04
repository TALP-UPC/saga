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
#include "Util.h"
#include "LisUdf.h"

#ifndef TEST_FILE_DIR
#define TEST_FILE_DIR ""
#endif

#ifndef TEST_READLISUDF_INPUT
#define TEST_READLISUDF_INPUT TEST_FILE_DIR "test_readlisudf.txt"
#endif

void test_lisudf(void)
{
	char **lisudf;
	size_t tamlis;
	TEST_CHECK_((tamlis = ReadLisUdf(TEST_READLISUDF_INPUT, &lisudf)) == 4,
	            "Could not read list");
	TEST_CHECK_(MeteLisUdf("raxacoricofallapatorius", &tamlis, &lisudf) != LIS_UDF_ERROR,
	            "Could not append to LisUdf");
	TEST_CHECK_(SeekLisUdf("raxacoricofallapatorius", tamlis, lisudf) == 4,
	            "LisUdf seek failed");
	return;
}

void test_matstr(void)
{
  char **matstr = NULL;
  matstr = MatStr(NULL);
  TEST_CHECK_(matstr == NULL, "MatStr(NULL) failed");
  LiberaMatStr(matstr);
  matstr = MatStr("hola amigos,estoy contento");
  TEST_CHECK_(matstr != NULL, "Error creating MatStr from text");
  TEST_CHECK_(MatStrLength(matstr) == 4, "Test MatStrLength failed");
  LiberaMatStr(matstr);
}

void test_initialize_engine(void) {
	SagaEngine engine;
	TEST_CHECK_(SagaEngine_Initialize(&engine) >= 0,
                "Error in SagaEngine_Initialize");
	TEST_CHECK_(
        SagaEngine_InputFromText(&engine, "hola", "ISO-8859-15") >= 0,
        "Error using InputFromText");
	TEST_CHECK_(SagaEngine_Refresh(&engine) >= 0, "Error Refreshing engine");
	TEST_CHECK_(SagaEngine_Clear(&engine) >= 0,"Error Clearing engine");
	return;
}



TEST_LIST = {
    {"Initialize engine from text", test_initialize_engine},
    {"Test Matrix of strings helpers", test_matstr},
    {"Test ListUdf helpers", test_lisudf},
    {0}
};
