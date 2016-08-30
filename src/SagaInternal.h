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

#ifndef SAGAINTERNAL_H
#define SAGAINTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && !defined(__attribute__)
#define __attribute__(A)
#endif

#include <stdio.h>
#include "Saga.h"

typedef struct {
	size_t	Long;
	int		Tipo;
	char	*Cont;
} GRP_ORT;

#define PALABRA		0x0001
#define LITERAL		0x0002
#define SILABA		0x0004
#define	SILENCIO	0x0008
#define	INTER_PAL	0x0010
#define	INTER_SIL	0x0020
#define	FIN_FRASE	0x0040
#define	LETRAS		0x0080
#define	ACENTOS		0X0100
#define	ESPACIOS	0X0200
#define	DESCONOCIDO	0X0400
#define	PAL_FON		0X0800
#define	SIL_FON		0X1000
#define	ESP_FON		0X2000
#define	PUNT_FON	0X4000


static char	*_Letras[] __attribute__((unused)) = {
	"a", "'a", "ha", "h'a", "b", "c", "ch", "d", "e", "'e", "he",
	"á", "há", "é", "hé", "í", "hí", "ó", "hó", "ú", "hú", 
	"h'e", "f", "g", "h", "i", "'i", "hi", "h'i", "j", "k", "l", "ll",
	"m", "n", "~n", "o", "'o", "ho", "h'o", "p", "qu", "r", "rr", "s",
	"t", "u", "'u", "~u", "hu", "h'u", "v", "w", "x", "y", "z", "tl",
	"tz", "&", (char *) 0};
static char	*_Vocales[] __attribute__((unused)) = {
	"a", "e", "i", "o", "u", "a_", "e_", "i_", "o_", "u_", "'a", "'e", "'i", "'o", "'u", "~u",
	"á", "há", "é", "hé", "í", "hí", "ó", "hó", "ú", "hú", 
	"ha", "h'a", "he", "h'e", "hi", "h'i", "ho", "h'o", "hu", "h'u", (char *) 0};
static char	*VocDeb[] __attribute__((unused)) = {
	"í", "hí", "ú", "hú", "i", "u", "~u", "hi", "hu", (char *) 0};
static char	*VocFort[] __attribute__((unused)) = {
	"á", "há", "é", "hé", "ó", "hó",
	"a", "e", "o", "'a", "'e", "'i", "'o", "'u", "ha", "h'a", "he", "h'e", "h'i", "ho", "h'o", "h'u", (char *) 0};
static char	*_ConsTxt[] __attribute__((unused)) = {
	"b", "c", "ch", "d", "f", "g", "h", "j", "k", "l", "ll", "m", "n", "~n", "p", "qu", "r", "rr", "s", "t", "v", "w", "x",
	"y", "z", "tl", "tz", (char *) 0};
static char	*Nasales[] __attribute__((unused)) = { 
	"m", "n", "~n", (char *) 0};
static char	*ConsL[] __attribute__((unused)) = {
	"b", "c", "k", "f", "g", "p", "t", (char *) 0};
static char	*ConsR[] __attribute__((unused)) = {
	"b", "c", "k", "d", "f", "g", "p", "t", (char *) 0};
static char	*Silen[] __attribute__((unused)) = {
	",", ";", ".", "...", ":", "(", ")", "!", "¡", "'!", "?", "¿", "'?", "#", ";", "*", "~", (char *) 0};
static char	*Espacios[] __attribute__((unused)) = {
	" ", "\t", "\"", "\n", "\r\n", (char *) 0};
static char	*InterSil[] __attribute__((unused)) = {
	"-", (char *) 0};
static char	*SilFon[] __attribute__((unused)) = {
	",", ";", ".", "...", ":", "(", ")", "!", "¡", "'!", "?", "¿", "'?", "~", "*", (char *) 0};
static char	*EspFon[] __attribute__((unused)) = {
	" ", "\t", "\"", (char *) 0};
static char	*PuntFon[] __attribute__((unused)) = {
	"\n", "\r\n", "#", (char *) 0};

static char	*_Fonemas[] __attribute__((unused)) = {
	"a", "e", "i", "o", "u", "'a", "'e", "'i", "'o", "'u", "a_", "e_", "i_", "o_", "u_", "j", "w",
	"b", "B", "d", "D", "g", "G", "p", "t", "k", "tS", "bcl", "dcl", "gcl", "pcl", "tcl", "kcl", "T", "f", "s", "z", "h", "x",
	"m", "n", "J", "N", "l", "@l", "L", "r", "rr", "R", "@r", "jj", "tL", "ts", (char *) 0};
static char	*FonCns[] __attribute__((unused)) = {
	"b", "B", "d", "D", "g", "G", "p", "t", "k", "bcl", "dcl", "gcl", "pcl", "tcl", "kcl", "tS",
	"B:.", "D:.", "G:.", "p:.", "t:.", "tS:.", "k:.", "pcl:.", "tcl:.", "kcl:.",
	"B.", "D.", "G.", "p.", "t.", "k.", "pcl.", "tcl.", "kcl.",
	"b:", "B:", "d:", "D:", "g:", "G:", "p:", "t:", "tS:", "k:", "bcl:", "dcl:", "gcl:", "pcl:", "tcl:", "kcl:",
	".b", ".B", ".d", ".D", ".g", ".G", ".p", ".t", ".k", ".bcl", ".dcl", ".gcl", ".pcl", ".tcl", ".kcl", ".tS",
	"T", "f", "s", "z", "h", "x", ".T", ".f", ".s", ".z", ".h", ".x",
	"T.", "f.", "s.", "z.", "h.", "x.", "T:.", "f:.", "s:.", "z:.", "h:.", "x:.",
	"T:", "f:", "s:", "z:", "h:", "x:", ".jj.",
	".m", ".n", ".J", ".l", ".L", ".rr", ".jj", ".ts",
	"m:", "n:", "N:", "l:", "r:", "R:", "jj:", "tL:", "ts:", "L:", "L:.", "tS:.",
	"m:.", "n:.", "N:.", "l:.", "r:.", "R:.", "jj:.", "tL:.", "ts:.",
	"m.", "n.", "N.", "l.", "r.", "R.", "jj.", "tL.", "ts.",
	"m", "n", "J", "N", "l", "@l", "L", "r", "rr", "R", "@r", "jj", "tL", "ts", (char *) 0};
static char	*FonSem[] __attribute__((unused)) = {
	"j", "j:", ".j", "j:.", ".j:.", ".j.", "j.", "w", "w:", ".w", "w:.", ".w:.", ".w.", "w.", (char *) 0};
static char	*FonVoc[] __attribute__((unused)) = {
	"a", "e", "i", "o", "u", "'a", "'e", "'i", "'o", "'u",
	".a.", ".e.", ".i.", ".o.", ".u.", ".'a.", ".'e.", ".'i.", ".'o.", ".'u.",
	".a", ".e", ".i", ".o", ".u", ".'a", ".'e", ".'i", ".'o", ".'u",
	"a.", "e.", "i.", "o.", "u.", "'a.", "'e.", "'i.", "'o.", "'u.",
	"a_", "e_", "i_", "o_", "u_", "a_.", "e_.", "i_.", "o_.", "u_.", (char *) 0};

char	*ArreglaTxt(char *TxtOrt);

char *CargTxtOrtChar(const char *txtin, intptr_t *TxtInOffset, int TrnLinAis);
char	*CargTxtOrt(FILE* fpin, int TrnLinAis);
char	*SilaTxtOrt(char	*TxtOrt, char	***DicTrnPal, SagaEngine *engine);
int		CogeGrpOrt(	char	*TxtOrt,
					int		PosAct,
					GRP_ORT	*GrpAct, char **Letras);
int		CogeGrpFon(char	*TrnFon,
					int		PosAct,
					GRP_ORT	*GrpAct, char **Fonemas);
char	**CogePalExt(	char	*TxtOrt,
						char	**PalExt,
						char	***DicExc,
						char	***DicTrn,
						char **ConsTxt, 
						char **Vocales, char **Letras);
int		EsPalExt(GRP_ORT	GrpOrt, char ** ConstTxt, char ** Vocales);
char	*SilaPalOrt(GRP_ORT	GrpOrt, SagaEngine *engine);
char	*AcenSilOrt(char	*SilOrt, char	***DicTrnPal, SagaEngine *engine);
char	*AcenPalSil(GRP_ORT	GrpOrt, SagaEngine *engine);
int		EsExcAcc(	char	*SilOrt,
					int		PosAct,
					char	**LisExc, char **Letras);
int		AcenSil(char *Sil, SagaEngine *engine);
char	*TrnSilAcc(	char	*SilAcc,
					char	***DicTrnPal,
					char	***DicTrnFon,
					int		TrnPalAis,
					long	ClaveModif,
					SagaEngine *engine);
char	*TrnPalSil(	GRP_ORT	GrpAnt,
					GRP_ORT	GrpAct,
					GRP_ORT	GrpSig,
					char	***DicTrnFon,
					int		TrnPalAis,
					long	ClaveModif,
					SagaEngine *engine);
int		IndexChr(	const char	* Chr,
					char	** VectChr);
char	*TrnFonFnm(	char	*TrnFon,
					int		ConSil, char **Fonemas);
char	*TrnFonFnmPal(	char	*TrnFon,
					int		ConSil, char **Fonemas);
char	*TrnFonSem(	char	*TrnFon,
					int		ConSil, char **Fonemas);
char	*TrnFonSefo(char *TrnFon,
					int		ConSil,
					const char	*StrFinPal,
					const char	*StrIniPal, char **Fonemas);
int		EscrPalExt(char	**PalExt);
char	***CargDicExc(const char *FicDicExc);
void BorraDicExc(char ***DicExc);
int		AplDicExc(	char	***DicExc,
					char	**TxtOrt, char **Letras);
int		AplDicSust(	char	***DicSust,
					char	**TrnFon, 
                           char **Fonemas);
int		AplDicGrp(	char	***DicExc,
					char	**TxtOrt, char **Fonemas);

#ifdef __cplusplus
}
#endif

#endif /* SAGA_INTERNAL_H */

