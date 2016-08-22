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

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<math.h>
#include	<limits.h>
#include	<time.h>
#include	"Util.h"

/***********************************************************************
 * CaseInsensitiveName - Construye el nombre reemplazando mayusculas 
 por mayusculas_ sin tener en cuenta el directorio ni la extensión.
 También reemplaza '<' por '(' y '>' por ')'
 Ejemplos: 
	/vGhNj/adf/FGjkJ.hmm    -> /vGhNj/adf/F_G_jkJ_.hmm
	/vGhNj/adf/<pause>_49.hmm -> /vGhNj/adf/(pause)_49.hmm
 **********************************************************************/
int CaseInsensitiveName(const char* src, char* dst, size_t dpmax)
{
	int sp, ext = 0;
	size_t dp;

	/* check dpmax */
	if (dpmax == 0)
		return -1;

	/* reset dst */
	strcpy(dst,"");

	/* search directory */
	for (sp=strlen(src)-1; sp>=0; sp--) {
		char c = src[sp];
		if ((c == '\\') || (c == '/') || (c == ':'))
			break;
	}
	dp = ++sp;

	/* check limits */
	if (dp>=dpmax) 
		return -1;

	strncpy(dst, src, dp);
	for (; src[sp] && dp<dpmax; sp++, dp++) {
		char c = src[sp];
		if (c == '<') {
			dst[dp] = '(';
		}
		else if (c == '>') {
			dst[dp] = ')';
		}
		else {
			dst[dp] = c;
		}
		if (!ext && (c >= 'A') && (c <= 'Z') && (dp<dpmax-1)) {
			dst[++dp] = '_';
		}
/*
		if (c == '.')
			ext = 1;
*/
	}
	if (dp < dpmax) {
		dst[dp] = 0;
		return 0;
	}
	else {
		dst[0] = 0;
		return -1;
	}
}


#if !defined(NO_PROTOTYPE)

int   IsAbsPath(char	*Path)

#else						/* defined (__STDC__)					*/
int	IsAbsPath(Path)
char	*Path;
#endif						/* defined (__STDC__)					*/

{

   if (Path == NULL || Path[0] == '\0')
      return -1;


#if defined(WIN32) || defined(_WINDOWS)
   return Path[0] == '/'  ||
          Path[0] == '\\' ||
          (Path[1] == ':' && Path[2] == '/') ||
          (Path[1] == ':' && Path[2] == '\\');

#else
   return Path[0] == '/';
#endif

}



/***********************************************************************
 * PathName - Construye el nombre completo a partir del path y el nombre
 **********************************************************************/

#if defined(__STDC__) && !defined(NO_PROTOTYPE)

void	PathName(	const char	*Path,
					const char	*Name,
					const char	*Ext,
					char		*FullName)

#else						/* defined (__STDC__)					*/

void	PathName(Path, Name, Ext, FullName)
const char	*Path;
const char	*Name;
const char	*Ext;
char		*FullName;

#endif						/* defined (__STDC__)					*/

{
	char	*ExtAnt;

	/*
	 * Si Path apunta a una posicion de memoria distinta de cero,
	 * construimos los nombres completos de los ficheros encadenando el
	 * path y el nombre.
	 */
	if (Path != (char *) 0) {
		(void) sprintf(FullName, "%s/%s", Path, Name);
	}
	else {
		(void) sprintf(FullName, "%s", Name);
	}

	/*
	 * Si se ha indicado extension y el nombre no la incluia, anhadimos
	 * la indicada.
	 */
	if (Ext != (char *) 0) {
		ExtAnt = strrchr(FullName, '.');
		if (ExtAnt == (char *) 0 || strcmp(ExtAnt + 1, Ext) != 0) {
			(void) strcat(FullName, ".");
			(void) strcat(FullName, Ext);
		}
	}
	
	return;
}

/***********************************************************************
 * ChkPathName - Comprueba la validez de un path y, si no existe,
 * lo crea.
 **********************************************************************/

#if defined(__STDC__) && !defined(NO_PROTOTYPE)

int		ChkPathName(const char	*FullPath)

#else						/* defined (__STDC__)					*/

int		ChkPathName(FullPath)
char	*FullPath;

#endif						/* defined (__STDC__)					*/

{
	char	Path[_POSIX_PATH_MAX+1], *Dir, *Name;

	/*
	 * Si FullPath == (char *) 0, error.
	 */
	if (FullPath == (char *) 0) {
		return -1;
	}

	(void) strcpy(Path, FullPath);

	/* 
	 * Si Path no contiene ninguna barra (/), Path=. y es correcto.
	 */
	if ((Name = strrchr(Path, '/')) == (char *) 0) {
		return 0;
	}
	else {
		Name[1] = '\0';
	}

	Dir = Path;
	/*
	 * Eliminamos los posibles '/' que haya al principio.
	 */
	while (Dir[0] == '/') Dir++;
	while ((Dir = strchr(Dir, '/')) != (char *) 0) {
		Dir[0] = '\0';
		if (access(Path, F_OK) != 0) {
			if (mkdir(Path, 0755) != 0) {
				return -3;
			}
			errno = 0;
		}
		Dir[0] = '/';
		Dir++;
	}
	
	return 0;
}

/***********************************************************************
 * QuitaExt - Elimina una extension si la encuentra en el nombre
 **********************************************************************/

int		QuitaExt(
	char	*FicSen,
	char	*ExtSen)
{
	int		LongFic, LongExt;
	
	if (ExtSen == (char *) 0) return 0;
	if (FicSen == (char *) 0) return -1;

	LongFic = strlen(FicSen);
	LongExt = strlen(ExtSen);

	if (LongExt >= LongFic) return -1;

	if (strcmp(FicSen + LongFic - LongExt, ExtSen) == 0) {
		if (ExtSen[0] != '.' && FicSen[LongFic - LongExt - 1] == '.') LongExt++;
		FicSen[LongFic - LongExt] = '\0';
		return 0;
	}

	return -1;
}

/***********************************************************************
 * QuitaDir - Elimina el directorio si lo encuentra en el nombre
 **********************************************************************/

char	*QuitaDir(
	char	*FicSen,
	char	*DirSen)
{
	int		LongFic, LongDir;
	
	if (DirSen == (char *) 0) return FicSen;
	if (FicSen == (char *) 0) return (char *) 0;

	LongFic = strlen(FicSen);
	LongDir = strlen(DirSen);

	if (LongDir >= LongFic) return FicSen;

	if (strncmp(FicSen, DirSen, LongDir) == 0) {
		FicSen += LongDir;
		
		if (*FicSen == '/') FicSen++;
		return FicSen;
	}

	return FicSen;
}

/***********************************************************************
 * CreaLisSen - Crea una lista de senhales a partir de un fichero guia
 * o de un vector de nombres de fichero rematado en (char *) 0.
 **********************************************************************/

char	**CreaLisSen(
 	char	*LisFic,
	char	**NomFic,
	char	*DirSen,
	char	*ExtSen,
	int		*NumFic)
{
	FILE	*FpGui;
	char	**FicGui;
	char	**LisSen, *NomSen;
	char	FicSen[_POSIX_PATH_MAX];
	int		Gui, NumGui;

	/*
	 * Debe especificarse el nombre del fichero guia o el vector de
	 * nombres
	 */
	if (LisFic == (char *) 0 && NomFic == (char **) 0) {
		if (!DirSen || (!strcmp(DirSen, "-") && !strcmp(DirSen, "stdin") && !strcmp(DirSen, "stdio"))) {
			fprintf(stderr, "Error al crear la lista de senhales:\n");
			fprintf(stderr, "Debe especificarse el fichero guia o el nombre de los ficheros\n");
			return (char **) 0;
		}
	}

	if (LisFic != (char *) 0 && NomFic != (char **) 0) {
		fprintf(stderr, "Error al crear la lista de senhales:\n");
		fprintf(stderr, "No se puede especificar un fichero guia y el nombre de los ficheros al mismo tiempo\n");
		return (char **) 0;
	}

	if ((LisSen = (char **) malloc(sizeof(char *))) == (char **) 0) {
		fprintf(stderr, "Error al crear la lista de senhales (%s)\n", strerror(errno));
		return (char **) 0;
	}

	/*
	 * Si se ha indicado fichero guia, lo leemos.
	 */
	if (LisFic != (char *) 0) {
		if ((FicGui = MatStrChr(LisFic, "+ ,")) == (char **) 0) {
			fprintf(stderr, "ERROR al procesar la lista de ficheros guia %s\n", LisFic);
			return (char **) 0;
		}

		for (NumGui = 0; FicGui[NumGui] != (char *) 0; NumGui++) ;

		/*
		 * Abrimos los fichero guia
		 */
		*NumFic = 0;
		for (Gui = 0; Gui < NumGui; Gui++) {
			if ((FpGui = fopen(FicGui[Gui], "rt")) == (FILE *) 0) {
				fprintf(stderr, "Error al leer la lista de ficheros %s (%s)\n", FicGui[Gui], strerror(errno));
				return (char **) 0;
			}

			/*
			 * Cargamos cada uno de los nombres en la matriz
			 */
			while (fscanf(FpGui, "%s", FicSen) != EOF) {
				(*NumFic)++;

				if ((LisSen = (char **) realloc((void *) LisSen, *NumFic * sizeof(char *))) == (char **) 0) {
					fprintf(stderr, "Error al crear la lista de senhales (%s)\n", strerror(errno));
					return (char **) 0;
				}

				(void) QuitaExt(FicSen, ExtSen);
			
				LisSen[*NumFic - 1] = strdup(FicSen);
			}
			fclose(FpGui);
		}
		LiberaLisSen(FicGui, NumGui);
		return LisSen;
	}
	else if (NomFic != (char **) 0) {
		char* tmp;
		for (*NumFic = 0; NomFic[*NumFic] != (char *) 0; (*NumFic)++) {
			if ((LisSen = (char **) realloc((void *) LisSen, (*NumFic + 1) * sizeof(char *))) == (char **) 0) {
				fprintf(stderr, "Error al crear la lista de senhales (%s)\n", strerror(errno));
				return (char **) 0;
			}

			tmp = NomSen = strdup(NomFic[*NumFic]);

			(void) QuitaExt(NomSen, ExtSen);

			if (DirSen != (char *) 0) {
				if ((NomSen = QuitaDir(NomSen, DirSen)) == (char *) 0) {
					fprintf(stderr, "Error al eliminar %s de %s\n", DirSen, NomFic[*NumFic]);
					return (char **) 0;
				}
			}

			LisSen[*NumFic] = strdup(NomSen);
			free(tmp);
		}
		return LisSen;
	}
	else if (DirSen && (!strcmp(DirSen, "-") || !strcmp(DirSen, "stdin") || !strcmp(DirSen, "stdio"))) {
		return LeeLisSen(NumFic);
	}

	return (char **) 0;
}

/***********************************************************************
 * LiberaMatStr - Libera una lista de strings acabados en 0 
 **********************************************************************/

void	LiberaMatStr(
	char	**matStr)
{
	int i;
	for (i=0; matStr[i]!=0; i++)
		free(matStr[i]);
	free(matStr);
}

/***********************************************************************
 * LiberaLisSen - Libera una lista de senhales 
 **********************************************************************/

void	LiberaLisSen(
	char	**NomFic,
	int		NumFic)
{
	int i;
	for (i=0; i<NumFic; i++)
		free(NomFic[i]);
	free(NomFic);
}




/***********************************************************************
 * LeeLisSen - Lee una lista de senhales de la entrada estandar
 **********************************************************************/

char	**LeeLisSen(int	*NumFic)
{
	char	**LisSen;
	char	FicSen[_POSIX_PATH_MAX];
	int		Fic;

	if (scanf("%d", NumFic) < 1) return (char **) 0;
	if ((LisSen = (char **) malloc(*NumFic * sizeof(char *))) == (char **) 0) {
		fprintf(stderr, "Error al crear la lista de senhales (%s)\n", strerror(errno));
		return (char **) 0;
	}
	for (Fic = 0; Fic < *NumFic; Fic++) {
		if (scanf("%s\n", FicSen) != 1) {
			fprintf(stderr, "Error al leer la lista de senhales (%s)\n", strerror(errno));
			return NULL;
                }
		LisSen[Fic] = strdup(FicSen);
	}

	return LisSen;
}

/***********************************************************************
 * EscrLisSen - Escribe en la selida estandar la lista de senhales
 **********************************************************************/

int		EscrLisSen(
	char	**LisSen,
	int		NumFic)
{
	int		Fic;

	printf("%d\n", NumFic);
	for (Fic = 0; Fic < NumFic; Fic++) {
		printf("%s\n", LisSen[Fic]);
	}

	return 0;
}

/***********************************************************************
 * TamMat - Calcula el número de elementos de una matriz a partir de una
 * cadena separada en "tokens" por comas y/o espacios.
 **********************************************************************/

#if defined(__STDC__) && !defined(NO_PROTOTYPE)

int		TamMat(char	*_Str)

#else						/* defined (__STDC__)					*/

int		TamMat(_Str)
char	*_Str;

#endif						/* defined (__STDC__)					*/

{
	char	*Str1, *Str = strdup(_Str);
	size_t	i;

	if ((Str1 = Str) == (char *) 0) {
		return -1;
	}

	for (i = 0; strtok(Str, " ,") != (char *) 0; i++) {
		Str = (char *) 0;	/* Para la siguiente llamada a strtok	*/
	}

	free((void *) Str1);

	return i;
}

/***********************************************************************
 * MatStr - Construye una matriz de cadenas a partir de una cadena 
 * separada en "tokens" por comas y/o espacios.
 **********************************************************************/

#if defined(__STDC__) && !defined(NO_PROTOTYPE)

char	**MatStr(char	*Str)

#else						/* defined (__STDC__)					*/

char	**MatStr(Str)
char	*Str;

#endif						/* defined (__STDC__)					*/

{
	return MatStrChr(Str, " ,");
}

/***********************************************************************
 * MatStr - Construye una matriz de cadenas a partir de una cadena 
 * separada en "tokens" por comas y/o espacios.
 **********************************************************************/

char	**MatStrChr(
	char	*_Str,
	char	*Delim)

{
	char	**Mat, *Str = strdup(_Str);
	char	*fStr = Str;
	size_t	i;

	if (Str == (char *) 0) {
		return (char **) 0;
	}

	if((Mat = (char **) malloc(sizeof(char *))) == (char **) 0) {
		free(fStr);
		return (char **) 0;
	}

	for (i = 0; (Mat[i] = strtok(Str, Delim)) != (char *) 0; i++) {
		Mat[i] = strdup(Mat[i]);
		Str = (char *) 0;	/* Para la siguiente llamada a strtok	*/
		if((Mat = (char **) realloc((void *) Mat, (i + 2) * sizeof(char *))) == (char **) 0) {
			free(fStr);
			return (char **) 0;
		}
	}

	Mat[i] = (char *) 0;
	free(fStr);

	return Mat;
}

/***********************************************************************
 * MatInt - Construye una matriz de enteros a partir de una cadena 
 * separada en "tokens" por comas y/o espacios.
 **********************************************************************/

#if defined(__STDC__) && !defined(NO_PROTOTYPE)

int	*MatInt(char	*_Str)

#else						/* defined (__STDC__)					*/

int	*MatInt(_Str)
char	*_Str;

#endif						/* defined (__STDC__)					*/

{
	char	*Str = strdup(_Str);
	char	*fStr = Str;
	char	*Cadena;
	int		*Mat;
	size_t	i;
	
	if (Str == (char *) 0) {
		return (int *) 0;
	}

	if((Mat = (int *) malloc(sizeof(int))) == (int *) 0) {
		free(fStr);
		return (int *) 0;
	}

	for (i = 0; (Cadena = strtok(Str, " ,")) != (char *) 0; i++) {
		Str = (char *) 0;	/* Para la siguiente llamada a strtok	*/
		Mat[i] = atoi(Cadena);
		if((Mat = (int *) realloc((void *) Mat, (i + 2) * sizeof(int))) == (int *) 0) {
			free(fStr);
			return (int *) 0;
		}
	}

	Mat[i] = -1;
	free(fStr);

	return Mat;
}

/***********************************************************************
 * MatLng - Construye una matriz de enteros long a partir de una cadena 
 * separada en "tokens" por comas y/o espacios.
 **********************************************************************/

#if defined(__STDC__) && !defined(NO_PROTOTYPE)

long	*MatLng(char	*_Str)

#else						/* defined (__STDC__)					*/

long	*MatLng(_Str)
char	*_Str;

#endif						/* defined (__STDC__)					*/

{
	char	*tmp = strdup(_Str);
	char	*Str = tmp;
	char	*Cadena;
	long		*Mat;
	size_t	i;

	if (Str == (char *) 0) {
		return (long *) 0;
	}

	if((Mat = (long *) malloc(sizeof(long))) == (long *) 0) {
		return (long *) 0;
	}

	for (i = 0; (Cadena = strtok(Str, " ,")) != (char *) 0; i++) {
		Str = (char *) 0;	/* Para la siguiente llamada a strtok	*/
		Mat[i] = atol(Cadena);
		if((Mat = (long *) realloc((void *) Mat, (i + 2) * sizeof(long))) == (long *) 0) {
			return (long *) 0;
		}
	}

	Mat[i] = -1;
	free(tmp);
	return Mat;
}

/***********************************************************************
 * MatDbl - Construye una matriz de reales doble precision a partir de
 * una cadena separada en "tokens" por comas y/o espacios.
 **********************************************************************/

#if defined(__STDC__) && !defined(NO_PROTOTYPE)

double	*MatDbl(char	*Str)

#else						/* defined (__STDC__)					*/

double	*MatDbl(Str)
char	*Str;

#endif						/* defined (__STDC__)					*/

{
	char	*Cadena;
	double	*Mat;
	size_t	i;

	if (Str == (char *) 0) {
		return (double *) 0;
	}

	if((Mat = (double *) malloc(sizeof(double))) == (double *) 0) {
		return (double *) 0;
	}

	for (i = 0; (Cadena = strtok(Str, " ,")) != (char *) 0; i++) {
		Str = (char *) 0;	/* Para la siguiente llamada a strtok	*/
		Mat[i] = atof(Cadena);
		if((Mat = (double *) realloc((void *) Mat, (i + 2) * sizeof(double))) == (double *) 0) {
			return (double *) 0;
		}
	}

	Mat[i] = -1;

	return Mat;
}

/***********************************************************************
 * EsExcUdf - Comprueba si una unidad fonetica es excluida.
 **********************************************************************/

int		EsExcUdf(	char	*_Unidad,	/* Unidad a comprobar	*/
					char	**ExcUdf)	/* Unidades excluidas	*/
{
	int		Exc;
	char	*Marca;
	char	Unidad[1024];

	strcpy(Unidad, _Unidad);

	while((Marca = strpbrk(Unidad, "_·:@'")) != (char *) 0) {
		memmove(Marca, Marca + 1, strlen(Marca));
	}

	if (ExcUdf == (char **) 0) return -1;

	for (Exc = 0; ExcUdf[Exc] != (char *) 0; Exc++) {
		if (strcmp(Unidad, ExcUdf[Exc]) == 0) return Exc;
	}

	return -1;
}

/***********************************************************************
 * EsIdenUdf - Comprueba si dos unidades foneticas estan identificadas.
 **********************************************************************/

int		EsIdenUdf(	char	*_Uni1,		/* Unidad a comprobar	*/
					char	*_Uni2,		/* Unidad a comprobar	*/
					char	***IdenUdf)	/* Unidades identif.	*/
{
	int		Iden, Udf;
	int		EsIden1, EsIden2;
	char	*Marca;
	char	Uni1[1024], Uni2[1024];

	strcpy(Uni1, _Uni1);
	strcpy(Uni2, _Uni2);

	while((Marca = strpbrk(Uni1, "_·:@'")) != (char *) 0) {
		memmove(Marca, Marca + 1, strlen(Marca));
	}
	while((Marca = strpbrk(Uni2, "_·:@'")) != (char *) 0) {
		memmove(Marca, Marca + 1, strlen(Marca));
	}

	if (!strcmp(Uni1, Uni2)) return 1;

	if (IdenUdf == (char ***) 0) return -1;

	for (Iden = 0; IdenUdf[Iden] != (char **) 0; Iden++) {
		EsIden1 = EsIden2 = 0;
		for (Udf = 0; IdenUdf[Iden][Udf] != (char *) 0; Udf++) {
			if (strcmp(Uni1, IdenUdf[Iden][Udf]) == 0) break;
		}
		if (IdenUdf[Iden][Udf] != (char *) 0) EsIden1 = 1;

		for (Udf = 0; IdenUdf[Iden][Udf] != (char *) 0; Udf++) {
			if (strcmp(Uni2, IdenUdf[Iden][Udf]) == 0) break;
		}
		if (IdenUdf[Iden][Udf] != (char *) 0) EsIden2 = 1;

		if (EsIden1 && EsIden2) return Iden;
	}

	return -1;
}

/***********************************************************************
 * NovaIden - Construye la matriz de identificacion fonetica
 **********************************************************************/

int		NovaIden(	char	****IdenUdf,
					int		*NumIden,
					char	*Iden)
{
	if (*IdenUdf == (char ***) 0) {
		*NumIden = 1;
		if ((*IdenUdf = (char ***) malloc(2*sizeof(char **))) == (char ***) 0) {
			(void) fprintf(stderr, "Error al ubicar la matriz de identificaciones\n");
			return -1;
		}
	}
	else {
		*NumIden += 1;
		if ((*IdenUdf = (char ***) realloc((void *) *IdenUdf, (*NumIden+1)*sizeof(char **))) == (char ***) 0) {
			(void) fprintf(stderr, "Error al reubicar la matriz de identificaciones\n");
			return -1;
		}
	}

	(*IdenUdf)[*NumIden-1] = MatStr(Iden);
	(*IdenUdf)[*NumIden] = (char **) 0;

	if ((*IdenUdf)[*NumIden-1][0] == (char *) 0 ||
		(*IdenUdf)[*NumIden-1][1] == (char *) 0) {
		(void) fprintf(stderr, "Cada identificacion fonetica debe indicarse como:\n");
		(void) fprintf(stderr, "\t-a Udf1,Udf2[,...]\n");
		return -1;
	}

	return 0;
}
