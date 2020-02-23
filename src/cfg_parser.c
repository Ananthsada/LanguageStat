#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PRE_DEF_LENGTH_STRING \
		"#define LANG_CNT "

#define PRE_DEF_PROJ_STRUCT_STR \
		"project_stat_t project_stat[] = { \n\
        0,\n{"
	
#define POST_DEF_PROJ_STRUCT_STR \
		"} };"
		
#define PRE_DEF_HASH_STRUCT_STR \
		"static lang_key_hashtable_t lang_extn_hashtable[] = \n\
{\n"
        
#define POST_DEF_HASH_STRUCT_STR \
		"};"
		
#define XML_CFG_FILE "/home/ananth/MyProject/LanguageStats/cfg/lang_list.xml"
#define OUT_FILE "./cfg_list.c"

static int out_fd = -1;
static char project_stat_buf[1000];
static char lang_extn_hash_buf[1000];
static int proj_stat_index = 0;
static int lang_hash_index = 0;

static void cfg_init()
{
	mode_t mode_l = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	int flags = O_WRONLY | O_CREAT | O_APPEND | O_TRUNC;
	
	out_fd = open( OUT_FILE, flags, mode_l );
	if( out_fd <= -1 )
	{
		printf("File open failed %s\n",strerror(errno));
		exit(-1);
	}
	
	printf("File created successfully\n");
	
	memset( project_stat_buf, 0 ,sizeof(project_stat_buf));
	memset( lang_extn_hash_buf, 0 ,sizeof(lang_extn_hash_buf));
	memcpy( project_stat_buf, PRE_DEF_PROJ_STRUCT_STR, strlen(PRE_DEF_PROJ_STRUCT_STR));
	memcpy( lang_extn_hash_buf, PRE_DEF_HASH_STRUCT_STR, strlen(PRE_DEF_HASH_STRUCT_STR));
	proj_stat_index = strlen(PRE_DEF_PROJ_STRUCT_STR);
	lang_hash_index = strlen(PRE_DEF_HASH_STRUCT_STR);
}

static xmlNode* GetChildren(const xmlNode* node )
{
	xmlNode* ret_node = node->children;
	
	while( ret_node )
	{
		if( ret_node->type != XML_ELEMENT_NODE ) 
		{
			ret_node = ret_node->next;
		}
		else
		{
			printf("Found element node %s\n", ret_node->name );
			break;
		}
	}	
	return ret_node;
}

static void update_buffer(const char* Key, const char* extensions, const char* full_name, const char* color)
{
	char local_buf[50];
	int local_index = 0;
	
	if( Key != NULL )
	{
		memcpy(&lang_extn_hash_buf[lang_hash_index], "        { 0, ", 13);
		lang_hash_index = lang_hash_index + 13;
		memcpy(&lang_extn_hash_buf[lang_hash_index], Key, strlen(Key) );
		lang_hash_index = lang_hash_index + strlen(Key);
		memcpy(&lang_extn_hash_buf[lang_hash_index], "}\n", 2 );
		lang_hash_index = lang_hash_index + 2;
		

		memcpy(&project_stat_buf[proj_stat_index], "        { 0, 0.0, \"", 19);
		proj_stat_index = proj_stat_index + 19;
		memcpy(&project_stat_buf[proj_stat_index], extensions, strlen(extensions) );
		proj_stat_index = proj_stat_index + strlen(extensions);
		memcpy(&project_stat_buf[proj_stat_index], "\", '\0', \"", 10 );
		proj_stat_index = proj_stat_index + 10;
		memcpy(&project_stat_buf[proj_stat_index], color , strlen(color) );
		proj_stat_index = proj_stat_index + strlen(color);
		memcpy(&project_stat_buf[proj_stat_index], "}\n", 2);		
		proj_stat_index = proj_stat_index + 2;
	}
}

static void buffer_enclose()
{
	memcpy( &project_stat_buf[proj_stat_index], "} };", 3 );
	memcpy( &lang_extn_hash_buf[lang_hash_index], "};", 2 );
}

static void parse_xml(xmlNode* root_node)
{
	xmlNode *curr_node = NULL;
	xmlChar* string_l;
	char LangExtn[30];
	char LangFullName[30];
	char LangColor[10] = "\x1b[97m";
	char LangKey[20];
	
	if( root_node != NULL )
	{
		printf("Root Element %s\n", root_node->name);
		curr_node = GetChildren(root_node);
		if( curr_node != NULL )
		{
			curr_node = GetChildren(curr_node);
			if( !curr_node )
			{
				printf("Something is wrong\n");
			}
		}
		for( ; curr_node ; curr_node = curr_node->next )
		{
			if( !strcmp( (const char*)curr_node->name, "Language" ) )
			{
				memset(&LangExtn[0], 0 ,sizeof(LangExtn));
				memset(&LangKey[0], 0 ,sizeof(LangKey));
				memset(&LangColor[0], 0 ,sizeof(LangColor));
				memset(&LangFullName[0], 0 ,sizeof(LangFullName));
				
				string_l = xmlGetProp( curr_node, "name");
				if( string_l )
				{
					printf("\nLanguage %s", string_l);
					strncpy( &LangKey[0], (const char*)string_l, sizeof(LangKey)); 
					xmlFree(string_l);
				}
				else
				{
					continue;
				}
				xmlNode* inner_node = NULL;
				inner_node = GetChildren(curr_node);
				while( inner_node )
				{
					string_l = xmlGetProp(inner_node, "key");
					if( string_l )
					{
						printf("Element %s : key %s, ",inner_node->name, string_l);
						if( !strcmp( (const char *)inner_node->name, "color" ) )
						{
							strncpy( &LangColor[0], (const char*)string_l, 10);
						}
						else if( !strcmp( (const char *)inner_node->name, "extensions" ) )
						{
							strncpy( &LangExtn[0], (const char*)string_l, 10);
						}	
						else if( !strcmp( (const char *)inner_node->name, "fullname" ) )
						{
							strncpy( &LangFullName[0], (const char*)string_l, 10);
						}					
						xmlFree(string_l);
						update_buffer( &LangKey[0], &LangExtn[0], &LangFullName[0], &LangColor[0] );
					}
					inner_node = inner_node->next;					
				}
				
			}
		}
	}
	else
	{
		printf("Root Element NULL");
	}
}

static void cfg_exit()
{
	if( out_fd >= 0 )
	{
		close(out_fd);
	}
}

static void cfg_write()
{
	int retval = lang_hash_index;
	int index = 0;
	
	if( out_fd >= 0 )
	{
/*		while( retval > 0 )
		{
			retval = write( out_fd, &lang_extn_hash_buf[index], retval);
			if( retval > 0 )
			{
				index = index + retval;
				retval = lang_hash_index - index;
			}
			else
			{
				printf("Stopping write operation %s", strerror(errno));
			}
		}
*/		
		retval = proj_stat_index;
		while( retval > 0 )
		{
			retval = write( out_fd, &project_stat_buf[index], retval);
			if( retval > 0 )
			{
				index = index + retval;
				retval = proj_stat_index - index;
			}
			else
			{
				printf("Stopping write operation %s", strerror(errno));
			}
		}
	}
}

int main()
{
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	
	LIBXML_TEST_VERSION 
 
	cfg_init();
	
	if( ( doc = xmlReadFile( XML_CFG_FILE, NULL, 0)) == NULL )
	{
		printf("error: could not parse file %s\n", XML_CFG_FILE);
		exit(-1);
	}

	root_element = xmlDocGetRootElement(doc);
	parse_xml(root_element);
	buffer_enclose();
	cfg_write();
	cfg_exit();
	
	xmlFreeDoc(doc);
	xmlCleanupParser();	
	
	return 0;
}
