#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define MIN 100
#define MB 1024
#define GB 1024*MB

/* functions */

/* file extension */
const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) 
    	return "";
    return dot + 1;
}

/* mueve el archivo segun su tamaño */
void move_to_folder(char *oldir, char * newdir, char *size, char * name){
	strcat(newdir,size);
	mkdir(newdir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	strcat(newdir,"/");strcat(newdir,name);
	rename(oldir, newdir);

}

/* los ordena segun tamaño para crear el directorio */
void order_by_size(char * file, char * namef, char * folder){
	struct stat st;
	stat(file, &st);
	strcat(folder,"/");

	if ((st.st_size/1024) < 100)
		move_to_folder(file,folder,"<100kb",namef);
	else if ((st.st_size/1024) < MB)
		move_to_folder(file,folder,"100kb-1mb",namef);
	else if ((st.st_size/1024) < 100*MB)
		move_to_folder(file,folder,"1mb-100mb",namef);
	else if ((st.st_size/1024) < GB)
		move_to_folder(file,folder,"100mb-1gb",namef);
	else
		move_to_folder(file,folder,">1gb",namef);
}


int main (int argc, char const *argv[])
{
	DIR *dp;
	struct dirent *ep;
	char aux_route[1024], old_route[1024], new_route[1024], folder_file[1024];

	if(argc < 2){
		printf("No ingreso carpeta.\n");
		return 0;
	}

	dp = opendir (argv[1]);

	/* ruta ingresada */
	strcpy(new_route,argv[1]);
	strcpy(old_route, argv[1]);
	strcpy(aux_route,argv[1]);

	if (dp != NULL){
		while (ep = readdir (dp)){
			if (ep->d_type == DT_REG){ /* archivos que no sean carpetas */
				if (strcmp(get_filename_ext(ep->d_name),"")==0){ 
					/* archivos sin parametro */
					strcat(new_route, "other");
				}
				else {
					strcat(new_route, get_filename_ext(ep->d_name));
				}	
				
				mkdir(new_route, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
				
				/* direccion actual de archivo */
				strcat(old_route, ep->d_name);

				/* nueva direccion archivo */
				strcat(new_route, "/"); 
				strcpy(folder_file,new_route);
				strcat(new_route, ep->d_name);

				/* cambio de directorio */
				rename(old_route, new_route);
				order_by_size(new_route,ep->d_name,folder_file);


				strcpy(old_route, aux_route);
				strcpy(new_route, aux_route);
			}
		}	
		(void) closedir (dp);

	}
	else{
		printf("No se pudo abrrir la carpeta\n");
	}

	return 0;
}

