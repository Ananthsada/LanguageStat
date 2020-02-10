#include <string.h> /* for memory related functions */
#include <sys/types.h> /* for stat() function */
#include <sys/stat.h>  /* for stat() function */
#include <stdlib.h> /* for malloc() and free() functions */
#include <stdio.h> /* for input and output buffer handling */
#include <errno.h> /* to check error status set by gnu library calls */

#include "updatelang.h" /* Internal header to declare functions defined in this file */
#include "user_types.h"	/* Header to declare common user defined types */
#include "presenter.h" /* Header to call presenter functions */

#define EXTN_FOUND	0
#define EXTN_NOT_FOUND 1

typedef  struct
{
	unsigned int key;
	const char* lang_string;
}lang_key_hashtable_t;

/*
 * Making as global variable 
 * as variable will reside in data segment not in stack
 * */
static unsigned char file_path[550];
static lang_key_hashtable_t lang_extn_hashtable[] = 
{
	{ 0, "c" },
	{ 1, "cpp" },
	{ 2, "py" },
	{ 3, "pl" },
	{ 4, "java" },
	{ 5, "html" },
	{ 6, "sh" },
	{ 7, "css" }
};

static lang_key_hashtable_t lang_name_hashtable[] = 
{
	{ 0, "Makefile" },
	{ 1, "CMakeLists.txt" },
};

static unsigned int extn_hastable_size = sizeof(lang_extn_hashtable) / sizeof(lang_key_hashtable_t);
static unsigned int name_hastable_size = sizeof(lang_name_hashtable) / sizeof(lang_key_hashtable_t);

project_stat_t project_stat = {
		0,
		{ { 0, 0.0, "C", '\0', Yellow },
		{ 0, 0.0, "C++", '\0', Blue },
		{ 0, 0.0, "Shell" , '\0', Green },
		{ 0, 0.0, "Python", '\0', Magenta },
		{ 0, 0.0, "Perl", '\0',  Red },
		{ 0, 0.0, "Java", '\0', Cyan },
		{ 0, 0.0, "HTML", '\0', LightYellow },
		{ 0, 0.0, "CSS", '\0', LightBlue },
		{ 0, 0.0, "Makefile", '\0', LightRed },
		{ 0, 0.0, "CMakeLists.txt", '\0', LightGreen }, }
};

static unsigned int getlanguage_extn_key(const char* extension);
static unsigned int getlanguage_name_key(const char* extension);
static unsigned int update_by_extension( const char* file_extension, unsigned int extn_length, unsigned int file_size);
static void update_by_name(const char* file_rel_name, unsigned int name_length, unsigned int file_size);
static void calculate_percent(void);

unsigned int getlanguage_extn_key(const char* extension)
{
	unsigned int index = extn_hastable_size;
		
	for( index = 0; index <= ( extn_hastable_size - 1 ); index++ )
	{
		if( strcmp( extension, lang_extn_hashtable[index].lang_string ) == 0 )
		{
			return index;
		}
	}
	return index;
}

unsigned int getlanguage_name_key(const char* file_rel_name)
{
	unsigned int index = name_hastable_size;
		
	for( index = 0; index <= ( name_hastable_size - 1 ); index++ )
	{
		if( strcmp( file_rel_name, lang_name_hashtable[index].lang_string ) == 0 )
		{
			return index;
		}
	}
	return index;
}

static unsigned int update_by_extension( const char* file_extension, unsigned int extn_length, unsigned int file_size)
{
	char* extension;
	unsigned int retval = EXTN_FOUND;
	
	extension = (char *)malloc( ( extn_length + 1 ) * sizeof(char) );
	(void)memset(&extension[0], 0, extn_length + 1 );
	(void)memcpy(&extension[0], file_extension, extn_length);
		
	switch( getlanguage_extn_key( extension ) )
	{
		case 0:
			project_stat.lang_list[LANG_C].size = project_stat.lang_list[LANG_C].size + (unsigned long long)file_size;
			project_stat.total_size = project_stat.total_size + (unsigned long long)file_size;
			break;
			
		case 1:
			project_stat.lang_list[LANG_CPP].size = project_stat.lang_list[LANG_CPP].size + (unsigned long long)file_size;
			project_stat.total_size = project_stat.total_size + (unsigned long long)file_size;
			break;
		
		case 2:
			project_stat.lang_list[LANG_PYTHON].size = project_stat.lang_list[LANG_PYTHON].size + (unsigned long long)file_size;
			project_stat.total_size = project_stat.total_size + (unsigned long long)file_size;
			break;
						
		case 3:
			project_stat.lang_list[LANG_PERL].size = project_stat.lang_list[LANG_PERL].size + (unsigned long long)file_size;
			project_stat.total_size = project_stat.total_size + (unsigned long long)file_size;
			break;
						
		case 4:
			project_stat.lang_list[LANG_JAVA].size = project_stat.lang_list[LANG_JAVA].size + (unsigned long long)file_size;
			project_stat.total_size = project_stat.total_size + (unsigned long long)file_size;
			break;
												
		case 5:
			project_stat.lang_list[LANG_HTML].size = project_stat.lang_list[LANG_HTML].size + (unsigned long long)file_size;
			project_stat.total_size = project_stat.total_size + (unsigned long long)file_size;
			break;
						
		case 6:
			project_stat.lang_list[LANG_SHELL].size = project_stat.lang_list[LANG_SHELL].size + (unsigned long long)file_size;
			project_stat.total_size = project_stat.total_size + (unsigned long long)file_size;
			break;
									
		case 7:
			project_stat.lang_list[LANG_CSS].size = project_stat.lang_list[LANG_CSS].size + (unsigned long long)file_size;
			project_stat.total_size = project_stat.total_size + (unsigned long long)file_size;
			break;
						
		default:
			retval = EXTN_NOT_FOUND;
			break;
	}
	free(extension);
	
	return retval;
}

static void update_by_name(const char* file_rel_name, unsigned int name_length, unsigned int file_size)
{
	char* file_name_l;
	
	file_name_l = (char *)malloc( ( name_length + 1 ) * sizeof(char) );
	(void)memset(&file_name_l[0], 0, ( name_length + 1 ) );
	(void)memcpy(&file_name_l[0], file_rel_name, ( name_length + 1 ) );
	
	switch( getlanguage_name_key( file_name_l ) )
	{
		case 0:
			project_stat.lang_list[LANG_MAKE].size = project_stat.lang_list[LANG_MAKE].size + (unsigned long long)file_size;
			project_stat.total_size = project_stat.total_size + (unsigned long long)file_size;
			break;
			
		case 1:
			project_stat.lang_list[LANG_CMAKE].size = project_stat.lang_list[LANG_CMAKE].size + (unsigned long long)file_size;
			project_stat.total_size = project_stat.total_size + (unsigned long long)file_size;
			break;
						
		default:
			break;
	}
	free(file_name_l);		
}

static void calculate_percent()
{
	lang_stat_t* parse_ptr = &project_stat.lang_list[LANG_C];
	lang_stat_t* end_ptr = &project_stat.lang_list[LANG_CMAKE];
	
	while( parse_ptr <= end_ptr )
	{
		parse_ptr->percent = (double)parse_ptr->size / (double)project_stat.total_size;
		parse_ptr->percent = parse_ptr->percent * 100;
		parse_ptr++;
	}
}

void update_file_type(const char* folder, const char* file_name)
{
	unsigned int file_name_length = strlen(file_name);
	unsigned int folder_name_length = strlen(folder);
	int index;
	struct stat file_stat;
	
	(void)memset( &file_path[0], 0, sizeof(file_path) );
	
	for( index = ( file_name_length - 1 ) ; index >= 0 ; index-- )
	{
		if( file_name[index] == '.' )
		{
			/**
			 * extension found 
			 * */
			 (void)memcpy(&file_path[0], folder, folder_name_length );
			 file_path[folder_name_length] = '/';
			 (void)memcpy(&file_path[folder_name_length + 1], file_name, file_name_length );
			 if( 0 == stat(file_path, &file_stat) )
			 {
				if( EXTN_NOT_FOUND == update_by_extension( &file_name[index + 1], file_name_length - index - 1 , file_stat.st_size ) )
				{
					/**
					 *  check for full name 
					 * */
					index = -1;
				}
				else
				{
					/**
					 * extension found and updated 
					 * */
				}
			 }
			 else
			 {
				printf("\nfile %s is not valid : error num %s", file_path, strerror(errno) );
			 }			 
			 break;
		}
		else
		{
			/**
			 * Continue to search for extension 
			 * */
		}
	}
	
	if( index == -1 )
	{
		(void)memcpy(&file_path[0], folder, folder_name_length );
		 file_path[folder_name_length] = '/';
		 (void)memcpy(&file_path[folder_name_length + 1], file_name, file_name_length );
		 
		 if( ( 0 == stat(file_path, &file_stat) ) && 
			 ( S_IFREG == ( file_stat.st_mode & 0100000) ) )
		 {
			update_by_name( file_name, file_name_length - 1 , file_stat.st_size );
		 }
		 else
		 {
			printf("\nfile %s is not valid : error num %s %d ", file_path, strerror(errno), file_stat.st_mode );
		 }		
	}
	
}

void finish_activity()
{
	calculate_percent();
	present_output();
}
