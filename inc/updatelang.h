#ifndef UPDATE_LANGUAGE_H
#define UPDATE_LANGUAGE_H

typedef enum
{
	LANG_C,
	LANG_CPP,
	LANG_SHELL,
	LANG_PYTHON,
	LANG_PERL,
	LANG_JAVA,
	LANG_HTML,
	LANG_CSS,
	LANG_MAKE,
	LANG_CMAKE,
	LANG_LAST
}lang_enum_t;

typedef  struct
{
	unsigned long long size;
	float percent;
	const char* lang_name;
	char zero;
	const char* color;
}lang_stat_t;


typedef struct 
{
	unsigned long long total_size;
	lang_stat_t lang_list[LANG_LAST];
}project_stat_t;


extern void update_file_type(const char* folder, const char* file_name);

#endif /*ifndef UPDATE_LANGUAGE_H */
