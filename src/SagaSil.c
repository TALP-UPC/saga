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
 * SilaTxtOrt - Silabifica un texto ortografico.
 **********************************************************************/


char	*SilaTxtOrt(char	*TxtOrt, char	***DicTrnPal)

{
	char	*SilOrt, *SilPal;
	int		PosAct, LongTxt = strlen(TxtOrt);
	int		Encontrada, i;
	GRP_ORT	GrpAct;

	/*
	 * Ubicamos memoria para albergar el doble mas uno de la longitud
	 * de TxtOrt. Esto garantiza que cabra cualquier silabificacion. No
	 * es muy elegante, pero si es eficiente.
	 */
	if ((SilOrt = (char *) calloc((size_t) (2*LongTxt+1), sizeof(char))) == (char *) 0) {
		fprintf(stderr, "Error al ubicar memoria para SilOrt\n");
		return (char *) 0;
	}

	/*
	 * Inicializamos SilOrt a "" para poder utilizar strcat's...
	 */
	strcpy(SilOrt, "");
	
	/*
	 * Bucle para todos los grupos ortograficos.
	 */
	PosAct = 0;
	while (PosAct < LongTxt) {
		/*
		 * Cogemos el siguiente grupo ortografico de TxtOrt.
		 */
		if (CogeGrpOrt(TxtOrt, PosAct, &GrpAct) < 0) {
			fprintf(stderr, "Error al localizar el siguiente grupo de TxtOrt\n");
			return (char *) 0;
		}

		Encontrada = 0;
		for (i = 0; DicTrnPal != (char ***) 0 && DicTrnPal[i] != (char **) 0; i++) {
			if (strncmp(DicTrnPal[i][0], GrpAct.Cont, GrpAct.Long) == 0) {
				Encontrada = 1;
				strncat(SilOrt, GrpAct.Cont, GrpAct.Long);
				break;
			}
		}
				
		if (!Encontrada && (GrpAct.Tipo & PALABRA)) {
			if ((SilPal = SilaPalOrt(GrpAct)) == (char *) 0) {
				strncat(SilOrt, GrpAct.Cont, GrpAct.Long);
/*
				strncpy(SilOrt, GrpAct.Cont, GrpAct.Long);
				SilOrt[GrpAct.Long] = '\0';
*/
			}
			else {
				strcat(SilOrt, SilPal);
				free((void *) SilPal);
			}
		}
		else if (!Encontrada) {
			strncat(SilOrt, GrpAct.Cont, GrpAct.Long);
		}

		PosAct += GrpAct.Long;
	}

	return SilOrt;
}

/***********************************************************************
 * SilaPalOrt - Silabifica una palabra de texto ortografico.
 **********************************************************************/


char	*SilaPalOrt(GRP_ORT	GrpOrt)

{
	int		Pos, Ind, Chr, Chr1, Chr2, Chr3, FinSil;
	int		NumVocs, NumCons;
	char	*SilOrt, GrpVocs[128], GrpCons[128];

	/*
	 * Ubicamos memoria para albergar el doble mas uno de la longitud de
	 * GrpOrt.Long. Esto garantiza que cabra cualquier silabificacion.
	 * No es muy elegante, pero si es eficiente.
	 */
	if ((SilOrt = (char *) calloc((size_t) (2*GrpOrt.Long+1), sizeof(char))) == (char *) 0) {
		fprintf(stderr, "Error al ubicar memoria para SilOrt\n");
		return (char *) 0;
	}

	/*
	 * Todas las silabas responden a la forma [cons]*[vocs]+[cons]*.
	 */
	Pos = 0;
	while (Pos < (int) GrpOrt.Long) {
		FinSil = 0;
		if ((Chr = IndexChr(GrpOrt.Cont+Pos, InterSil)) >= 0) {
			strcat(SilOrt, InterSil[Chr]);
			Pos += strlen(InterSil[Chr]);
			continue;
		}

		while ((Chr = IndexChr(GrpOrt.Cont+Pos, ConsTxt)) >= 0) {
			if (strcmp(ConsTxt[Chr], "y") == 0) {
				Ind = Pos + strlen(ConsTxt[Chr]);
				if ((Chr1 = IndexChr(GrpOrt.Cont+Ind, ConsTxt)) >= 0 ||
					IndexChr(GrpOrt.Cont+Ind, Letras) < 0) {
					break;
				}
			}
			else if (strcmp(ConsTxt[Chr], "h") == 0) {
				Ind = Pos + strlen(ConsTxt[Chr]);
				if ((Chr1 = IndexChr(GrpOrt.Cont+Ind, ConsTxt)) >= 0 ||
					IndexChr(GrpOrt.Cont+Ind, Letras) < 0) {
					Pos += strlen(ConsTxt[Chr]);
					break;
				}
				else if ((Chr1 = IndexChr(GrpOrt.Cont+Ind, Vocales)) >= 0) {
					break;
				}
			}

			strcat(SilOrt, ConsTxt[Chr]);
			Pos += strlen(ConsTxt[Chr]);

			if (strcmp(ConsTxt[Chr], "g") == 0) {
				Ind = Pos;
				if ((Chr1 = IndexChr(GrpOrt.Cont+Ind, Letras)) < 0) {
					free((void *) SilOrt);
					return (char *) 0;
				}
				Ind += strlen(Letras[Chr1]);
				Chr2 = IndexChr(GrpOrt.Cont+Ind, Letras);
				if (Chr2 >= 0 &&
					strcmp(Letras[Chr1], "u") == 0 &&
					strpbrk(Letras[Chr2], "ei") != (char *) 0) {
					strcat(SilOrt, Letras[Chr1]);
					Pos += strlen(Letras[Chr1]);
				}
			}
		}

		Ind = Pos;
		NumVocs = 0;
		GrpVocs[0] = '\0';
		while ((Chr = IndexChr(GrpOrt.Cont+Ind, Vocales)) >= 0) {
			if (Ind > 0 && (!strcmp(Vocales[Chr], "hu") || !strcmp(Vocales[Chr], "hi"))) {
				Chr1 = IndexChr(GrpOrt.Cont+Ind+2, Vocales);
				if (Chr1 >= 0 && strpbrk(Vocales[Chr1], "e")) {
					if (NumVocs > 0) {
						strcat(SilOrt, GrpVocs);
						strcat(SilOrt, InterSil[0]);
					}
					Pos = Ind;
					NumVocs = 1;
					strcpy(GrpVocs, Vocales[Chr]);
					break;
				}
			}
			Ind += strlen(Vocales[Chr]);
			strcat(GrpVocs, Vocales[Chr]);
			NumVocs++;
		}

		switch (NumVocs) {
		case 0 :
			if (strcmp(SilOrt, "y") == 0) {
				break;
			}
			else {
				free((void *) SilOrt);
				return (char *) 0;
			}
		case 1 :
			strcat(SilOrt, GrpVocs);
			Pos += strlen(GrpVocs);
			break;
		case 2 :
			Ind = Pos;
			Chr1 = IndexChr(GrpOrt.Cont+Ind, Vocales);
			Ind += strlen(Vocales[Chr1]);
			Chr2 = IndexChr(GrpOrt.Cont+Ind, Vocales);
			if (IndexChr(Vocales[Chr1], VocFort) >= 0 &&
				IndexChr(Vocales[Chr2], VocFort) >= 0) {
				strcat(SilOrt, Vocales[Chr1]);
				Pos += strlen(Vocales[Chr1]);
				FinSil = 1;
				break;
			}
			else {
				strcat(SilOrt, GrpVocs);
				Pos += strlen(GrpVocs);
				break;
			}
		default :
			Ind = Pos;
			Chr1 = IndexChr(GrpOrt.Cont+Ind, Vocales);
			Ind += strlen(Vocales[Chr1]);
			Chr2 = IndexChr(GrpOrt.Cont+Ind, Vocales);
			Ind += strlen(Vocales[Chr2]);
			Chr3 = IndexChr(GrpOrt.Cont+Ind, Vocales);
			if (strpbrk(Vocales[Chr1], "iu") != (char *) 0 &&
				strpbrk(Vocales[Chr2], "iu") != (char *) 0) {
				strcat(SilOrt, Vocales[Chr1]);
				Pos += strlen(Vocales[Chr1]);
				FinSil = 1;
				if (strpbrk(Vocales[Chr3], "iu") != (char *) 0) {
					strcat(SilOrt, InterSil[0]);
					strcat(SilOrt, Vocales[Chr2]);
					Pos += strlen(Vocales[Chr2]);
				}
			}
			else if (IndexChr(Vocales[Chr2], VocFort) >= 0) {
				if (strchr(Vocales[Chr2], 'i') != (char *) 0) {
					strcat(SilOrt, Vocales[Chr1]);
					Pos += strlen(Vocales[Chr1]);
					strcat(SilOrt, InterSil[0]);
					strcat(SilOrt, Vocales[Chr2]);
					Pos += strlen(Vocales[Chr2]);
					FinSil = 1;
				}
				else if (IndexChr(Vocales[Chr1], VocFort) >= 0) {
					strcat(SilOrt, Vocales[Chr1]);
					Pos += strlen(Vocales[Chr1]);
					FinSil = 1;
				}
				else {
					strcat(SilOrt, Vocales[Chr1]);
					Pos += strlen(Vocales[Chr1]);
				}
			}
			else if (strpbrk(Vocales[Chr2], "iu") != (char *) 0 &&
				strpbrk(Vocales[Chr3], "iu") != (char *) 0) {
				strcat(SilOrt, Vocales[Chr1]);
				Pos += strlen(Vocales[Chr1]);
				strcat(SilOrt, Vocales[Chr2]);
				Pos += strlen(Vocales[Chr2]);
				FinSil = 1;
			}
			else if (IndexChr(Vocales[Chr3], VocFort) >= 0) {
				strcat(SilOrt, Vocales[Chr1]);
				Pos += strlen(Vocales[Chr1]);
				FinSil = 1;
			}
		}
		
		if (FinSil != 0) {
			FinSil = 0;
			if (IndexChr(GrpOrt.Cont+Pos, Letras) >= 0) {
				strcat(SilOrt, InterSil[0]);
			}
			continue;
		}

		Ind = Pos;
		NumCons = 0;
		GrpCons[0] = '\0';
		while ((Chr = IndexChr(GrpOrt.Cont+Ind, ConsTxt)) >= 0) {
/* J. Adrián 21-JUN-2005
			if (strcmp(ConsTxt[Chr], "y") == 0) {
				if ((Chr1 = IndexChr(GrpOrt.Cont+Ind+strlen(ConsTxt[Chr]), ConsTxt)) >= 0 ||
					IndexChr(GrpOrt.Cont+Ind+strlen(ConsTxt[Chr]), Letras) < 0) {
					break;
				}
			}
*/
			if (strcmp(ConsTxt[Chr], "h") == 0) {
/* J. Adrián 21-JUN-2005
				if ((Chr1 = IndexChr(GrpOrt.Cont+Ind+strlen(ConsTxt[Chr]), ConsTxt)) >= 0 ||
					IndexChr(GrpOrt.Cont+Ind+strlen(ConsTxt[Chr]), Letras) < 0) {
					Ind += strlen(ConsTxt[Chr]);
					break;
				}
*/
				if ((Chr1 = IndexChr(GrpOrt.Cont+Ind+strlen(ConsTxt[Chr]), Vocales)) >= 0) {
					break;
				}
			}
			Ind += strlen(ConsTxt[Chr]);
			strcat(GrpCons, ConsTxt[Chr]);
			NumCons++;
		}

		if ((Chr = IndexChr(GrpOrt.Cont+Ind, Vocales)) < 0) {
			strcat(SilOrt, GrpCons);
			Pos = Ind;
			continue;
		}
		else if (Pos > 0 && (!strcmp(Vocales[Chr], "hu") || !strcmp(Vocales[Chr], "hi"))) {
			Chr1 = IndexChr(GrpOrt.Cont+Ind+2, Vocales);
			if (Chr1 >= 0 && strpbrk(Vocales[Chr1], "e") != (char *) 0) {
				strcat(SilOrt, GrpCons);
				strcat(SilOrt, InterSil[0]);
				Pos = Ind;
				continue;
			}
		}

		switch (NumCons) {
		case 0 :
			break;
		case 1 :
			FinSil = 1;
			break;
		case 2 :
			Ind = Pos;
			Chr1 = IndexChr(GrpOrt.Cont+Ind, ConsTxt);
			Ind += strlen(ConsTxt[Chr1]);
			Chr2 = IndexChr(GrpOrt.Cont+Ind, ConsTxt);
			if ((IndexChr(ConsTxt[Chr1], ConsL) < 0 ||
				strcmp(ConsTxt[Chr2], "l") != 0) &&
				(IndexChr(ConsTxt[Chr1], ConsR) < 0 ||
				strcmp(ConsTxt[Chr2], "r") != 0)) {
				strcat(SilOrt, ConsTxt[Chr1]);
				Pos += strlen(ConsTxt[Chr1]);
			}
			FinSil = 1;
			break;
		case 3 :
			Ind = Pos;
			Chr1 = IndexChr(GrpOrt.Cont+Ind, ConsTxt);
			Ind += strlen(ConsTxt[Chr1]);
			Chr2 = IndexChr(GrpOrt.Cont+Ind, ConsTxt);
			Ind += strlen(ConsTxt[Chr2]);
			Chr3 = IndexChr(GrpOrt.Cont+Ind, ConsTxt);
			if ((IndexChr(ConsTxt[Chr2], ConsL) < 0 ||
				strcmp(ConsTxt[Chr3], "l") != 0) &&
				(IndexChr(ConsTxt[Chr2], ConsR) < 0 ||
				strcmp(ConsTxt[Chr3], "r") != 0)) {
				strcat(SilOrt, ConsTxt[Chr1]);
				Pos += strlen(ConsTxt[Chr1]);
				strcat(SilOrt, ConsTxt[Chr2]);
				Pos += strlen(ConsTxt[Chr2]);
			}
			else {
				strcat(SilOrt, ConsTxt[Chr1]);
				Pos += strlen(ConsTxt[Chr1]);
			}
			FinSil = 1;
			break;
		default :
			Ind = Pos;
			Chr1 = IndexChr(GrpOrt.Cont+Ind, ConsTxt);
			Ind += strlen(ConsTxt[Chr1]);
			Chr2 = IndexChr(GrpOrt.Cont+Ind, ConsTxt);
			strcat(SilOrt, ConsTxt[Chr1]);
			Pos += strlen(ConsTxt[Chr1]);
			strcat(SilOrt, ConsTxt[Chr2]);
			Pos += strlen(ConsTxt[Chr2]);
			FinSil = 1;
		}
		if (FinSil != 0) {
			strcat(SilOrt, InterSil[0]);
		}
	}

	return SilOrt;
}
