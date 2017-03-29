/*
 * saga_example.c
 * 
 * Copyright 2017 Sergio Oller <sergioller@gmail.com>
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

#include <stdio.h>
#include <stdlib.h>

#include "Saga.h"

int main()
{
    /* Input text */
    const char texto[] = "hola mundo";
    /* Placeholders for all possible outputs: They will be allocated as needed. We must free them */

    /* Load engine */
    SagaEngine *engine = SagaEngine_NewFromVariant("castilla");
    if (engine == NULL)
    {
        return EXIT_FAILURE;
    }

    SagaEngine_EnableFonOutput(engine, 1);
    SagaEngine_EnableFnmOutput(engine, 1);
    SagaEngine_EnableFnmPalOutput(engine, 1);
    SagaEngine_EnableSefoOutput(engine, 1);
    SagaEngine_EnableSemOutput(engine, 1);

    /* Transcribe the text */
    if (SagaEngine_TranscribeText(engine, texto, "UTF-8") < 0)
    {
        SagaEngine_Clear(engine);
        free(engine);
        printf("Error in transcribe text\n");
        return EXIT_FAILURE;
    }
    /* Print all outputs */
    /* Phonemes: 'o-la m'un-do */
    printf("Phonemes: %s\n", SagaEngine_GetFonOutput(engine, 0));
    /* Phonemes separated:  'o l a m 'u n d o  */
    printf("Phonemes separated: %s\n", SagaEngine_GetFnmOutput(engine, 0));
    /* Phonemes separated and separating words: 'o - l a / m 'u n - d o */
    printf("Phonemes separated and separating words: %s\n",
           SagaEngine_GetFnmPalOutput(engine, 0));
    /* Semisyllables: o- -'o la- -a mu- -'un do- -o  */
    printf("Semisyllables: %s\n", SagaEngine_GetFnmPalOutput(engine, 0));
    /* Semiphonemes: .-'o 'o+l 'o-l l+a l-a a+. .-m m+'u m-'u 'u+n 'u-n n+d n-d d+o d-o o+. */
    printf("Semiphonemes: %s\n", SagaEngine_GetSemOutput(engine, 0));
    fflush(stdout);
    /* Clear the engine, prepare it for the next transcription */
    SagaEngine_Refresh(engine);

    /* Another text to transcribe */
    const char texto2[] = "hola mundo me gusta la carne";
    /* Now we just want one output, we disable the other four */
    SagaEngine_EnableFonOutput(engine, 0);
    SagaEngine_EnableFnmOutput(engine, 0);
    SagaEngine_ClearFnmPalOutput(engine);
    SagaEngine_EnableSefoOutput(engine, 0);
    SagaEngine_EnableSemOutput(engine, 0);

    if (SagaEngine_TranscribeText(engine, texto2, "UTF-8") < 0)
    {
        SagaEngine_Clear(engine);
        free(engine);
        printf("Error in transcribe text\n");
        return EXIT_FAILURE;
    }
    printf("%s\n", SagaEngine_GetFnmPalOutput(engine, 0));
    /* 'o - l a / m 'u n - d o / m e / G 'u s - t a / l a / k 'a r - n e */
    /* Final clear */
    SagaEngine_Refresh(engine);
    SagaEngine_Clear(engine);
    free(engine);
    return EXIT_SUCCESS;
}
