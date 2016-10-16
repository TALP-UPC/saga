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
#include	"ExcAcc.h"
#include "Util.h"

/***********************************************************************
 * AcenSilOrt - Acentua un texto silabificado.
 **********************************************************************/


char	*AcenSilOrt(char	*SilOrt, char	***DicTrnPal, SagaEngine *engine)

{
	char	*AccSil, *AcenPal;
	int		PosAct, LongTxt = strlen(SilOrt), Long;
	GRP_ORT	GrpAct;
	int		Encontrada, i;

	/*
	 * Ubicamos memoria para albergar el doble mas uno de la longitud
	 * de SilOrt. Esto garantiza que cabra cualquier texto acentuado. No
	 * es muy elegante, pero si es eficiente.
	 */
	if ((AccSil = (char *) calloc((size_t) (20*LongTxt+1), sizeof(char))) == (char *) 0) {
		fprintf(stderr, "Error al ubicar memoria para AccSil\n");
		return (char *) 0;
	}

	/*
	 * Inicializamos AccSil a "" para poder utilizar strcat's...
	 */
	strcpy(AccSil, "");
	
	/*
	 * Bucle para todos los grupos ortograficos.
	 */
	PosAct = 0;
	while (PosAct < LongTxt) {
		/*
		 * Comprobamos si se trata de una excepcion.
		 */
		if ((Long = EsExcAcc(SilOrt, PosAct, ExcAcc, engine->Letras)) > 0) {
			strncat(AccSil, SilOrt+PosAct, Long);
			PosAct += Long;
			continue;
		}

		/*
		 * Cogemos el siguiente grupo ortografico de SilOrt.
		 */
		if (CogeGrpOrt(SilOrt, PosAct, &GrpAct, engine->Letras) < 0) {
			fprintf(stderr, "Error al localizar el siguiente grupo de SilOrt\n");
			free(AccSil);
			return (char *) 0;
		}

		Encontrada = 0;
		for (i = 0; DicTrnPal != (char ***) 0 && DicTrnPal[i] != (char **) 0; i++) {
			if (strncmp(DicTrnPal[i][0], GrpAct.Cont, GrpAct.Long) == 0) {
				Encontrada = 1;
				strncat(AccSil, GrpAct.Cont, GrpAct.Long);
				break;
			}
		}
				
		if (!Encontrada && (GrpAct.Tipo & PALABRA)) {
			if ((AcenPal = AcenPalSil(GrpAct, engine)) == (char *) 0) {
				strncpy(AccSil, GrpAct.Cont, GrpAct.Long);
				AccSil[GrpAct.Long] = '\0';
				fprintf(stderr, "Error al acentuar %s\n", AccSil);
				free(AccSil);
				return (char *) 0;
			}
			else {
				strcat(AccSil, AcenPal);
				free(AcenPal);
			}
		}
		else if (!Encontrada) {
			strncat(AccSil, GrpAct.Cont, GrpAct.Long);
		}
		PosAct += GrpAct.Long;
	}

	AccSil = realloc(AccSil, strlen(AccSil) + 1);

	return AccSil;
}

/***********************************************************************
 * AcenPalSil - Acentua una palabra silabificada.
 **********************************************************************/


char	*AcenPalSil(GRP_ORT	GrpOrt, SagaEngine *engine)

{
	char	*AcenPal, *PalSil;
	char *tmp;
	char *UltSil = NULL;
	size_t UltSilSize = 0, UltSilFilled = 0;
	char *PenSil = NULL;
	size_t PenSilSize = 0, PenSilFilled = 0;
	int		Pos, PosUltSil, PosPenSil;
	int		Long = GrpOrt.Long, Mente, LongPalSil;

	/*
	 * Ubicamos memoria para albergar el doble mas uno de la longitud de
	 * GrpOrt.Long. Esto garantiza que cabra cualquier silabificacion.
	 * No es muy elegante, pero si es eficiente.
	 */
	if ((AcenPal = (char *) calloc((size_t) (20*Long+1), sizeof(char))) == (char *) 0) {
		fprintf(stderr, "Error al ubicar memoria para AcenPal\n");
		return NULL;
	}
	if ((PalSil = (char *) calloc((size_t) (20*Long+1), sizeof(char))) == (char *) 0) {
		fprintf(stderr, "Error al ubicar memoria para PalSil\n");
		free(AcenPal);
		return NULL;
	}

	UltSilSize = 128;
	if ((UltSil = calloc(UltSilSize, sizeof(char))) == NULL) {
		fprintf(stderr, "Error al ubicar memoria para UltSil\n");
		free(AcenPal);
		free(PalSil);
		return NULL;
	}

	PenSilSize = 128;
	if ((PenSil = calloc(PenSilSize, sizeof(char))) == NULL) {
		fprintf(stderr, "Error al ubicar memoria para PenSil\n");
		free(AcenPal);
		free(PalSil);
		free(UltSil);
		return NULL;
	}


	strncpy(PalSil, GrpOrt.Cont, Long);
	PalSil[Long] = '\0';

	/*
	 * Las reglas de acentuacion son las mismas si acaba en mente.
	 */
	Mente = 0;
	Pos = Long - strlen("-men-te");
	if ((Pos>0) && !strcmp(PalSil+Pos, "-men-te") && ((int) strcspn(PalSil, "-") < Pos)) {
		Mente = 1;
		PalSil[Pos] = '\0';
		Long = Pos;
	}

	/*
	 * Si ya hay acento, ya lo hay.
	 */
	if (strchr(PalSil, '\'') != (char *) 0) {
		strcat(AcenPal, PalSil);
	}
	else if (strchr(PalSil, '-') == (char *) 0) {
		if (AcenSil(PalSil, engine) < 0) {
			free(AcenPal);
			free(PalSil);
			free(UltSil);
			free(PenSil);
			return (char *) 0;
		}
		strcat(AcenPal, PalSil);
	}
	else {
		/*
		 * Determinamos la posicion de la ultima y penultima silabas.
		 */
		for (PosUltSil = strlen(PalSil); PalSil[PosUltSil-1] != '-'; PosUltSil--) ;
		for (PosPenSil = PosUltSil-1; PosPenSil > 0 && PalSil[PosPenSil-1] != '-'; PosPenSil--) ;

		UltSilFilled = 0;
		if (safe_strcat(&UltSil, &PalSil[PosUltSil], &UltSilSize, &UltSilFilled) < 0) {
			free(AcenPal);
			free(PalSil);
			free(UltSil);
			free(PenSil);
			return NULL;
		}
		PenSilFilled = 0;
		if (safe_strcat(&PenSil, &PalSil[PosPenSil], &PenSilSize, &PenSilFilled) < 0) {
			free(AcenPal);
			free(PalSil);
			free(UltSil);
			free(PenSil);
			return NULL;
		}
		for (Pos = 0; Pos < (int) strlen(PenSil); Pos++) {
			if (PenSil[Pos] == '-') PenSil[Pos] = '\0';
		}

		/*
		 * Lo anterior a la penultima silaba puede ser copiado integro.
		 */
		strcat(AcenPal, PalSil);
		AcenPal[PosPenSil] = '\0';

		/*
		 * si la ultima letra de la palabra es vocal, n o s, la palabra
		 * es llana.
		 */
		LongPalSil = strlen(PalSil);
		if (((LongPalSil > 0) && strchr("aeiouns", PalSil[LongPalSil-1]) != (char *) 0) ||
			((LongPalSil > 1) && PalSil[LongPalSil-1] == 'y' && !strchr("aeiou", PalSil[LongPalSil-2]))) {
			if (AcenSil(PenSil, engine) < 0) {
				free(AcenPal);
				free(PalSil);
				free(UltSil);
				free(PenSil);
				return (char *) 0;
			}
		}
		else {
			if (AcenSil(UltSil, engine) < 0) {
				free(AcenPal);
				free(PalSil);
				free(UltSil);
				free(PenSil);
				return (char *) 0;
			}
		}
		strcat(AcenPal, PenSil);
		strcat(AcenPal, InterSil[0]);
		strcat(AcenPal, UltSil);
	}

	if (Mente != 0) {
		strcat(AcenPal, "-m'en-te");
	}

	free(PalSil);
	free(UltSil);
	free(PenSil);

  tmp = realloc(AcenPal, strlen(AcenPal) + 1);
  if (tmp == NULL) {
		return AcenPal;
	} else {
		AcenPal = tmp;
	}
	return AcenPal;
}

/***********************************************************************
 * AcenSil - Determina el nucleo silabico de una silaba y le coloca
 * la tilde.
 **********************************************************************/


int		AcenSil(char *Sil, SagaEngine *engine)
{
	int		Ind, Pos, PosAcc;
	int		Chr, Chr1, Chr2;
	int		NumVocs;
	
	/*
	 * Todas las silabas responden a la forma [cons]*[vocs]+[cons]*.
	 */
	Pos = 0;
	while ((Chr = IndexChr(Sil+Pos, engine->ConsTxt)) >= 0) {
		Pos += strlen(engine->ConsTxt[Chr]);

		if (strcmp(engine->ConsTxt[Chr], "g") == 0) {
			Ind = Pos;
			if ((Chr1 = IndexChr(Sil+Ind, engine->Letras)) < 0) {
				return -1;
			}
			Ind += strlen(engine->Letras[Chr1]);
			Chr2 = IndexChr(Sil+Ind, engine->Letras);
			if (strcmp(engine->Letras[Chr1], "u") == 0 && Chr2 >= 0 && strpbrk(engine->Letras[Chr2], "ei") != (char *) 0) {
				Pos += strlen(engine->Letras[Chr1]);
			}
		}
	}

	Ind = Pos;
	NumVocs = 0;
	while ((Chr = IndexChr(Sil+Ind, engine->Vocales)) >= 0) {
		Ind += strlen(engine->Vocales[Chr]);
		NumVocs++;
	}

	switch (NumVocs) {
	case 0 :
		return -1;
	case 1 :
		PosAcc = Pos;
		break;
	default :
		Ind = Pos;
		Chr1 = IndexChr(Sil+Ind, engine->Vocales);
		if (IndexChr(engine->Vocales[Chr1], VocFort) >= 0) {
			PosAcc = Pos;
			break;
		}
		else {
			PosAcc = Pos + strlen(engine->Vocales[Chr1]);
			break;
		}
	}

	if (Sil[PosAcc] == 'h') PosAcc++;
	for (Pos = strlen(Sil)+1; Pos > PosAcc; Pos--) {
		Sil[Pos] = Sil[Pos-1];
	}
	Sil[PosAcc] = '\'';

	return 0;
}

/***********************************************************************
 * EsExcAcc - Determina si una palabra es una excepcion de acentuacion.
 **********************************************************************/


int		EsExcAcc(	char	*SilOrt,
					int		PosAct,
					char	**LisExc, char **Letras)

{
	int		Pos, Long;
	GRP_ORT	GrpOrt;
	char	*Guion;
	int		ActLength = strlen(SilOrt + PosAct);

	for (Pos = 0; LisExc[Pos] != (char *) 0; Pos++) {
		Long = strlen(LisExc[Pos]);
		if (Long > ActLength ||
			IndexChr(SilOrt+PosAct+Long, Letras) >= 0 ||
			IndexChr(SilOrt+PosAct+Long, InterSil) >= 0) {
			continue;
		}
		if (!strncmp(SilOrt + PosAct, LisExc[Pos], Long)) {
			Guion = strchr(SilOrt + PosAct, '-');
			if ((Guion == (char *) 0 || Guion-SilOrt > PosAct+Long)) {
				return Long;
			}
			CogeGrpOrt(SilOrt, PosAct+Long, &GrpOrt, Letras);
			if (GrpOrt.Tipo & ESPACIOS) {
				return Long;
			}
		}
	}

	return 0;
}
