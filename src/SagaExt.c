/* coding: utf-8 */
/* Saga - Un transcriptor fonético para el idioma español
 *
 * Copyright (C) 1993-2009  Albino Nogueiras Rodríguez y José B. Mariño
 *       TALP - Universitat Politècnica de Catalunya, ESPAÑA
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	"SagaInternal.h"
#include "Util.h"

/***********************************************************************
 * CogePalExt - Localiza las palabras extranhas de un texto ortografico.
 **********************************************************************/


char **CogePalExt(char *TxtOrt,
                  char **PalExt,
                  char ***DicExc,
                  char ***DicTrn,
                  char **ConsTxt, char **Vocales, char **Letras)
{
    char **tmpPalExt;
    size_t PosAct, LongTxt = strlen(TxtOrt);
    size_t Ext, NumExt;
    size_t Encontrada, Long;
    GRP_ORT GrpAct;

    if (PalExt == NULL)
    {
        if ((PalExt = (char **) malloc(sizeof(char *))) == NULL)
        {
            fprintf(stderr, "Error al ubicar memoria para PalExt\n");
            return NULL;
        }
        PalExt[0] = NULL;
    }

    NumExt = MatStrLength(PalExt);

    /*
     * Bucle para todos los grupos ortograficos.
     */
    PosAct = 0;
    while (PosAct < LongTxt)
    {
        /*
         * Cogemos el siguiente grupo ortografico de TxtOrt.
         */
        if (CogeGrpOrt(TxtOrt, PosAct, &GrpAct, Letras) < 0)
        {
            fprintf(stderr,
                    "Error al localizar el siguiente grupo de TxtOrt\n");
            LiberaMatStr(PalExt);
            return NULL;
        }

        /*
         * Comprobamos que la palabra no este ya en la lista.
         */
        Encontrada = 0;
        for (Ext = 0; Ext < NumExt; Ext++)
        {
            if ((Long = strlen(PalExt[Ext])) != GrpAct.Long)
                continue;
            if (strncmp(GrpAct.Cont, PalExt[Ext], Long) == 0)
            {
                Encontrada++;
                break;
            }
        }
        if (Encontrada > 0)
        {
            PosAct += GrpAct.Long;
            continue;
        }

        /*
         * Comprobamos que la palabra no este en alguno de los diccionarios
         */
        Encontrada = 0;
        for (Ext = 0; DicExc && DicExc[Ext]; Ext++)
        {
            if ((Long = strlen(DicExc[Ext][1])) != GrpAct.Long)
                continue;
            if (strncmp(GrpAct.Cont, DicExc[Ext][1], Long) == 0)
            {
                Encontrada++;
                break;
            }
        }
        if (Encontrada > 0)
        {
            PosAct += GrpAct.Long;
            continue;
        }
        Encontrada = 0;
        for (Ext = 0; DicTrn && DicTrn[Ext]; Ext++)
        {
            if ((Long = strlen(DicTrn[Ext][0])) != GrpAct.Long)
                continue;
            if (strncmp(GrpAct.Cont, DicTrn[Ext][0], Long) == 0)
            {
                Encontrada++;
                break;
            }
        }
        if (Encontrada > 0)
        {
            PosAct += GrpAct.Long;
            continue;
        }

        if (((GrpAct.Tipo & PALABRA) && EsPalExt(GrpAct, ConsTxt, Vocales))
            || (GrpAct.Tipo & DESCONOCIDO))
        {
            NumExt++;
            if ((tmpPalExt =
                 (char **) realloc(PalExt,
                                   (NumExt + 1) * sizeof(char *))) == NULL)
            {
                fprintf(stderr, "Error al reubicar memoria para PalExt\n");
                LiberaMatStr(PalExt);
                return NULL;
            }
            PalExt = tmpPalExt;
            if ((PalExt[NumExt - 1] =
                 calloc(GrpAct.Long + 1, sizeof(char))) == NULL)
            {
                fprintf(stderr, "Error al ubicar memoria para PalExt\n");
                LiberaMatStr(PalExt);
                return NULL;
            }
            strncpy(PalExt[NumExt - 1], GrpAct.Cont, GrpAct.Long);
            PalExt[NumExt] = NULL;
        }

        PosAct += GrpAct.Long;
    }

    return PalExt;
}

/***********************************************************************
 * EsPalExt - Determina si una palabra es extranha.
 **********************************************************************/


int EsPalExt(GRP_ORT GrpOrt, char **ConsTxt, char **Vocales)
{
    int Pos, Ultima = -1, Chr;
    int NumVocs, NumCons;
    char GrpVocs[128], GrpCons[128];

    /*
     * Todas las silabas responden a la forma [cons]*[vocs]+[cons]*.
     */
    Pos = 0;
    NumCons = 0;
    while ((Chr = IndexChr(GrpOrt.Cont + Pos, ConsTxt)) >= 0)
    {
        NumCons++;
        Pos += strlen(ConsTxt[Chr]);
        Ultima = Chr;
    }

    /*
     * A principio de palabra solo podemos tener una consonante o dos
     * siendo la segunda "l" o "r".
     */
    switch (NumCons)
    {
    case 0:
    case 1:
        break;
    case 2:
        if (!strcmp(ConsTxt[Ultima], "l") || !strcmp(ConsTxt[Ultima], "r"))
        {
            break;
        }
        else
        {
            return -1;
        }
    default:
        return -1;
    }

    while (Pos < (int) GrpOrt.Long)
    {
        if ((Chr = IndexChr(GrpOrt.Cont + Pos, InterSil)) >= 0)
        {
            Pos++;

            if (NumCons > 2)
            {
                return -1;
            }

            NumCons = 0;
            while ((Chr = IndexChr(GrpOrt.Cont + Pos, ConsTxt)) >= 0)
            {
                NumCons++;
                Pos += strlen(ConsTxt[Chr]);
                Ultima = Chr;
            }

            /*
             * A principio de silaba solo podemos tener una consonante o
             * dos siendo la segunda "l" o "r".
             */
            switch (NumCons)
            {
            case 0:
            case 1:
                break;
            case 2:
                if (!strcmp(ConsTxt[Ultima], "l") ||
                    !strcmp(ConsTxt[Ultima], "r"))
                {
                    break;
                }
                else
                {
                    return -1;
                }
            default:
                return -1;
            }

            continue;
        }

        strcpy(GrpVocs, "");
        NumVocs = 0;
        while ((Chr = IndexChr(GrpOrt.Cont + Pos, Vocales)) >= 0)
        {
            if (!strcmp(Vocales[Chr], "y") && NumVocs > 0)
                break;
            Pos += strlen(Vocales[Chr]);
            strcat(GrpVocs, Vocales[Chr]);
            NumVocs++;
        }

        /*
         * La unica palabra valida sin vocales es "y". Ninguna palabra
         * puede tener mas de cuatro vocales seguidas ni cuatro tales
         * que ninguna lleve tilde.
         */
        switch (NumVocs)
        {
        case 0:
            if (strncmp(GrpOrt.Cont, "y", GrpOrt.Long) == 0)
            {
                return 0;
            }
            else
            {
                return -1;
            }
        case 1:
        case 2:
        case 3:
            break;
        case 4:
            if (strchr(GrpVocs, '\''))
            {
                break;
            }
            else
            {
                return -1;
            }
        default:
            return -1;
        }

        NumCons = 0;
        strcpy(GrpCons, "");
        while ((Chr = IndexChr(GrpOrt.Cont + Pos, ConsTxt)) >= 0)
        {
            Pos += strlen(ConsTxt[Chr]);
            NumCons++;
            Ultima = Chr;
        }

        /*
         * Ninguna palabra puede tener mas de cuatro consonantes en su
         * interior. Si tiene cuatro, la cuarta ha ser "l" o "r".
         */
        switch (NumCons)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            break;
        case 4:
            if (!strcmp(ConsTxt[Ultima], "l") ||
                !strcmp(ConsTxt[Ultima], "r"))
            {
                break;
            }
            else
            {
                return -1;
            }
        default:
            return -1;
        }
    }

    /*
     * A final de palabra solo puede haber una consonante.
     */
    switch (NumCons)
    {
    case 0:
        break;
    case 1:
        if (!strcmp(ConsTxt[Ultima], "d") ||
            !strcmp(ConsTxt[Ultima], "h") ||
            !strcmp(ConsTxt[Ultima], "z") ||
            !strcmp(ConsTxt[Ultima], "s") ||
            !strcmp(ConsTxt[Ultima], "n") ||
            !strcmp(ConsTxt[Ultima], "l") ||
            !strcmp(ConsTxt[Ultima], "r") || !strcmp(ConsTxt[Ultima], "y"))
        {
            break;
        }
        else
        {
            return -1;
        }
    default:
        return -1;
    }

    return 0;
}
