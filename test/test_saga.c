/*
 * test.c
 * 
 * Copyright (C) 1993-2017  Albino Nogueiras Rodríguez y José B. Mariño
 *       TALP - Universitat Politècnica de Catalunya, ESPAÑA
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
/* Author: Sergio Oller, 2017 */
#include <stdlib.h>
#include <string.h>
#include "Saga.h"
#include "Util.h"
#include "LisUdf.h"

#ifndef TEST_FILE_DIR
#define TEST_FILE_DIR ""
#endif

#ifndef TEST_READLISUDF_INPUT
#define TEST_READLISUDF_INPUT TEST_FILE_DIR "test_readlisudf.txt"
#endif

int test_lisudf(void)
{
    char **lisudf;
    size_t tamlis;
    int err;
    tamlis = ReadLisUdf(TEST_READLISUDF_INPUT, &lisudf);
    if (tamlis != 4)
    {
			fprintf(stderr, "Could not read list\n");
			return -1;
		}
		err = MeteLisUdf("raxacoricofallapatorius", &tamlis, &lisudf);
		if (err == LIS_UDF_ERROR)
		{
			fprintf(stderr, "Could not append to LisUdf\n");
			return -1;
		}
		tamlis = SeekLisUdf("raxacoricofallapatorius", tamlis, lisudf);
		if (tamlis != 4)
		{
			fprintf(stderr, "LisUdf seek failed\n");
			return -1;
		}
    return 0;
}

int test_matstr(void)
{
    char **matstr = NULL;
    matstr = MatStr(NULL);
    if (matstr != NULL)
		{
			fprintf(stderr, "MatStr(NULL) failed\n");
			return -1;
		}
    LiberaMatStr(matstr);
    matstr = MatStr("hola amigos,estoy contento");
    if (matstr == NULL)
		{
			fprintf(stderr, "Error creating MatStr from text\n");
			return -1;
		}
    if (MatStrLength(matstr) != 4)
    {
			fprintf(stderr,  "Test MatStrLength failed\n");
			return -1;
		}
    LiberaMatStr(matstr);
    return 0;
}

int test_initialize_engine(void)
{
    SagaEngine engine;
    int err;
    err = SagaEngine_Initialize(&engine);
    if (err < 0)
    {
			  fprintf(stderr, "Error in SagaEngine_Initialize\n");
			  return -1;
		}
    err = SagaEngine_InputFromText(&engine, "hola", "UTF-8");
    if (err < 0)
    {
			  fprintf(stderr, "Error using InputFromText\n");
			  return -1;
		}
    err = SagaEngine_Refresh(&engine);
    if (err < 0)
    {
			  fprintf(stderr, "Error Refreshing engine\n");
			  return -1;
		}
    err = SagaEngine_Clear(&engine);
    if (err < 0)
    {
			  fprintf(stderr, "Error Clearing engine\n");
			  return -1;
		}
    return 0;
}

int test_empty()
{
    const char *text = "";
    SagaEngine engine;
    int err;
    err = SagaEngine_Initialize(&engine);
    if (err < 0)
    {
			  fprintf(stderr, "Error in SagaEngine_Initialize\n");
			  return -1;
	}
    err = SagaEngine_Prepare(&engine);
    if (err < 0)
    {
			  fprintf(stderr, "Error in SagaEngine_Prepare\n");
			  return -1;
	}
    SagaEngine_EnableFnmPalOutput(&engine, 1);
    if (SagaEngine_TranscribeText(&engine, text, "UTF-8") < 0)
    {
        SagaEngine_Clear(&engine);
        return -1;
    }
    if (engine.StreamOutputs.fnmpal == NULL)
    {
        fprintf(stderr, "fnmpal should be empty but it is NULL\n");
        return -1;
    }
    SagaEngine_Clear(&engine);
    return 0;
}

int test_emoji()
{
    const char *text = "hola 😀 mundo";
    SagaEngine engine;
    int err;
    err = SagaEngine_Initialize(&engine);
    if (err < 0)
    {
			  fprintf(stderr, "Error in SagaEngine_Initialize\n");
			  return -1;
	}
    err = SagaEngine_Prepare(&engine);
    if (err < 0)
    {
			  fprintf(stderr, "Error in SagaEngine_Prepare\n");
			  return -1;
	}
    SagaEngine_EnableFnmPalOutput(&engine, 1);
    if (SagaEngine_TranscribeText(&engine, text, "UTF-8") < 0)
    {
        SagaEngine_Clear(&engine);
        return -1;
    }
    SagaEngine_Clear(&engine);
    return 0;
}

int test_utf8_to_latin9()
{
    char output[20];
    char input[] = "hola 😀 mundo";
    utf8_to_latin9(output, input, strlen(input));
    if (strcmp("hola  mundo", output) != 0)
    {
        return -1;
        }
    return 0;
}
int main()
{
    int ret = 0;
	if (test_initialize_engine() < 0)
	{
        printf("Test initialize engine failed\n");
        ret = -1;
    }
	if (test_matstr() < 0)
	{ 
        printf("Test matstr failed\n");
        ret = -1;
    }
	if (test_lisudf() < 0)
	{ 
        printf("Test lisudf failed\n");
        ret = -1;
    }
	if (test_utf8_to_latin9() < 0)
	{ 
        printf("Test utf8_to_latin9 failed\n");
        ret = -1;
    }
	if (test_empty() < 0)
	{ 
        printf("Test empty failed\n");
        ret = -1;
    }
	if (test_emoji() < 0)
	{ 
        printf("Test emoji failed\n");
        ret = -1;
    }
	return ret;
}

