/* Saga - Un transcriptor fon�tico para el idioma espa�ol
 *
 * Copyright (C) 1993-2009  Albino Nogueiras Rodr�guez y Jos� B. Mari�o
 *       TALP - Universitat Polit�cnica de Catalunya, ESPA�A
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

#ifndef _MAX_PATH
#define _MAX_PATH	16000
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && !defined(__attribute__)
#define __attribute__(A)
#endif

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

#define SESEO		0x00001
#define EQUIS_KS	0x00002
#define SC_KS		0x00004
#define ESE_ASP_INC	0x00008
#define ESE_ASP_CON	0x00010
#define BDG_ANDES	0x00020
#define ELIM_B		0x00040
#define ELIM_D		0x00080
#define ELIM_G		0x00100
#define ENE_VELAR	0x00200
#define NAS_VELAR	0x00400
#define ARCHI_IMPL	0x00800
#define Y_VOCAL		0x01000
#define ERRE_IMPL	0x02000
#define GRUPO_SIL	0x04000
#define MARCA_IMPL	0x08000
#define VOCAL_NASAL	0x10000
#define VOCAL_PTON	0x20000
#define OCLUS_EXPL	0x40000
#define INI_FIN_PAL	0x80000

typedef struct struct_SagaEngine {
  /* Argumentos de entrada (linea de comandos) */
  int TrnPalAis; /* Palabras aisladas? */
  int TrnLinAis; /* Lineas aisladas? */
  int ConSil; /* Conservar los silencios */
  const char *StrIniPal; /* Marca inicio palabra */
  const char *StrFinPal; /* Marca fin de palabra */
  const char *FicDicExc; /* Nombre fichero Diccionario excepciones */
  const char *FicTrnFon; /* Nombre fichero Diccionario transcripcion de grafemas */
  const char *FicTrnPal; /* Nombre fichero Diccionario transcripcion de palabras */
  const char *FicDicSust; /* Nombre fichero Diccionario de substitucion de fonemas */
  const char *FicDicGrp; /* Nombre fichero Diccionario de substitucion de grupos foneticos */
  const char *FicNovVoc; /* Nombre fichero Lista de grafemas vocalicos introducidos */
  const char *FicNovCons; /* Nombre fichero Lista de grafemas consonanticos introducidos */
  const char *FicNovFon; /* Nombre fichero Lista de fonemas introducidos */
  long ClaveModif; /* Incluye las opciones -M de la linea de comandos */

  /* Diccionarios cargados */
  char ***DicExc;
  char ***DicTrnFon;
  char ***DicTrnPal;
  char ***DicSust;
  char ***DicGrp;

  /* Listas de nuevos fonemas */
  char **LisNovVoc;
  char **LisNovCons;
  char **LisNovFon;
  /* Contenido de las listas */
  char **Letras;
  char **Fonemas;
  char **ConsTxt;
  char **Vocales;

  /* Opciones de salida */
  int SalFon; /* Transcripcion fonetica */
  int SalFnm; /* Transcripcion en fonemas */
  int SalFnmPal; /* En fonemas por palabras */
  int SalSefo; /* En semifonemas */
  int SalSem; /* En semisilabas */

  /* Salida */
  char* TxtSalFon; /* Transcripcion fonetica */
  char* TxtSalFnm; /* Transcripcion en fonemas */
  char* TxtSalFnmPal; /* En fonemas por palabras */
  char* TxtSalSefo; /* En semifonemas */
  char* TxtSalSem; /* En semisilabas */

} SagaEngine;


static char	*_Letras[] __attribute__((unused)) = {
	"a", "'a", "ha", "h'a", "b", "c", "ch", "d", "e", "'e", "he",
	"�", "h�", "�", "h�", "�", "h�", "�", "h�", "�", "h�", 
	"h'e", "f", "g", "h", "i", "'i", "hi", "h'i", "j", "k", "l", "ll",
	"m", "n", "~n", "o", "'o", "ho", "h'o", "p", "qu", "r", "rr", "s",
	"t", "u", "'u", "~u", "hu", "h'u", "v", "w", "x", "y", "z", "tl",
	"tz", "&", (char *) 0};
static char	*_Vocales[] __attribute__((unused)) = {
	"a", "e", "i", "o", "u", "a_", "e_", "i_", "o_", "u_", "'a", "'e", "'i", "'o", "'u", "~u",
	"�", "h�", "�", "h�", "�", "h�", "�", "h�", "�", "h�", 
	"ha", "h'a", "he", "h'e", "hi", "h'i", "ho", "h'o", "hu", "h'u", (char *) 0};
static char	*VocDeb[] __attribute__((unused)) = {
	"�", "h�", "�", "h�", "i", "u", "~u", "hi", "hu", (char *) 0};
static char	*VocFort[] __attribute__((unused)) = {
	"�", "h�", "�", "h�", "�", "h�",
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
	",", ";", ".", "...", ":", "(", ")", "!", "�", "'!", "?", "�", "'?", "#", ";", "*", "~", (char *) 0};
static char	*Espacios[] __attribute__((unused)) = {
	" ", "\t", "\"", "\n", "\r\n", (char *) 0};
static char	*InterSil[] __attribute__((unused)) = {
	"-", (char *) 0};
static char	*SilFon[] __attribute__((unused)) = {
	",", ";", ".", "...", ":", "(", ")", "!", "�", "'!", "?", "�", "'?", "~", "*", (char *) 0};
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
void	EmpleoSaga(char **ArgV);
int		OpcSaga(int ArgC, char	**ArgV, SagaEngine *engine, char **NomIn, char **NomOut);

char	*CargTxtOrt(int TrnLinAis);
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

