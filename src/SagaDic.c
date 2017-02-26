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


#define _POSIX_C_SOURCE 200809L

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<limits.h>
#include	<errno.h>
#include	"SagaInternal.h"
#include	"Util.h"

#ifndef _POSIX_PATH_MAX
#ifdef _MAX_PATH
#define _POSIX_PATH_MAX _MAX_PATH
#else
#error "_POSIX_PATH_MAX not defined"
#endif
#endif

char	***ReadDicExc(char	*FicDicExc);
int		AplDicPal(
	char	**Frase,
	int		PosAct,
	int		LongPal,
	char	***DicExc);

void BorraDicExc(char ***DicExc) {
	size_t i;
	if (DicExc == NULL) {
		return;
	}
	for (i=0; DicExc[i] != NULL; i++) {
		free(DicExc[i][0]);
		free(DicExc[i][1]);
		free(DicExc[i]);
	}
	free(DicExc);
}

/***********************************************************************
 * CargDicExc - Carga un diccionario de excepciones.
 **********************************************************************/

char	***CargDicExc(const char	*FicDic)

{
	int		Fic, NumFic;
	char	**LisFic;
	char	***DicExc, ***OtroDic, ***tmpDicExc;
	int		Pal, TamDic;


	if ((LisFic = MatStrChr(FicDic, "+")) == NULL) {
		fprintf(stderr, "ERROR al procesar el nombre del diccionario %s\n", FicDic);
		return NULL;
	}

    NumFic = MatStrLength(LisFic);

	if ((DicExc = ReadDicExc(LisFic[0])) == NULL) {
		fprintf(stderr, "Error al leer %s\n", LisFic[0]);
		LiberaMatStr(LisFic);
		return NULL;
	}

	for (TamDic = 0; DicExc[TamDic] != NULL; TamDic++) ;

	for (Fic = 1; Fic < NumFic; Fic++) {
		if ((OtroDic = ReadDicExc(LisFic[Fic])) == NULL) {
			fprintf(stderr, "Error al leer %s\n", LisFic[Fic]);
			BorraDicExc(DicExc);
			LiberaMatStr(LisFic);
			return NULL;
		}

		for (Pal = 0; OtroDic[Pal] != NULL; Pal++) {
			if ((tmpDicExc = realloc(DicExc, (TamDic + Pal + 2) * sizeof(char **))) == NULL) {
				fprintf(stderr, "Error al ubicar memoria para el diccionario (%s)\n", strerror(errno));
				BorraDicExc(DicExc);
				BorraDicExc(OtroDic);
				LiberaMatStr(LisFic);
				return NULL;
			}
			DicExc = tmpDicExc;
			DicExc[TamDic + Pal] = OtroDic[Pal];
		}

		TamDic += Pal;

		DicExc[TamDic] = (char **) 0;

		free((void *) OtroDic);
	}
	LiberaMatStr(LisFic);
	return DicExc;
}

/***********************************************************************
 * ReadDicExc - Carga un diccionario de excepciones.
 **********************************************************************/

char	***ReadDicExc(char	*FicDicExc)

{
	int		Long, NumPal;
	char	***DicExc, ***tmpDicExc;
	char	Linea[512], PalIn[256], PalOut[256];
	FILE	*FpDicExc;

	if ((FpDicExc = fopen(FicDicExc, "rt")) == (FILE *) 0) {
		fprintf(stderr, "Error al abrir el diccionario %s\n", FicDicExc);
		return NULL;
	}

	DicExc = NULL;
	Long = 0;
	while (fgets(Linea, sizeof(Linea), FpDicExc) != NULL) {
		if ((tmpDicExc = realloc(DicExc, (Long+2) * sizeof(char **))) == NULL) {
			fprintf(stderr, "Error al reubicar memoria para el diccionario\n");
			fclose(FpDicExc);
			BorraDicExc(DicExc);
			return NULL;
		}
		DicExc = tmpDicExc;
		if ((DicExc[Long] = (char **) malloc(2*sizeof(char *))) == NULL) {
			fprintf(stderr, "Error al reubicar memoria para el diccionario\n");
			fclose(FpDicExc);
			BorraDicExc(DicExc);
			return NULL;
		}

		NumPal = sscanf(Linea, "%[^ \t\n]%*[ \t]%[^\t\n]", PalIn, PalOut);
		if (NumPal == 0) {
			continue;
		}
		else if (NumPal == 1) {
			PalOut[0] = '\0';
		}
		else if (NumPal != 2) {
			fprintf(stderr, "Error de formato en el diccionario\n");
			fclose(FpDicExc);
			BorraDicExc(DicExc);
			return NULL;
		}

		DicExc[Long][0] = strdup(PalIn);
		if (DicExc[Long][0] == NULL) {
			fprintf(stderr, "Error de memoria para el diccionario\n");
			free(DicExc[Long]);
			DicExc[Long] = NULL;
			BorraDicExc(DicExc);
			fclose(FpDicExc);
			return NULL;
		}
		DicExc[Long][1] = strdup(PalOut);
		if (DicExc[Long][1] == NULL) {
			fprintf(stderr, "Error de memoria para el diccionario\n");
			free(DicExc[Long][0]);
			free(DicExc[Long]);
			DicExc[Long] = NULL;
			BorraDicExc(DicExc);
			fclose(FpDicExc);
			return NULL;
		}

		Long++;
		DicExc[Long] = NULL;
	}


	fclose(FpDicExc);

	return DicExc;
}

/***********************************************************************
 * AplDicExc - Aplica un diccionario de excepciones.
 **********************************************************************/


int		AplDicExc(	char	***DicExc,
					char	**TxtOrt, char **Letras)

{
	int		LongTxt = strlen(*TxtOrt);
	int		PosAct;
	int		Sust;
	GRP_ORT	GrpOrt;

	do {
		Sust = 0;
		PosAct = 0;
		while (PosAct < LongTxt) {
			/*
			 * Cogemos el siguiente grupo ortografico de TxtOrt.
			 */
			if (CogeGrpOrt(*TxtOrt, PosAct, &GrpOrt, Letras) < 0) {
				fprintf(stderr, "Error al localizar el siguiente grupo de TxtOrt\n");
				return -1;
			}
	
			if (AplDicPal(TxtOrt, PosAct, GrpOrt.Long, DicExc) >= 0) {
				Sust = 1;
			}
			PosAct += GrpOrt.Long;
		}
	} while (Sust > 0);

	return 0;
}


/***********************************************************************
 * AplDicGrp - Aplica un diccionario de excepciones.
 **********************************************************************/


int		AplDicGrp(	char	***DicExc,
					char	**TxtOrt, char **Fonemas)

{
	int		LongTxt = strlen(*TxtOrt);
	int		PosAct;
	int		Sust;
	GRP_ORT	GrpOrt;

	do {
		Sust = 0;
		PosAct = 0;
		while (PosAct < LongTxt) {
			/*
			 * Cogemos el siguiente grupo ortografico de TxtOrt.
			 */
			if (CogeGrpFon(*TxtOrt, PosAct, &GrpOrt, Fonemas) < 0) {
				fprintf(stderr, "Error al localizar el siguiente grupo de TxtOrt\n");
				return -1;
			}
	
			if (AplDicPal(TxtOrt, PosAct, GrpOrt.Long, DicExc) >= 0) {
				Sust = 1;
			}
			PosAct += GrpOrt.Long;
		}
	} while (Sust > 0);

	return 0;
}

/***********************************************************************
 * AplDicSust - Aplica un diccionario de substituciones.
 **********************************************************************/


int		AplDicSust(	char	***DicSust,
					char	**TrnFon, 
                           char **Fonemas)

{
	int		LongFon = strlen(*TrnFon), LongIn, LongOut;
	int		Ind, Chr, Pos, Pos2, PosAct;
	GRP_ORT	GrpAct;

	/*
	 * Bucle para todos los grupos foneticos.
	 */
	PosAct = 0;
	while (PosAct < LongFon) {
		/*
		 * Cogemos el siguiente grupo fonetico de TrnFon.
		 */
		if (CogeGrpFon(*TrnFon, PosAct, &GrpAct, Fonemas) < 0) {
			fprintf(stderr, "Error al localizar el siguiente grupo de SilOrt\n");
			return -1;
		}

		if (GrpAct.Tipo & PAL_FON) {
			Pos = 0;
			while (Pos < (int) GrpAct.Long) {
				if ((Chr = IndexChr(GrpAct.Cont+Pos, Fonemas)) >= 0) {
					LongIn = LongOut = strlen(Fonemas[Chr]);
					for (Ind = 0; DicSust[Ind] != (char **) 0; Ind++) {
						if (!strcmp(Fonemas[Chr], DicSust[Ind][0])) {
							LongOut = strlen(DicSust[Ind][1]);

							LongFon += LongOut - LongIn;
							GrpAct.Long += LongOut - LongIn;

							if (LongOut > LongIn) {
								Pos2 = GrpAct.Cont - *TrnFon;
								if ((*TrnFon = (char *) realloc((void *) *TrnFon, (LongFon+1) * sizeof(char))) == (char *) 0) {
									fprintf(stderr, "Error de memoria\n");
									return -1;
								}
								GrpAct.Cont = *TrnFon + Pos2;

								for (Pos2 = LongFon; Pos2 > PosAct; Pos2--) {
									(*TrnFon)[Pos2] = (*TrnFon)[Pos2+LongIn-LongOut];
								}
							}
							else if (LongOut < LongIn) {
								for (Pos2 = PosAct + 1; Pos2 <= LongFon; Pos2++) {
									(*TrnFon)[Pos2] = (*TrnFon)[Pos2+LongIn-LongOut];
								}
							}

							for (Pos2 = PosAct; Pos2 < PosAct + LongOut; Pos2++) {
								(*TrnFon)[Pos2] = DicSust[Ind][1][Pos2 - PosAct];
							}

							break;
						}
					}
					Pos += LongOut;
					PosAct += LongOut;
				}
				else {
					if ((Chr = IndexChr(GrpAct.Cont+Pos, InterSil)) < 0) {
						return -1;
					}
					else {
						Pos += strlen(InterSil[Chr]);
						PosAct += strlen(InterSil[Chr]);
						continue;
					}
				}
			}
		}
		else {
			PosAct += GrpAct.Long;
		}
	}

	return 0;
}






/***********************************************************************
 * AplDicPal - Busca una palabra en el diccionario
 **********************************************************************/

int		AplDicPal(
	char	**Frase,
	int		_PosAct,
	int		LongPal,
	char	***DicExc)
{
	char *Mascara = NULL;
	size_t MascaraSize = 0, MascaraFilled = 0;

	char *_Palabra = NULL;

	char	*Palabra, *PalDic, *IniPal;
	int		Ind, IniCom, FinCom;
	int		Pos;
	int		LongExc, LongFrs;
	int		PosAct = _PosAct;

	/* Un buffer de 256 para Mascara es suficiente */
	MascaraSize = 256;
	if ((Mascara = calloc(MascaraSize, sizeof(char))) == NULL) {
		fprintf(stderr, "Error al ubicar memoria para Mascara\n");
		return -1;
	}

	if ((_Palabra = malloc(LongPal+1)) == NULL) {
		fprintf(stderr, "Error al ubicar memoria para _Palabra\n");
		free(Mascara);
		return -1;
	}


	LongFrs = strlen(*Frase);

  
  _Palabra[0] = '\0';
  strncat(_Palabra, (*Frase) + PosAct, LongPal);
	Palabra = _Palabra;

	/*
	 * Comprobamos primero si la palabra aparece tal cual, en cuyo caso la sustituimos
	 */
	for (Ind = 0; DicExc[Ind] != (char **) 0; Ind++) {
		if (strcmp(DicExc[Ind][0], Palabra) != 0) continue;

		LongPal = strlen(Palabra);
		LongExc = strlen(DicExc[Ind][1]);

		LongFrs += LongExc - LongPal;

		if (LongPal > LongExc) {
			for (Pos = PosAct + LongExc; Pos <= LongFrs; Pos++) {
				(*Frase)[Pos] = (*Frase)[Pos + LongPal - LongExc];
			}
		}

		if (LongPal < LongExc) {
			if ((*Frase = (char *) realloc((void *) *Frase, (LongFrs + 1) * sizeof(char))) == (char *) 0) {
				fprintf(stderr, "Error de memoria\n");
				free(Mascara);
				free(_Palabra);
				return -1;
			}

			for (Pos = LongFrs; Pos > PosAct; Pos--) {
				(*Frase)[Pos] = (*Frase)[Pos + LongPal - LongExc];
			}
		}

		for (Pos = PosAct; Pos < PosAct + LongExc; Pos++) {
			(*Frase)[Pos] = DicExc[Ind][1][Pos - PosAct];
		}

		free(Mascara);
		free(_Palabra);
		return Ind;
	}

	for (Ind = 0; DicExc[Ind] != (char **) 0; Ind++) {
		Palabra = _Palabra;
		MascaraFilled = 0;
		if (safe_strcat(&Mascara, DicExc[Ind][0], &MascaraSize, &MascaraFilled) < 0) {
			free(Mascara);
			free(_Palabra);
			return -1;
		}
		PalDic = Mascara;
		LongExc = strlen(PalDic);
		IniCom = FinCom = 0;
		if (PalDic[0] == '*') {
			PalDic++;
			LongExc--;
			IniCom = 1;
		}
		if (PalDic[0] == '\\' && PalDic[1] == '*') {
			PalDic++;
			LongExc--;
		}
		if (LongExc > 1 && PalDic[LongExc - 1] == '*') {
			if (LongExc > 1 && PalDic[LongExc - 2] == '\\') {
				PalDic[LongExc - 2] = '*';
				PalDic[LongExc - 1] = '\0';
				LongExc--;
			}
			else {
				PalDic[LongExc - 1] = '\0';
				LongExc--;
				FinCom = 1;
			}
		}

		if (!IniCom && !FinCom) continue;

		if (IniCom) {
			if (strstr(Palabra, PalDic) == (char *) 0) continue;
			IniPal = Palabra;
			PosAct = _PosAct;
			while ((IniPal = strstr(IniPal, PalDic)) != (char *) 0) {
				PosAct += IniPal - Palabra;
				Palabra = IniPal;
				IniPal++;
			}
			LongPal = strlen(Palabra);
		}
		else if (strncmp(Palabra, PalDic, LongExc) != 0) continue;

		if (FinCom) {
			if (strncmp(Palabra, PalDic, LongExc) != 0) continue;
			LongPal = LongExc;
		}
		else {
			if (strncmp(Palabra + LongPal - LongExc, PalDic, LongExc) != 0) continue;
		}

		/* The condition (!IniCom && !FinCom) is false, otherwise we would
		 * have hit "continue" about 20 lines above.
		 * Therefore here we have: if (FALSE && something) continue
		 * This case will never be TRUE, we can remove it.
		 *  */
		/*if (!IniCom && !FinCom && (LongPal != LongExc || strcmp(Palabra, PalDic) != 0)) continue;*/

		LongExc = strlen(DicExc[Ind][1]);

		LongFrs += LongExc - LongPal;

		if (LongPal > LongExc) {
			for (Pos = PosAct + LongExc; Pos <= LongFrs; Pos++) {
				(*Frase)[Pos] = (*Frase)[Pos + LongPal - LongExc];
			}
		}

		if (LongPal < LongExc) {
			if ((*Frase = (char *) realloc((void *) *Frase, (LongFrs + 1) * sizeof(char))) == (char *) 0) {
				fprintf(stderr, "Error de memoria\n");
				free(Mascara);
				free(_Palabra);
				return -1;
			}

			for (Pos = LongFrs; Pos > PosAct; Pos--) {
				(*Frase)[Pos] = (*Frase)[Pos + LongPal - LongExc];
			}
		}

		for (Pos = PosAct; Pos < PosAct + LongExc; Pos++) {
			(*Frase)[Pos] = DicExc[Ind][1][Pos - PosAct];
		}

		free(Mascara);
		free(_Palabra);
		return Ind;
	}

	free(Mascara);
	free(_Palabra);
	return -1;
}
