#include <dirent.h> /* to read directory content */
#include <unistd.h>	/* to get current working directory */
#include <string.h> /* for memory related functions */
#include <errno.h> /* to check error status set by gnu library calls */
#include <stdio.h> /* for input and output buffer handling */
#include <stdlib.h> /* for exit() function */
       
#include "logger.h" /* Internal library to print logs */
#include "parse_dir.h" /* Internal header to declare functions defined in this file */
#include "updatelang.h" /* internal header to handle programming language graph implementations */

#define CURRENT_DIR_SHORT 	"."
#define PARENT_DIR_SHORT	".."

/*
 * store absolute pathname of directory to get stats
 * */
static char native_directory[150];

void init_dir_core()
{
	memset( &native_directory[0], 0, sizeof(native_directory));
	get_current_directory();
}

void get_current_directory()
{
	if( NULL == getcwd( &native_directory[0], sizeof(native_directory)) )
	{
		printf("\n%d %s failed, Error status %s", __LINE__, __FUNCTION__, strerror(errno));
		fflush(stdout);
		exit(0);
	}
	else
	{
		printf("\n%d %s : Current Working directory : %s", __LINE__, __FUNCTION__, native_directory);
	}
}

void get_folder_content(const char* folder_name) 
{
	DIR *directory;
	struct dirent* dir_content;
	char current_directory[500];
	
	(void)memset( &current_directory[0], 0, sizeof(current_directory) );
	(void)memcpy( &current_directory[0], folder_name, strlen(folder_name) );
	
	directory = opendir(folder_name);
	if(directory != NULL)
	{	
		
		while( NULL != ( dir_content = readdir(directory) ) )
		{
			if( DT_DIR == dir_content->d_type )
			{
				if( ( 0 != strcmp( PARENT_DIR_SHORT, dir_content->d_name )  ) && 
				( 0 != strcmp( CURRENT_DIR_SHORT, dir_content->d_name )  ) )
				{
					
					current_directory[strlen(folder_name)] = '/';
					(void)memcpy( &current_directory[strlen(folder_name) + 1], dir_content->d_name, strlen( dir_content->d_name) );
					
					get_folder_content( current_directory );
					(void)memset( &current_directory[strlen(folder_name) + 1], 0, strlen( dir_content->d_name));
				}
				else
				{
					/** 
					 * Shortcut . or ..  read 
					 * */
				}
			}
			else if( DT_REG == dir_content->d_type )
			{
				update_file_type( current_directory, dir_content->d_name);
			}
		}
	}
	else
	{
		/**
		 * Folder is empty 
		 * */
	}
}

void start_activity()
{
	get_folder_content(native_directory);
}
