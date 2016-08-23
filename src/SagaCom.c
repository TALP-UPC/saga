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
#include	"Saga.h"

/***********************************************************************
 * CogeGrpOrt - Coge un grupo ortografico.
 **********************************************************************/


int		CogeGrpOrt(	char	*TxtOrt,
					int		PosAct,
					GRP_ORT	*GrpAct, char **Letras)

{
	int		Long, Chr;

	Long = 0;
	GrpAct->Cont = TxtOrt + PosAct;
	if ((Chr = IndexChr(TxtOrt+PosAct+Long, Letras)) >= 0) {
		Long += strlen(Letras[Chr]);
		while (IndexChr(TxtOrt+PosAct+Long, Letras) >= 0 || IndexChr(TxtOrt+PosAct+Long, InterSil) >= 0) {
			if ((Chr = IndexChr(TxtOrt+PosAct+Long, Letras)) >= 0) {
				Long += strlen(Letras[Chr]);
			}
			else {
				Chr = IndexChr(TxtOrt+PosAct+Long, InterSil);
				Long += strlen(InterSil[Chr]);
			}
		}
		GrpAct->Tipo = PALABRA;
		GrpAct->Long = Long;
		return 0;
	}
	else if (TxtOrt[PosAct] == '+') {
		Long++;
		while (TxtOrt[PosAct + Long] &&
			IndexChr(TxtOrt+PosAct+Long, Espacios) < 0 &&
			IndexChr(TxtOrt+PosAct+Long, Silen) < 0) {
			Long++;
		}
		GrpAct->Tipo = LITERAL;
		GrpAct->Long = Long;
		return 0;
	}
	else if ((Chr = IndexChr(TxtOrt+PosAct+Long, Espacios)) >= 0) {
		Long += strlen(Espacios[Chr]);
		while ((Chr = IndexChr(TxtOrt+PosAct+Long, Espacios)) >= 0) {
			Long += strlen(Espacios[Chr]);
		}
		GrpAct->Tipo = ESPACIOS;
		GrpAct->Long = Long;
		return 0;
	}
	else if ((Chr = IndexChr(TxtOrt+PosAct+Long, Silen)) >= 0) {
		Long += strlen(Silen[Chr]);
		while ((Chr = IndexChr(TxtOrt+PosAct+Long, Silen)) >= 0) {
			Long += strlen(Silen[Chr]);
		}
		GrpAct->Tipo = SILENCIO;
		GrpAct->Long = Long;
		return 0;
	}

	/* 
	 * Si llegamos hasta aqui es que no podemos determinar a que tipo
	 * de caracter pertenece la entrada, asi que es un error. Puede
	 * ser un numero, un caracter extendido, etc.
	 */
	else {
		Long++;
		while (TxtOrt[PosAct + Long] &&
			IndexChr(TxtOrt+PosAct+Long, Espacios) < 0 &&
			IndexChr(TxtOrt+PosAct+Long, Silen) < 0) {
			Long++;
		}
		GrpAct->Tipo = DESCONOCIDO;
		GrpAct->Long = Long;
		return 0;
	}

	return -1;
}

/***********************************************************************
 * CogeGrpFon - Coge un grupo fonetico.
 **********************************************************************/


int		CogeGrpFon(char	*TrnFon,
					int		PosAct,
					GRP_ORT	*GrpAct, char **Fonemas)

{
	int		Long, Chr;

	/*
	 * Primero, miramos si es una palabra transcrita
	 */
	Long = 0;
	GrpAct->Cont = TrnFon + PosAct;
	if ((Chr = IndexChr(TrnFon+PosAct+Long, Fonemas)) >= 0) {
		Long += strlen(Fonemas[Chr]);
		while (IndexChr(TrnFon+PosAct+Long, Fonemas) >= 0 || IndexChr(TrnFon+PosAct+Long, InterSil) >= 0) {
			if ((Chr = IndexChr(TrnFon+PosAct+Long, Fonemas)) >= 0) {
				Long += strlen(Fonemas[Chr]);
			}
			else {
				Chr = IndexChr(TrnFon+PosAct+Long, InterSil);
				Long += strlen(InterSil[Chr]);
			}
		}
		if (TrnFon[PosAct+Long] == '\0' ||
			IndexChr(TrnFon+PosAct+Long, EspFon ) >= 0 ||
			IndexChr(TrnFon+PosAct+Long, PuntFon) >= 0 ||
			IndexChr(TrnFon+PosAct+Long, SilFon ) >= 0) {
			GrpAct->Tipo = PAL_FON;
			GrpAct->Long = Long;
			return 0;
		}
	}

	/*
	 * No es una palabra
	 */
	Long = 0;
	GrpAct->Cont = TrnFon + PosAct;
	if (TrnFon[PosAct] == '+') {
		Long++;
		while (TrnFon[PosAct+Long] &&
			IndexChr(TrnFon+PosAct+Long, EspFon) < 0 &&
			IndexChr(TrnFon+PosAct+Long, PuntFon) < 0 &&
			IndexChr(TrnFon+PosAct+Long, SilFon) < 0) {
			Long++;
		}
		GrpAct->Tipo = LITERAL;
		GrpAct->Long = Long;
		return 0;
	}
	else if ((Chr = IndexChr(TrnFon+PosAct+Long, EspFon)) >= 0) {
		Long += strlen(EspFon[Chr]);
		while ((Chr = IndexChr(TrnFon+PosAct+Long, EspFon)) >= 0) {
			Long += strlen(EspFon[Chr]);
		}
		GrpAct->Tipo = ESP_FON;
		GrpAct->Long = Long;
		return 0;
	}
	else if ((Chr = IndexChr(TrnFon+PosAct+Long, SilFon)) >= 0) {
		Long += strlen(SilFon[Chr]);
		while ((Chr = IndexChr(TrnFon+PosAct+Long, SilFon)) >= 0) {
			Long += strlen(SilFon[Chr]);
		}
		GrpAct->Tipo = SIL_FON;
		GrpAct->Long = Long;
		return 0;
	}
	else if ((Chr = IndexChr(TrnFon+PosAct+Long, PuntFon)) >= 0) {
		Long += strlen(PuntFon[Chr]);
		GrpAct->Tipo = PUNT_FON;
		GrpAct->Long = Long;
		return 0;
	}

	/* 
	 * Si llegamos hasta aqui es que no podemos determinar a que tipo
	 * de caracter pertenece la entrada, asi que es un error. Puede
	 * ser un numero, un caracter extendido, etc.
	 */
	else {
		Long++;
		while (TrnFon[PosAct+Long] &&
			IndexChr(TrnFon+PosAct+Long, EspFon ) < 0 && 
			IndexChr(TrnFon+PosAct+Long, PuntFon) < 0 && 
			IndexChr(TrnFon+PosAct+Long, SilFon ) < 0) {
			Long++;
		}
		GrpAct->Tipo = DESCONOCIDO;
		GrpAct->Long = Long;
		return 0;
	}

	return -1;
}

/***********************************************************************
 * IndexChr - Determina el indice de un string en un vector de strings.
 **********************************************************************/


int		IndexChr(const char	* Chr,
					char ** VectChr)

{
	int		Pos, Long;
	int		Ajuste, Selec;

	Ajuste = 0;
	Selec = -1;
	for (Pos = 0; VectChr[Pos] != (char *) 0; Pos++) {
		Long = strlen(VectChr[Pos]);
		if (strncmp(Chr, VectChr[Pos], Long) == 0 && Long > Ajuste) {
			Ajuste = Long;
			Selec = Pos;
		}
	}

	return Selec;
}

/***********************************************************************
 * EscrPalExt - Escribe las palabras extranhas encontradas.
 **********************************************************************/


int		EscrPalExt(char	**PalExt)

{
	if (*PalExt != (char *) 0) {
		fprintf(stderr, "\n-----------------------------\n");
		fprintf(stderr, "Palabras extranhas encontradas:\n");
	}

	while (*PalExt != (char *) 0) {
		fprintf(stderr, "%s\n", *PalExt);
		PalExt++;
	}

	return 0;
}
