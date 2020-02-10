#include <stdio.h> /* for input and output buffer handling */

#include "updatelang.h"
#include "presenter.h" /* Internal header to declare functions defined in this file */
#include "user_types.h"	/* Header to declare common user defined types */

extern project_stat_t project_stat;

void present_output()
{	
	lang_stat_t* parse_ptr = &project_stat.lang_list[LANG_C];
	lang_stat_t* end_ptr = &project_stat.lang_list[LANG_CMAKE];
	unsigned int ret_val = 0;
	unsigned int index = 0;
	float others = 0.0f;
	
	printf("\033[H\033[J");
	while( parse_ptr <= end_ptr )
	{
		if( parse_ptr->percent >= 2.0f )
		{
			printf("%s",parse_ptr->color);
			ret_val = printf("%s",parse_ptr->lang_name);
			for( index = 0; index < ( 20 - ret_val ) ; index++ )
			{
				printf(" ");
			}
			for( index = 0; index < ( (unsigned long long)parse_ptr->percent / 2 ) ; index++ )
			{
				printf("*");
			}
			ret_val = printf("(%.2f%%)",parse_ptr->percent);
			printf("\n");
		}
		else
		{
			others = others + parse_ptr->percent;
		}
		
		parse_ptr++;
	}	
	if( others > 0.0f )
	{
		printf("%s",LightGray);
		ret_val = printf("Others");
		for( index = 0; index < ( 20 - ret_val ) ; index++ )
		{
			printf(" ");
		}
		for( index = 0; index < ( (unsigned long long)others / 2 ) ; index++ )
		{
			printf("*");
		}
		printf("(%.2f%%)",others);
		printf("\n");
	}
}

