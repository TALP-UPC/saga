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
#include	<ctype.h>
#include	"SagaInternal.h"


/***********************************************************************
 * TrnSilAcc - Transcribe un texto silabificado y acentuado.
 **********************************************************************/

char	*TrnSilAcc(	char	*SilAcc,
					char	***DicTrnPal,
					char	***DicTrnFon,
					int		TrnPalAis,
					long	ClaveModif, SagaEngine *engine)

{
	char	*TrnFon, *TrnPal;
	size_t		PosAct, PosSig, LongTxt = strlen(SilAcc), Long;
	GRP_ORT	GrpAct, GrpAnt, GrpSig;
	int		Encontrada, i;

	/*
	 * Ubicamos memoria para albergar el doble mas uno de la longitud
	 * de SilAcc. Esto garantiza que cabra cualquier transcripcion. No
	 */
	if ((TrnFon = calloc((size_t) (20*LongTxt+1), sizeof(char))) == NULL) {
		fprintf(stderr, "Error al ubicar memoria para TrnFon\n");
		return NULL;
	}

	/*
	 * Inicializamos TrnFon a "" para poder utilizar strcat's...
	 */
	strcpy(TrnFon, "");
	
	/*
	 * Cogemos el primer grupo ortografico.
	 */
	if (CogeGrpOrt(SilAcc, 0, &GrpAct, engine->Letras) < 0) {
		free(TrnFon);
		return NULL;
	}

	/*
	 * Bucle para todos los grupos ortograficos.
	 */
	PosAct = 0;
	PosSig = GrpAct.Long;
	GrpAnt.Tipo = SILENCIO;
	GrpAnt.Long = 0;
	while (PosAct < LongTxt) {
		/*
		 * Cogemos el siguiente grupo ortografico de SilAcc.
		 */
		if (CogeGrpOrt(SilAcc, PosAct, &GrpAct, engine->Letras) < 0) {
			free(TrnFon);
			return NULL;
		}

		/*
		 * Cogemos el contexto posterior de GrpAct.
		 */
		PosSig = PosAct + GrpAct.Long;
		if (PosSig < LongTxt) {
			CogeGrpOrt(SilAcc, PosSig, &GrpSig, engine->Letras);
			if (GrpSig.Tipo == LITERAL) {
				GrpSig.Tipo = PALABRA;
				GrpSig.Cont++;
				GrpSig.Long--;
			}
		}
		else {
			GrpSig.Tipo = SILENCIO;
		}
		if (GrpSig.Tipo == ESPACIOS) {
			PosSig = PosSig + GrpSig.Long;
			if (PosSig < LongTxt) {
				CogeGrpOrt(SilAcc, PosSig, &GrpSig, engine->Letras);
				if (GrpSig.Tipo == LITERAL) {
					GrpSig.Tipo = PALABRA;
					GrpSig.Cont++;
					GrpSig.Long--;
				}
			}
			else {
				GrpSig.Tipo = SILENCIO;
			}
		}

		Encontrada = 0;
		for (i = 0; DicTrnPal != NULL && DicTrnPal[i] != NULL; i++) {
			if ((Long = strlen(DicTrnPal[i][0])) != GrpAct.Long) continue;
			if (strncmp(DicTrnPal[i][0], GrpAct.Cont, Long) == 0) {
				Encontrada = 1;
				strcat(TrnFon, DicTrnPal[i][1]);
				break;
			}
		}
				
		if (!Encontrada && (GrpAct.Tipo & PALABRA)) {
			if ((TrnPal = TrnPalSil(GrpAnt, GrpAct, GrpSig, DicTrnFon, TrnPalAis, ClaveModif, engine)) == NULL) {
				free(TrnFon);
				return NULL;
			}

			strcat(TrnFon, TrnPal);
			free(TrnPal);
		}
		else if (!Encontrada) {
			strncat(TrnFon, GrpAct.Cont, GrpAct.Long);
		}
		
		PosAct += GrpAct.Long;

		if (GrpAct.Tipo != ESPACIOS) {
			GrpAnt = GrpAct;
			if (GrpAnt.Tipo & LITERAL) {
				GrpAnt.Tipo = PALABRA;
				GrpAnt.Cont++;
				GrpAnt.Long--;
			}
		}
	}

	TrnFon = realloc(TrnFon, strlen(TrnFon) + 1);

	return TrnFon;
}


/***********************************************************************
 * TrnPalSil - Transcribe una palabra silabificada y acentuada.
 **********************************************************************/

char	*TrnPalSil(	GRP_ORT	GrpAnt,
					GRP_ORT	GrpAct,
					GRP_ORT	GrpSig,
					char	***DicTrnFon,
					int		TrnPalAis,
					long	ClaveModif, SagaEngine *engine)
{
	char	*TrnPal, *Sil, *Pointer;
	char	CntAnt[128], CntSig[128];
	char	SilAnt[128], SilAct[128], SilSig[128];
	char	ChrAnt[128], ChrAct[128], ChrSig[128];
	int		Chr, Chr1, Chr2;
	int		Pos, Ind, Long = GrpAct.Long;
	int		IniSil, FinSil, IniPal, FinPal;
	int		SigueEse;
	int		EsVocal;
	int		EsSemi;
	static int	EraVocal = 0;
	static int	EraSemi = 0;
	int		Encontrada, i;

	/*
	 * Ubicamos memoria para albergar el doble mas uno de la longitud de
	 * GrpOrt.Long. Esto garantiza que cabra cualquier silabificacion.
	 */
	if ((TrnPal = calloc((size_t) (20*Long+1), sizeof(char))) == NULL) {
		fprintf(stderr, "Error al ubicar memoria para TrnPal\n");
		return NULL;
	}

	/*
	 * Determinamos los contextos anterior y posterior.
	 */
	strcpy(ChrAnt, "");
	if (TrnPalAis == 1 || GrpAnt.Tipo != PALABRA) {
		strcpy(CntAnt, "");
		EraVocal = 0;
		EraSemi = 0;
	}
	else {
		for (Pos = GrpAnt.Long; Pos > 0; Pos--) {
			if (GrpAnt.Cont[Pos-1] == '-') break;
		}
		Sil = GrpAnt.Cont + Pos;
		Pos = 0;
		strcpy(CntAnt, "");
		while ((Chr = IndexChr(Sil+Pos, engine->Letras)) >= 0) {
			Pos += strlen(engine->Letras[Chr]);
			strcat(CntAnt, engine->Letras[Chr]);
			strcpy(ChrAnt, engine->Letras[Chr]);
		}
	}
	if (TrnPalAis == 1 || GrpSig.Tipo != PALABRA) {
		strcpy(CntSig, "");
	}
	else {
		Pos = 0;
		strcpy(CntSig, "");
		while ((Chr = IndexChr(GrpSig.Cont+Pos, engine->Letras)) >= 0) {
			Pos += strlen(engine->Letras[Chr]);
			strcat(CntSig, engine->Letras[Chr]);
		}
	}

	strcpy(SilAnt, CntAnt);
	strcpy(SilAct, "");
	Pos = 0;
	IniPal = 1;
	FinPal = 0;
	while (Pos < (int) GrpAct.Long) {
		if ((Chr = IndexChr(GrpAct.Cont+Pos, InterSil)) >= 0) {
			Pos += strlen(InterSil[Chr]);
			strcpy(SilAnt, SilAct);
			strcpy(SilAct, "");

			/*
			 * Empezamos el procesado de la <x> porque afecta a la
			 * silabificacion. Lo que queda es siempre <s>.
			 */
			Chr1 = IndexChr(GrpAct.Cont+Pos, engine->Letras);
			if (Chr1 >= 0 && strcmp(engine->Letras[Chr1], "x") == 0) {
				if (ClaveModif & EQUIS_KS) {
					if (ClaveModif & OCLUS_EXPL) {
						strcat(TrnPal, "kcl");
						if ((ClaveModif & MARCA_IMPL)) strcat(TrnPal, ":");
					}
					strcat(TrnPal, "k");
					*(GrpAct.Cont+Pos) = 's';
				}
				else {
					strcat(TrnPal, "G");
					if ((ClaveModif & MARCA_IMPL)) strcat(TrnPal, ":");
				}
			}
			strcat(TrnPal, InterSil[Chr]);
		}
		
		while ((Chr = IndexChr(GrpAct.Cont+Pos, engine->Letras)) >= 0) {
			Pos += strlen(engine->Letras[Chr]);
			strcat(SilAct, engine->Letras[Chr]);
		}

		if ((Chr = IndexChr(GrpAct.Cont+Pos, InterSil)) >= 0) {
			Ind = Pos + strlen(InterSil[Chr]);
			strcpy(SilSig, "");
			while ((Chr = IndexChr(GrpAct.Cont+Ind, engine->Letras)) >= 0) {
				Ind += strlen(engine->Letras[Chr]);
				strcat(SilSig, engine->Letras[Chr]);
			}
		}
		else {
			strcpy(SilSig, CntSig);
		}

		Ind = 0;
		strcpy(ChrAnt, "");
		while ((Chr = IndexChr(SilAnt+Ind, engine->Letras)) >= 0) {
			Ind += strlen(engine->Letras[Chr]);
			strcpy(ChrAnt, engine->Letras[Chr]);
		}

		if (Pos == (int) GrpAct.Long) FinPal = 1;

		IniSil = 1;
		FinSil = 0;
		Ind = 0;
		while ((Chr = IndexChr(SilAct+Ind, engine->Letras)) >= 0) {
			if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");

			EsVocal = 0;
			EsSemi = 0;
			strcpy(ChrAct, engine->Letras[Chr]);
			Ind += strlen(ChrAct);
			strcpy(ChrSig, "");
			if (Ind == (int) strlen(SilAct)) {
				FinSil = 1;
				Chr = IndexChr(SilSig, engine->Letras);
				if (Chr >= 0) strcpy(ChrSig, engine->Letras[Chr]);
			}
			else {
				Chr = IndexChr(SilAct+Ind, engine->Letras);
				if (Chr >= 0) strcpy(ChrSig, engine->Letras[Chr]);
			}

			Encontrada = 0;
			for (i = 0; DicTrnFon != (char ***) 0 && DicTrnFon[i] != (char **) 0; i++) {
				if (strcmp(DicTrnFon[i][0], ChrAct) == 0) {
					Encontrada = 1;
					break;
				}
			}

			if (Encontrada) {
				strcat(TrnPal, DicTrnFon[i][1]);
			}
			else if (strchr(ChrAct, 'a') != (char *) 0) {
				if (strchr(ChrAct, '\'') != (char *) 0) {
					strcat(TrnPal, "'");
				}
				strcat(TrnPal, "a");
				EsVocal = 1;
			}

			else if (strchr(ChrAct, 'e') != (char *) 0) {
				if (strchr(ChrAct, '\'') != (char *) 0) {
					strcat(TrnPal, "'");
				}
				strcat(TrnPal, "e");
				EsVocal = 1;
			}

			else if (strchr(ChrAct, 'i') != (char *) 0) {
				if (strchr(ChrAct, '\'') != (char *) 0) {
					strcat(TrnPal, "'i");
					EsVocal = 1;
				}
				else if (!strcmp(ChrAct, "hi") && IniSil && !FinSil &&
					(Chr = IndexChr(SilAct+Ind, engine->Letras)) >= 0 && strchr(engine->Letras[Chr], 'e')) {
					strcat(TrnPal, "jj");
				}
				else if ((!FinSil && IndexChr(ChrSig, engine->Vocales) >= 0) || (!IniSil && IndexChr(ChrAnt, VocFort) >= 0)) {
					strcat(TrnPal, "j");
					EsSemi = 1;
				}
				else {
					strcat(TrnPal, "i");
					EsVocal = 1;
				}
			}

			else if (strchr(ChrAct, 'o') != (char *) 0) {
				if (strchr(ChrAct, '\'') != (char *) 0) {
					strcat(TrnPal, "'");
				}
				strcat(TrnPal, "o");
				EsVocal = 1;
			}

			else if (strchr(ChrAct, 'u') != (char *) 0) {
				if (!strcmp(ChrAct, "qu")) {
					if (ClaveModif & OCLUS_EXPL) {
						strcat(TrnPal, "kcl");
						if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
						if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
					}
					strcat(TrnPal, "k");
				}
				else if (strchr(ChrAct, '\'') != (char *) 0) {
					strcat(TrnPal, "'u");
					EsVocal = 1;
				}
				else if (!strcmp(ChrAct, "u") && IniSil && IniPal && FinSil && FinPal) {
					Chr1 = IndexChr(ChrSig, engine->Vocales);
					if (Chr1 >= 0 && strchr(engine->Vocales[Chr1], 'o')) {
						strcat(TrnPal, "w");
						EsSemi = 1;
					}
					else {
						strcat(TrnPal, "u");
						EsVocal = 1;
					}
				}
				else if (!strcmp(ChrAct, "hu") &&
					IniSil && !FinSil &&
					(Chr = IndexChr(SilAct+Ind, engine->Letras)) >= 0 &&
					strchr(engine->Letras[Chr], 'e')) {
					strcat(TrnPal, "Gw");
				}
				else if ((!FinSil && IndexChr(ChrSig, engine->Vocales) >= 0) ||
					(!IniSil && IndexChr(ChrAnt, VocFort) >= 0)) {
					strcat(TrnPal, "w");
					EsSemi = 1;
				}
				else {
					strcat(TrnPal, "u");
					EsVocal = 1;
				}
			}

			else if (!strcmp(ChrAct, "y")) {
				if (IniSil && IniPal && FinSil && FinPal) {
					Chr1 = IndexChr(ChrAnt, engine->Vocales);
					Chr2 = IndexChr(ChrSig, engine->Vocales);

					if (!strcmp(ChrSig, "hi") || !strcmp(ChrSig, "hu")) {
						if (Chr1 >= 0 && !strchr(engine->Vocales[Chr1], 'i')) {
							strcat(TrnPal, "j");
							EsSemi = 1;
						}
						else {
							strcat(TrnPal, "i");
							EsVocal = 1;
						}
					}
					else if (!(ClaveModif & Y_VOCAL) &&
						Chr1 >= 0 && !strchr(engine->Vocales[Chr1], 'i') &&
						Chr2 >= 0 && !strchr(engine->Vocales[Chr2], 'i')) {
						strcat(TrnPal, "jj");
					}
					else if ((Chr1 >= 0 && strchr(engine->Vocales[Chr1], 'i')) ||
						(Chr2 >= 0 && strchr(engine->Vocales[Chr2], 'i'))) {
						strcat(TrnPal, "i");
						EsVocal = 1;
					}
					else if (Chr1 >= 0 && !strchr(engine->Vocales[Chr1], 'i')) {
						strcat(TrnPal, "j");
						EsSemi = 1;
					}
					else if (Chr2 >= 0 && !strchr(engine->Vocales[Chr2], 'i')) {
						strcat(TrnPal, "j");
						EsSemi = 1;
					}
					else {
						strcat(TrnPal, "i");
						EsVocal = 1;
					}
				}
				else if ((IniSil && IniPal && IndexChr(ChrSig, engine->Vocales) >= 0) ||
					(IniSil && IndexChr(ChrAnt, engine->ConsTxt) >= 0)) {
					strcat(TrnPal, "jj");
				}
				else if (IniSil && IndexChr(ChrSig, engine->Vocales) >= 0) {
					strcat(TrnPal, "jj");
				}
				else if (IndexChr(ChrSig, engine->Vocales) >= 0) {
					strcat(TrnPal, "j");
					EsSemi = 1;
				}
				else if (EraVocal) {
					strcat(TrnPal, "j");
					EsSemi = 1;
				}
				else {
					strcat(TrnPal, "i");
					EsVocal = 1;
				}
			}

			else if (!strcmp(ChrAct, "h")) ;
		
			else if ((ClaveModif & ARCHI_IMPL) && FinSil && !FinPal && (
				!strcmp(ChrAct, "p") || !strcmp(ChrAct, "b") || !strcmp(ChrAct, "v") || !strcmp(ChrAct, "d") ||
				!strcmp(ChrAct, "t") || !strcmp(ChrAct, "c") || !strcmp(ChrAct, "g") || !strcmp(ChrAct, "j"))) {
				strcat(TrnPal, "G");
			}

			else if (!strcmp(ChrAct, "b") || !strcmp(ChrAct, "v") || !strcmp(ChrAct, "w")) {
				if ((ClaveModif & ELIM_B) && !IniPal && FinPal && 
					strchr(ChrSig, '\'') == (char *) 0 && EraVocal && IndexChr(ChrSig, engine->Vocales) >= 0) {
				}
				else if ((ClaveModif & ELIM_B) && FinPal && FinSil && EraVocal) {
				}
				else if (ClaveModif & BDG_ANDES) {
					if (!IniSil || EraVocal) {
						strcat(TrnPal, "B");
					}
					else {
						if (ClaveModif & OCLUS_EXPL) {
							strcat(TrnPal, "bcl");
							if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
							if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
						}
						strcat(TrnPal, "b");
					}
				}
				else if (IniSil && ((IniPal && !strlen(CntAnt)) || !strcmp(ChrAnt, "m") || !strcmp(ChrAnt, "n"))) {
					if (ClaveModif & OCLUS_EXPL) {
						strcat(TrnPal, "bcl");
						if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
						if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
					}
					strcat(TrnPal, "b");
				}
				else {
					strcat(TrnPal, "B");
				}
			}

			else if (!strcmp(ChrAct, "c")) {
				if (FinSil == 0 && (strchr(ChrSig, 'e') || strchr(ChrSig, 'i'))) {
					strcat(TrnPal, ((ClaveModif & SESEO) ? "s" : "T"));
				}
				else if (FinSil && FinPal && (!strcmp(ChrSig, "l") || !strcmp(ChrSig, "r"))) {
					strcat(TrnPal, "G");
				}
				else if (!strcmp(ChrSig, "b") || !strcmp(ChrSig, "d") ||
					!strcmp(ChrSig, "m") || !strcmp(ChrSig, "n") ||
					!strcmp(ChrSig, "~n") || !strcmp(ChrSig, "v")) {
					strcat(TrnPal, "G");
				}
				else if (!strcmp(ChrSig, "g")) {
					Chr = IndexChr(SilSig+1, engine->Letras);
					if (Chr >= 0 && (strchr(engine->Letras[Chr], 'e') || strchr(engine->Letras[Chr], 'i'))) {
						strcat(TrnPal, "G");
					}
					else {
						if (ClaveModif & OCLUS_EXPL) {
							strcat(TrnPal, "kcl");
							if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
							if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
						}
						strcat(TrnPal, "k");
					}
				}
				else {
					if (ClaveModif & OCLUS_EXPL) {
						strcat(TrnPal, "kcl");
						if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
						if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
					}
					strcat(TrnPal, "k");
				}
			}

			else if (!strcmp(ChrAct, "ch")) {
				if (ClaveModif & OCLUS_EXPL) {
					strcat(TrnPal, "tcl");
					if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
				}
				strcat(TrnPal, "tS");
				if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
				if ((ClaveModif & INI_FIN_PAL) && FinPal && FinSil) {
					strcat(TrnPal, ".");
				}
			}

			else if (!strcmp(ChrAct, "d")) {
				if ((ClaveModif & ELIM_D) && !IniPal && FinPal && 
					strchr(ChrSig, '\'') == (char *) 0 && EraVocal && IndexChr(ChrSig, engine->Vocales) >= 0) {
				}
				else if ((ClaveModif & ELIM_D) && FinPal && FinSil && EraVocal) {
				}
				else if (ClaveModif & BDG_ANDES) {
					if (!IniSil || EraVocal) {
						strcat(TrnPal, "D");
					}
					else {
						if (ClaveModif & OCLUS_EXPL) {
							strcat(TrnPal, "dcl");
							if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
							if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
						}
						strcat(TrnPal, "d");
					}
				}
				else if (IniSil && ((IniPal && strlen(ChrAnt) == 0) ||
					!strcmp(ChrAnt, "m") || !strcmp(ChrAnt, "n") || !strcmp(ChrAnt, "l"))) {
					if (ClaveModif & OCLUS_EXPL) {
						strcat(TrnPal, "dcl");
						if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
						if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
					}
					strcat(TrnPal, "d");
				}
				else {
					strcat(TrnPal, "D");
				}
			}

			else if (!strcmp(ChrAct, "f")) {
				strcat(TrnPal, "f");
			}

			else if (!strcmp(ChrAct, "g")) {
				if (FinSil == 0 && (strchr(ChrSig, 'e') || strchr(ChrSig, 'i'))) {
					strcat(TrnPal, "x");
				}
				else if ((ClaveModif & ELIM_G) && !IniPal && FinPal && 
					strchr(ChrSig, '\'') == (char *) 0 && EraVocal && IndexChr(ChrSig, engine->Vocales) >= 0) {
				}
				else if ((ClaveModif & ELIM_G) && FinPal && FinSil && EraVocal) {
				}
				else if (ClaveModif & BDG_ANDES) {
					if (!IniSil || EraVocal) {
						strcat(TrnPal, "G");
					}
					else {
						if (ClaveModif & OCLUS_EXPL) {
							strcat(TrnPal, "gcl");
							if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
							if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
						}
						strcat(TrnPal, "g");
					}
				}
				else if (IniSil && ((IniPal && strlen(ChrAnt) == 0) || !strcmp(ChrAnt, "m") || !strcmp(ChrAnt, "n"))) {
					if (ClaveModif & OCLUS_EXPL) {
						strcat(TrnPal, "gcl");
						if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
						if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
					}
					strcat(TrnPal, "g");
				}
				else {
					strcat(TrnPal, "G");
				}
				if (FinSil == 0 && !strcmp(ChrSig, "u")) {
					Chr = IndexChr(SilAct+Ind+1, engine->Letras);
					if (Chr >= 0 && (strchr(engine->Letras[Chr], 'e') || strchr(engine->Letras[Chr], 'i'))) {
						strcpy(ChrAct, engine->Letras[Chr]);
						Ind++;
					}
				}
			}

			else if (!strcmp(ChrAct, "j")) {
				strcat(TrnPal, "x");
			}

			else if (!strcmp(ChrAct, "k")) {
				if (ClaveModif & OCLUS_EXPL) {
					strcat(TrnPal, "kcl");
					if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
					if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
				}
				strcat(TrnPal, "k");
			}

			else if (!strcmp(ChrAct, "l")) {
				if ((ClaveModif & GRUPO_SIL) &&
					(!IniSil && !FinSil && IndexChr(ChrSig, engine->Vocales) >= 0)) {
					strcat(TrnPal, "@l");
				}
				else {
					strcat(TrnPal, "l");
				}
			}

			else if (!strcmp(ChrAct, "ll")) {
				strcat(TrnPal, "L");
			}

			else if (!strcmp(ChrAct, "m")) {
				if ((ClaveModif & NAS_VELAR) && FinSil) {
					strcat(TrnPal, "N");
				}
				else {
					strcat(TrnPal, "m");
				}
			}

			else if (!strcmp(ChrAct, "n")) {
				if ((ClaveModif & (ENE_VELAR | NAS_VELAR)) && FinSil) {
					strcat(TrnPal, "N");
				}
				else if (!strcmp(ChrSig, "p") || !strcmp(ChrSig, "b") ||
					!strcmp(ChrSig, "v") || !strcmp(ChrSig, "m") ||
					!strcmp(ChrSig, "f")) {
					strcat(TrnPal, "m");
				}
				else if (!strcmp(ChrSig, "g") || !strcmp(ChrSig, "j") ||
					!strcmp(ChrSig, "k") || !strcmp(ChrSig, "qu")) {
					strcat(TrnPal, "N");
				}
				else if (!strcmp(ChrSig, "c")) {
					Chr = IndexChr(SilSig+1, engine->Letras);
					if (Chr >= 0 && !strchr(engine->Letras[Chr], 'e') && !strchr(engine->Letras[Chr], 'i')) {
						strcat(TrnPal, "N");
					}
					else {
						strcat(TrnPal, "n");
					}
				}
				else if (FinSil && !strcmp(ChrSig, "hu")) {
					Chr = IndexChr(SilSig+2, engine->Letras);
					if (Chr >= 0 && strchr(engine->Letras[Chr], 'e')) {
						strcat(TrnPal, "N");
					}
					else {
						strcat(TrnPal, "n");
					}
				}
				else {
					strcat(TrnPal, "n");
				}
			}
			
			else if (!strcmp(ChrAct, "~n")) {
				strcat(TrnPal, "J");
			}

			else if (!strcmp(ChrAct, "p")) {
				if (ClaveModif & OCLUS_EXPL) {
					strcat(TrnPal, "pcl");
					if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
					if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
				}
				strcat(TrnPal, "p");
			}

			else if (!strcmp(ChrAct, "r")) {
				if ((ClaveModif & ERRE_IMPL) && IndexChr(ChrSig, engine->Vocales) < 0) {
					strcat(TrnPal, "R");
				}
				else if ((ClaveModif & ERRE_IMPL) && FinSil && (!strcmp(ChrSig, "hu") || !strcmp(ChrSig, "hi")) && (Chr = IndexChr(SilSig+2, engine->Letras)) >= 0 && IndexChr(engine->Letras[Chr], engine->Vocales) >= 0) {
					strcat(TrnPal, "R");
				}
				else if ((ClaveModif & GRUPO_SIL) &&
					(!IniSil && !FinSil && IndexChr(ChrSig, engine->Vocales) >= 0)) {
					strcat(TrnPal, "@r");
				}
				else if ((IniPal && IniSil) || !strcmp(ChrAnt, "l") ||
					!strcmp(ChrAnt, "n") || !strcmp(ChrAnt, "s")) {
					strcat(TrnPal, "rr");
				}
				else {
					strcat(TrnPal, "r");
				}
			}

			else if (!strcmp(ChrAct, "rr")) {
				strcat(TrnPal, "rr");
			}

			/*
			 * La <x> se empezo a hacer antes, ahora solo queda como s.
			 */
			else if (!strcmp(ChrAct, "s") || !strcmp(ChrAct, "x") || !strcmp(ChrAct, "~c")) {
				SigueEse = 0;
				if (FinSil && !FinPal && !strcmp(ChrSig, "s")) SigueEse = 1;
				else if (FinSil && !FinPal && (ClaveModif & SESEO) && !strcmp(ChrSig, "z")) SigueEse = 1;
				else if (FinSil && !FinPal && (ClaveModif & SESEO) && !strcmp(ChrSig, "c")) {
					Chr = IndexChr(SilSig+strlen(ChrSig), engine->Letras);
					if (Chr >= 0 && (strchr(engine->Letras[Chr], 'e') || strchr(engine->Letras[Chr], 'i'))) SigueEse = 1;
				}

				if (!(IniPal && IniSil) && strcmp(ChrAct, "x") == 0 && (ClaveModif & EQUIS_KS)) {
					if (ClaveModif & OCLUS_EXPL) {
						strcat(TrnPal, "kcl");
						if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
						if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
					}
					strcat(TrnPal, "k");
				}

/*
				if (!strcmp(ChrAct, "x") && (ClaveModif & EQUIS_KS) && FinSil && !FinPal) { 
*/
				if (!strcmp(ChrAct, "x") && (ClaveModif & EQUIS_KS) && FinSil) { 
					if (!SigueEse) {
						strcat(TrnPal, "s");
					}
				}
				else if ((ClaveModif & SC_KS) && FinSil && !FinPal) { 
					if (ClaveModif & OCLUS_EXPL) {
						strcat(TrnPal, "kcl");
						if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
						if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
					}
					strcat(TrnPal, "k");
				}
				else if (SigueEse) { 
					/* Nos comemos la ese */
				}
				else if ((ClaveModif & ESE_ASP_INC) && FinSil) {
					strcat(TrnPal, "h");
				}
				else if ((ClaveModif & ESE_ASP_CON) && FinSil) {
					if (FinPal && FinSil && 
						(IndexChr(ChrSig, engine->Vocales) >= 0 || IndexChr(ChrSig, engine->Letras) < 0 ||
						(!strcmp(ChrSig, "y") && GrpSig.Long == 1))) {
						strcat(TrnPal, "s");
					}
					else {
						strcat(TrnPal, "h");
					}
				}
				else if (!strcmp(ChrSig, "b") || !strcmp(ChrSig, "d") ||
					!strcmp(ChrSig, "m") || !strcmp(ChrSig, "n") ||
					!strcmp(ChrSig, "l") || !strcmp(ChrSig, "ll") ||
					!strcmp(ChrSig, "v") || !strcmp(ChrSig, "w") ||
					!strcmp(ChrSig, "r") || !strcmp(ChrSig, "rr")) {
					strcat(TrnPal, "z");
				}
				else if (!strcmp(ChrSig, "y") && (!FinPal || GrpSig.Long != 1)) {
					strcat(TrnPal, "z");
				}
				else if (!strcmp(ChrSig, "hu") || !strcmp(ChrSig, "hi")) { 
					Chr = IndexChr(SilSig+strlen(ChrSig), engine->Letras);
					if (Chr >= 0 && strchr(engine->Letras[Chr], 'e')) {
						strcat(TrnPal, "z");
					}
					else {
						strcat(TrnPal, "s");
					}
				}
				else if (!strcmp(ChrSig, "hu")) { 
					Chr = IndexChr(SilSig+strlen(ChrSig), engine->Letras);
					if (Chr >= 0 && strchr(engine->Letras[Chr], 'a')) {
						strcat(TrnPal, "z");
					}
					else {
						strcat(TrnPal, "s");
					}
				}
				else if (!strcmp(ChrSig, "g")) {
					Chr = IndexChr(SilSig+1, engine->Letras);
					if (Chr >= 0 && (strchr(engine->Letras[Chr], 'e') || strchr(engine->Letras[Chr], 'i'))) {
						strcat(TrnPal, "s");
					}
					else {
						strcat(TrnPal, "z");
					}
				}
				else {
					strcat(TrnPal, "s");
				}
			}

			else if (!strcmp(ChrAct, "t")) {
				if (FinSil) {
					if ((strlen(TrnPal) > 0) && (TrnPal[strlen(TrnPal)-1] == 's')) {
						TrnPal[strlen(TrnPal)-1] = 'z';
					}
					if (!FinPal && (!strcmp(ChrAnt, "m") || !strcmp(ChrAnt, "n") || !strcmp(ChrAnt, "l"))) {
						if (ClaveModif & OCLUS_EXPL) {
							strcat(TrnPal, "dcl");
							if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
							if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
						}
						strcat(TrnPal, "d");
					}
					else if (FinPal) {
						strcat(TrnPal, "t");
						if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
						if ((ClaveModif & INI_FIN_PAL) && FinPal && FinSil) strcat(TrnPal, ".");
					}
					else {
						strcat(TrnPal, "D");
					}
				}
				else {
					if (ClaveModif & OCLUS_EXPL) {
						strcat(TrnPal, "tcl");
						if ((ClaveModif & MARCA_IMPL) && !IniSil && (EraVocal || FinSil)) strcat(TrnPal, ":");
						if ((ClaveModif & INI_FIN_PAL) && IniPal && IniSil) strcat(TrnPal, ".");
					}
					strcat(TrnPal, "t");
				}
			}

			else if (!strcmp(ChrAct, "z")) {
				if (ClaveModif & SESEO) {
					if ((ClaveModif & ESE_ASP_INC) && FinSil) {
						strcat(TrnPal, "h");
					}
					else if ((ClaveModif & ESE_ASP_CON) && FinSil) {
						if (FinPal && FinSil && 
							(IndexChr(ChrSig, engine->Vocales) >= 0 || IndexChr(ChrSig, engine->Letras) < 0 ||
							(!strcmp(ChrSig, "y") && GrpSig.Long == 1))) {
							strcat(TrnPal, "s");
						}
						else {
							strcat(TrnPal, "h");
						}
					}
					else if (!strcmp(ChrSig, "b") || !strcmp(ChrSig, "d") ||
						!strcmp(ChrSig, "m") || !strcmp(ChrSig, "n") ||
						!strcmp(ChrSig, "l") || !strcmp(ChrSig, "ll") ||
						!strcmp(ChrSig, "v") || !strcmp(ChrSig, "w") ||
						!strcmp(ChrSig, "r") || !strcmp(ChrSig, "rr")) {
						strcat(TrnPal, "z");
					}
					else if (!strcmp(ChrSig, "y") && (!FinPal || GrpSig.Long != 1)) {
						strcat(TrnPal, "z");
					}
					else if (!strcmp(ChrSig, "hu")) { 
						Chr = IndexChr(SilSig+strlen(ChrSig), engine->Letras);
						if (Chr >= 0 && strchr(engine->Letras[Chr], 'e')) {
							strcat(TrnPal, "z");
						}
						else {
							strcat(TrnPal, "s");
						}
					}
					else if (!strcmp(ChrSig, "g")) {
						Chr = IndexChr(SilSig+1, engine->Letras);
						if (Chr >= 0 && (strchr(engine->Letras[Chr], 'e') || strchr(engine->Letras[Chr], 'i'))) {
							strcat(TrnPal, "s");
						}
						else {
							strcat(TrnPal, "z");
						}
					}
					else {
						strcat(TrnPal, "s");
					}
				}
				else {
					strcat(TrnPal, "T");
				}
			}

			/*
			 * Sonidos especificos de dialectos.
			 */
			else if (!strcmp(ChrAct, "tz")) {
				strcat(TrnPal, "ts");
			}

			else if (!strcmp(ChrAct, "tl")) {
				if (FinPal && FinSil) {
					strcat(TrnPal, "tL");
					if (ClaveModif & MARCA_IMPL) strcat(TrnPal, ":");
				}
				else {
					if (ClaveModif & OCLUS_EXPL) {
						strcat(TrnPal, "tcl");
					}
					strcat(TrnPal, "t");
					if (ClaveModif & GRUPO_SIL) strcat(TrnPal, "@");
					strcat(TrnPal, "l");
				}
			}

			if ((ClaveModif & VOCAL_PTON) && strchr("aeiou", TrnPal[strlen(TrnPal) - 1]) && TrnPal[strlen(TrnPal) - 2] != '\'') {
				char	*Acento;
				if ((Acento = strchr(GrpAct.Cont, '\'')) && Acento - GrpAct.Cont < Pos) {
					strcat(TrnPal, "_");
				}
			}
					
			if ((ClaveModif & MARCA_IMPL) && !EsVocal && !IniSil && (EraVocal || EraSemi || FinSil) && (strlen(TrnPal) > 0) && TrnPal[strlen(TrnPal) - 1] != ':') {
				if (IndexChr(ChrSig, engine->Vocales) < 0) { 
					strcat(TrnPal, ":");
				}
				else if (EraVocal && strchr("jw", TrnPal[strlen(TrnPal) - 1]) && strcmp("jj", TrnPal + strlen(TrnPal) - 2)) { 
					strcat(TrnPal, ":");
				}
				else if (FinSil && (!strcmp(ChrSig, "hu") || !strcmp(ChrSig, "hi"))) {
					Chr = IndexChr(SilSig+2, engine->Letras);
					if (Chr >= 0 && IndexChr(engine->Letras[Chr], engine->Vocales) >= 0) {
						strcat(TrnPal, ":");
					}
				}
			}

			if ((ClaveModif & VOCAL_NASAL) && EsVocal && IndexChr(ChrSig, Nasales) >= 0) {
				if (IndexChr(ChrAnt, Nasales) >= 0 || IndexChr(ChrAnt, engine->Letras) < 0) { 
					strcat(TrnPal, "~");
				}
			}

			if ((ClaveModif & INI_FIN_PAL) && FinPal && FinSil) {
				strcat(TrnPal, ".");
			}

			strcpy(ChrAnt, ChrAct);
			IniSil = 0;
			EraVocal = EsVocal;
			EraSemi = EsSemi;
		}

		IniPal = 0;
	}

	Pointer = TrnPal;
	TrnPal = strdup(TrnPal);
	free((void *) Pointer);

	return TrnPal;
}

/***********************************************************************
 * TrnFonFnm - Realiza la transcripcion en fonemas.
 **********************************************************************/

char	*TrnFonFnm(	char	*TrnFon,
					int		ConSil, char **Fonemas)
{
	char	*TrnFnm, *Pointer;
	int		LongFon = strlen(TrnFon);
	int		Chr, Pos, PosAct, Espacio;
	GRP_ORT	GrpAct;

	/*
	 * Ubicamos memoria para albergar el doble mas uno de la longitud de
	 * TrnFon. Esto garantiza que cabra cualquier transcripcion.
	 */
	if ((TrnFnm = (char *) calloc((size_t) (20*LongFon+1), sizeof(char))) == (char *) 0) {
		fprintf(stderr, "Error al ubicar memoria para TrnFnm\n");
		return (char *) 0;
	}

	/*
	 * Bucle para todos los grupos foneticos.
	 */
	Espacio = 1;
	PosAct = 0;
	while (PosAct < LongFon) {
		/*
		 * Cogemos el siguiente grupo fonetico de TrnFon.
		 */
		if (CogeGrpFon(TrnFon, PosAct, &GrpAct, Fonemas) < 0) {
			fprintf(stderr, "Error al localizar el siguiente grupo de SilOrt\n");
			free(TrnFnm);
			return NULL;
		}

		if (GrpAct.Tipo & PAL_FON) {
			Pos = 0;
			while (Pos < (int) GrpAct.Long) {
				if ((Chr = IndexChr(GrpAct.Cont+Pos, Fonemas)) < 0) {
					if ((Chr = IndexChr(GrpAct.Cont+Pos, InterSil)) < 0) {
						free(TrnFnm);
						return NULL;
					}
					else {
						Pos += strlen(InterSil[Chr]);
						continue;
					}
				}
				if (Espacio == 0) strcat(TrnFnm, " ");
				strcat(TrnFnm, Fonemas[Chr]);
				Espacio = 0;
				Pos += strlen(Fonemas[Chr]);
			}
		}
		else if (GrpAct.Tipo & ESP_FON) {
			if (Espacio == 0) strcat(TrnFnm, " ");
			Espacio = 1;
		}
		else if (GrpAct.Tipo & SIL_FON) {
			if (Espacio == 0) strcat(TrnFnm, " ");
			Espacio = 1;
			if (ConSil != 0) {
				strncat(TrnFnm, GrpAct.Cont, GrpAct.Long);
				Espacio = 0;
			}
		}
		else {
			if (GrpAct.Cont[0] == '\n') {
				Espacio = 1;
			}
			else {
				if (Espacio == 0) strcat(TrnFnm, " ");
				Espacio = 0;
			}
			strncat(TrnFnm, GrpAct.Cont, GrpAct.Long);
		}

		PosAct += GrpAct.Long;
	}

	Pointer = TrnFnm;
	TrnFnm = strdup(TrnFnm);
	free((void *) Pointer);

	return TrnFnm;
}

/***********************************************************************
 * TrnFonFnmPal - Realiza la transcripcion en fonemas por palabras.
 **********************************************************************/

char	*TrnFonFnmPal(	char	*TrnFon,
					int		ConSil, char **Fonemas)
{
	char	*TrnFnm, *Pointer;
	int		LongFon = strlen(TrnFon);
	int		Chr, Pos, PosAct, Espacio;
	GRP_ORT	GrpAct;

	/*
	 * Ubicamos memoria para albergar el doble mas uno de la longitud de
	 * TrnFon. Esto garantiza que cabra cualquier transcripcion.
	 */
	if ((TrnFnm = (char *) calloc((size_t) 20 * LongFon, sizeof(char))) == (char *) 0) {
		fprintf(stderr, "Error al ubicar memoria para TrnFnm\n");
		return NULL;
	}

	/*
	 * Bucle para todos los grupos foneticos.
	 */
	Espacio = 1;
	PosAct = 0;
	while (PosAct < LongFon) {
		/*
		 * Cogemos el siguiente grupo fonetico de TrnFon.
		 */
		if (CogeGrpFon(TrnFon, PosAct, &GrpAct, Fonemas) < 0) {
			fprintf(stderr, "Error al localizar el siguiente grupo de SilOrt\n");
			free(TrnFnm);
			return NULL;
		}

		if (PosAct > 0 && ((GrpAct.Tipo & ESP_FON) == 0 && (ConSil || (GrpAct.Tipo & (PUNT_FON | SIL_FON)) == 0))) {
			if (Espacio == 0) strcat(TrnFnm, " ");
			strcat(TrnFnm, "/");
			Espacio = 0;
		}

		if (GrpAct.Tipo & PAL_FON) {
			Pos = 0;
			while (Pos < (int) GrpAct.Long) {
				if ((Chr = IndexChr(GrpAct.Cont+Pos, Fonemas)) < 0) {
					if ((Chr = IndexChr(GrpAct.Cont+Pos, InterSil)) < 0) {
						free(TrnFnm);
						return NULL;
					}
					else {
						if (Espacio == 0) strcat(TrnFnm, " ");
						strcat(TrnFnm, InterSil[Chr]);
						Espacio = 0;
						Pos += strlen(InterSil[Chr]);
						continue;
					}
				}
				if (Espacio == 0) strcat(TrnFnm, " ");
				strcat(TrnFnm, Fonemas[Chr]);
				Espacio = 0;
				Pos += strlen(Fonemas[Chr]);
			}
		}
		else if (GrpAct.Tipo & ESP_FON) {
			if (Espacio == 0) strcat(TrnFnm, " ");
			Espacio = 1;
		}
		else if (GrpAct.Tipo & SIL_FON) {
			if (Espacio == 0) strcat(TrnFnm, " ");
			Espacio = 1;
			if (ConSil != 0) {
				strncat(TrnFnm, GrpAct.Cont, GrpAct.Long);
				Espacio = 0;
			}
		}
		else {
			if (Espacio == 0) strcat(TrnFnm, " ");
			strncat(TrnFnm, GrpAct.Cont, GrpAct.Long);
			Espacio = 0;
		}

		PosAct += GrpAct.Long;
	}

	Pointer = TrnFnm;
	TrnFnm = strdup(TrnFnm);
	free(Pointer);

	return TrnFnm;
}

/***********************************************************************
 * TrnFonSefo - Realiza la transcripcion en semifonemas.
 **********************************************************************/

char	*TrnFonSefo(char	*TrnFon,
					int		ConSil,
					const char	*StrIniPal,
					const char	*StrFinPal, char **Fonemas)
{
	char	*TrnSefo, *Pointer;
	int		LongFon = strlen(TrnFon);
	int		Chr, ChrAnt, Pos, PosAct, Espacio;
	GRP_ORT	GrpAct;

	/*
	 * Ubicamos memoria para albergar diez veces la longitud de
	 * TrnFon. Esto garantiza que cabra cualquier transcripcion.
	 */
	if ((TrnSefo = (char *) calloc((size_t) (20*LongFon+1), sizeof(char))) == (char *) 0) {
		fprintf(stderr, "Error al ubicar memoria para TrnSefo\n");
		return (char *) 0;
	}

	/*
	 * Bucle para todos los grupos foneticos.
	 */
	Espacio = 1;
	PosAct = 0;
	while (PosAct < LongFon) {
		/*
		 * Cogemos el siguiente grupo fonetico de TrnFon.
		 */
		if (CogeGrpFon(TrnFon, PosAct, &GrpAct, Fonemas) < 0) {
			fprintf(stderr, "Error al localizar el siguiente grupo de SilOrt\n");
			free(TrnSefo);
			return NULL;
		}

		if (GrpAct.Tipo & PAL_FON) {
			Pos = 0;
			if (Espacio == 0) strcat(TrnSefo, " ");

			if ((ChrAnt = IndexChr(GrpAct.Cont+Pos, Fonemas)) < 0) {
				free(TrnSefo);
				return NULL;
			}
			strcat(TrnSefo, StrIniPal);
			strcat(TrnSefo, Fonemas[ChrAnt]);
			strcat(TrnSefo, " ");
			strcat(TrnSefo, Fonemas[ChrAnt]);
			Pos += strlen(Fonemas[ChrAnt]);
			while (Pos < (int) GrpAct.Long) {
				if ((Chr = IndexChr(GrpAct.Cont+Pos, Fonemas)) < 0) {
					if ((Chr = IndexChr(GrpAct.Cont+Pos, InterSil)) < 0) {
						free(TrnSefo);
						return NULL;
					}
					else {
						Pos += strlen(InterSil[Chr]);
						continue;
					}
				}
				strcat(TrnSefo, "+");
				strcat(TrnSefo, Fonemas[Chr]);
				strcat(TrnSefo, " ");
				strcat(TrnSefo, Fonemas[ChrAnt]);
				strcat(TrnSefo, "-");
				strcat(TrnSefo, Fonemas[Chr]);
				strcat(TrnSefo, " ");
				strcat(TrnSefo, Fonemas[Chr]);
				ChrAnt = Chr;
				Pos += strlen(Fonemas[Chr]);
			}
			strcat(TrnSefo, StrFinPal);
			Espacio = 0;
		}
		else if (GrpAct.Tipo & ESP_FON) {
			if (Espacio == 0) strcat(TrnSefo, " ");
			Espacio = 1;
		}
		else if (GrpAct.Tipo & SIL_FON) {
			if (Espacio == 0) strcat(TrnSefo, " ");
			Espacio = 1;
			if (ConSil != 0) {
				strncat(TrnSefo, GrpAct.Cont, GrpAct.Long);
				Espacio = 0;
			}
		}
		else {
			if (Espacio == 0) strcat(TrnSefo, " ");
			strncat(TrnSefo, GrpAct.Cont, GrpAct.Long);
			Espacio = 0;
		}

		PosAct += GrpAct.Long;
	}

	Pointer = TrnSefo;
	TrnSefo = strdup(TrnSefo);
	free((void *) Pointer);

	return TrnSefo;
}

/***********************************************************************
 * TrnFonSem - Realiza la transcripcion en semisilabas.
 **********************************************************************/

char	*TrnFonSem(	char	*TrnFon,
					int		ConSil, char **Fonemas)

{
	char	*TrnSem, *Pointer;
	int		LongFon = strlen(TrnFon);
	int		Chr, Voc, Pos, PosAct, Espacio;
	GRP_ORT	GrpAct;

	/*
	 * Ubicamos memoria para albergar el doble mas uno de la longitud de
	 * TrnFon. Esto garantiza que cabra cualquier transcripcion.
	 */
	if ((TrnSem = (char *) calloc((size_t) (20*LongFon+1), sizeof(char))) == (char *) 0) {
		fprintf(stderr, "Error al ubicar memoria para TrnSem\n");
		return (char *) 0;
	}

	strcpy(TrnSem, "");

	/*
	 * Bucle para todos los grupos foneticos.
	 */
	Espacio = 1;
	PosAct = 0;
	while (PosAct < LongFon) {
		/*
		 * Cogemos el siguiente grupo fonetico de TrnFon.
		 */
		if (CogeGrpFon(TrnFon, PosAct, &GrpAct, Fonemas) < 0) {
			free((void *) TrnSem);
			return (char *) 0;
		}

		if (GrpAct.Tipo & PAL_FON) {
			if (Espacio == 0) strcat(TrnSem, " ");
			Pos = 0;
			while (Pos < (int) GrpAct.Long) {
				if ((Chr = IndexChr(GrpAct.Cont+Pos, Fonemas)) < 0) {
					if ((Chr = IndexChr(GrpAct.Cont+Pos, InterSil)) < 0) {
						free((void *) TrnSem);
						return (char *) 0;
					}
					else {
						strcat(TrnSem, " ");
						Pos += strlen(InterSil[Chr]);
						continue;
					}
				}
				if ((Voc = IndexChr(GrpAct.Cont+Pos, FonVoc)) < 0) {
					strcat(TrnSem, Fonemas[Chr]);
					Pos += strlen(Fonemas[Chr]);
					continue;
				}
				if (FonVoc[Voc][0] == '\'') {
					strcat(TrnSem, Fonemas[Chr]+1);
				}
				else {
					strcat(TrnSem, Fonemas[Chr]);
				}
				strcat(TrnSem, "- -");
				strcat(TrnSem, Fonemas[Chr]);

				Pos += strlen(Fonemas[Chr]);
			}
			Espacio = 0;
		}
		else if (GrpAct.Tipo & ESP_FON) {
			if (Espacio == 0) strcat(TrnSem, " ");
			Espacio = 1;
		}
		else if (GrpAct.Tipo & SIL_FON) {
			if (Espacio == 0) strcat(TrnSem, " ");
			Espacio = 1;
			if (ConSil != 0) {
				strncat(TrnSem, GrpAct.Cont, GrpAct.Long);
				Espacio = 0;
			}
		}
		else {
			if (Espacio == 0) strcat(TrnSem, " ");
			strncat(TrnSem, GrpAct.Cont, GrpAct.Long);
			Espacio = 0;
		}

		PosAct += GrpAct.Long;
	}

	Pointer = TrnSem;
	TrnSem = strdup(TrnSem);
	free((void *) Pointer);

	return TrnSem;
}
