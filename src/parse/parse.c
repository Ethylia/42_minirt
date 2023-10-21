#include <ctype.h>

#include "parse.h"
#include "util/util.h"

int	parsedouble(const char **str, float *v)
{
	*v = 0.0f;
	*str += countws((const char*)*str);
	long sign = 1;
	if(**str == '-')
	{
		sign = -1;
		(*str)++;
	}
	if(!isdigit(**str) && **str != '.')
		return 3;
	while (isdigit(**str))
		*v = *v * 10.0 + (*((*str)++) - '0');
	if(**str == '.')
	{
		if(!isdigit(*(++(*str))))
			return 3;
		while (isdigit(**str))
		{
			*v = *v * 10.0 + (**str - '0');
			sign *= 10;
			++(*str);
		}
	}
	*v /= sign;
	return (**str != ',' && **str != ' ' &&
		**str != '\t' && **str != '\n' && **str != '\0') * 3;
}

int	parsevec(const char **str, float *v)
{
	int	r;
	if((r = parsedouble(str, v + 0)) ||
		(*(*str)++) != ',' ||
		(r = parsedouble(str, v + 1)) ||
		(*(*str)++) != ',' ||
		(r = parsedouble(str, v + 2)))
		return 3;

	return (**str != ' ' && **str != '\t' && **str != '\n' && **str != '\0') * 3;
}

int	parsecolor(const char **str, float *v)
{
	if(parsevec(str, v))
		return 3;
	if(v[0] < 0.0f || v[0] > 255.0f ||
		v[1] < 0.0f || v[1] > 255.0f ||
		v[2] < 0.0f || v[2] > 255.0f)
		return 2;
	v[0] /= 255.0f;
	v[1] /= 255.0f;
	v[2] /= 255.0f;
	return 0;
}

int	parseuint(const char **str, uint *v)
{
	*str += countws(*str);
	*v = 0;
	if(!isdigit(**str))
		return 3;
	while(isdigit(**str))
		*v = *v * 10 + (*((*str)++) - '0');
	return (**str != ',' && **str != ' ' && **str != '\t' && **str != '\n' && **str != '\0') * 3;
}
