#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PRE_DEF_LENGTH_STIRNG \
		"#define LANG_CNT "

#define PRE_DEF_STRUCT_STIRNG \
		"project_stat_t project_stat[] = { \
				0,"
		
#define XML_CFG_FILE "/home/ananth/MyProject/LanguageStats/cfg/lang_list.xml"
#define OUT_FILE "./cfg_list.c"

static int out_fd = -1;

void cfg_init()
{
	mode_t mode_l = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	int flags = O_WRONLY | O_CREAT | O_APPEND | O_TRUNC;
	out_fd = open( OUT_FILE, flags, mode_l );
	if( out_fd == -1 )
	{
		printf("File open failed %s\n",strerror(errno));
		exit(-1);
	}
	else
	{
		printf("File created successfully\n");
	}
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

static void parse_xml(xmlNode* root_node)
{
	xmlNode *curr_node = NULL;
	xmlChar* string_l;
	char LangExtn[30] = "Others";
	char LangColor[10] = "\x1b[97m";
	
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
		while( curr_node )
		{
			if( !strcmp( (const char*)curr_node->name, "Language" ) )
			{
				string_l = xmlGetProp( curr_node, "name");
				if( string_l )
				{
					printf("\nLanguage %s", string_l);
					xmlFree(string_l);
				}
				xmlNode* inner_node = NULL;
				inner_node = GetChildren(curr_node);
				while( inner_node )
				{
					string_l = xmlGetProp(inner_node, "key");
					if( string_l )
					{
						printf("Element %s : key %s, ",inner_node->name, string_l);
						xmlFree(string_l);
					}
					inner_node = inner_node->next;
				}
				
			}
			curr_node = curr_node->next;
		}
	}
	else
	{
		printf("Root Element NULL");
	}
}

void cfg_exit()
{
	if( out_fd >= 0 )
	{
		close(out_fd);
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
	cfg_exit();
	
	xmlFreeDoc(doc);
	xmlCleanupParser();	
	
	return 0;
}
