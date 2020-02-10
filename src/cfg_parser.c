#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PRE_DEF_LENGTH_STIRNG \
		"#define LANG_CNT "

#define PRE_DEF_STRUCT_STIRNG \
		"project_stat_t project_stat[] = {"
		
#define XML_CFG_FILE "/home/ananth/MyProject/LanguageStats/cfg/lang_list.xml"
#define OUT_FILE "./cfg_list.c"

static int out_fd = -1;

void cfg_init()
{
	out_fd = open( OUT_FILE, O_WRONLY | O_CREAT )
	if( out_fd == -1 )
	{
		exit(0);
	}
}

static void print_element_names(xmlNode * a_node)
{
	xmlNode *cur_node = NULL;
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) 
	{
		if (cur_node->type == XML_ELEMENT_NODE) 
		{
			printf("node type: Element, name: %s\n", cur_node->name);
		}
		print_element_names(cur_node->children);
	}
}

static void parse_xml(xmlNode* root_node)
{
	xmlNode *curr_node = NULL;
	
	if( root_node != NULL )
	{
		curr_node = root_node->child;
		curr_node = curr_node ? curr_node->child : NULL;
		while( curr_node )
		{
			if( curr_node->name == "Language" )
			{
				xmlNode* innernode = NULL;
				innernode = curr_node->child;
				while( inner_node )
				{
					
				}
				curr_node = curr_node->next;
			}
			else
			{
				curr_node = NULL;
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
	/*print_element_names(root_element);
	xmlFreeDoc(doc);
	xmlCleanupParser();*/	
	return 0;
}
