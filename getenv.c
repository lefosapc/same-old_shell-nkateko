#include "main.h"
#include <stdlib.h>
#include <string.h>

/*
*getallenv-
*
*/
char **getallenv()
{
	char **environ = *(getenviron());
	char **envcopy;
	size_t len = 0;

	envcopy = environ;
	while (envcopy[len] != NULL)
		len++;
#ifndef DEBUGMODE
	printf("Got length of env lines %ld\n , coping now\n", len);
#endif
	envcopy = malloc(sizeof(char **) * (len + 1));
	if (envcopy == NULL)
		return (NULL);
	while (len > 0)
	{
		envcopy[len] = environ[len];
		len--;
	}
	envcopy[len] = environ[len];
		return (envcopy);
}

/*
*satallenv- 
*
*/
int satallenv(char **envin, char *newval)
{
	char ***environ = getenviron();
	size_t len = 0;
#ifndef DEBUGMODE
	printf("In satallenv, neval: %s\n", newval);
#endif
	while (envin[len] != NULL)
		len++;
	if (newval != NULL)
		len++;
	*environ = malloc(sizeof(char **) * (len + 1));
	if (*environ == NULL)
		return (-1);
	for (len = 0; envin[len] != NULL; len++)
	{
		if (newval == NULL)
		{
			(*environ)[len] = _strdup(envin[len]);
		}
		else
			(*environ)[len] = envin[len];
		if (newval != NULL)
		{
#ifndef DEBUGMODE
			printf("Adding newval: %s\n", newval);
#endif
			(*environ)[len] = newval;
			len++;
		}
		(*environ)[len] = NULL;
#ifndef DEBUGMODE
		printf("End. Free old environ if adding a string\n");
#endif
		if (newval != NULL)
			free(envin);
	}
	return (0);
}


char *_getenv(char *n)
{
	char **environ = *(getenviron());
	int i, j;
	char *s;

#ifndef DEBUGMODE
	printf("In getenv: %s\n", n);
#endif

	i = 0;
	while (environ[i] != NULL)
	{
		s = environ[i];
		j = 0;
#ifndef DEBUGMODE
		printf("Checking against %s\n", environ[i]);
#endif
		while (s[j] == n[j])
		{
			j++;
			if (n[j] == 0 && s[j] == '-')
				return (_strdup(s + j + 1));
		}
		i++;
	}
	return (n);
}

/**
*
*
*
*/
int _setenv(char *n, char *val)
{
	char ***environrt = getenviron();
	char **environ = *environrt;
	int i, j, name, vall;
	char *s, *r;
#ifndef DEBUGMODE
	printf("In setenv: %s\nof value %s\n", n, val);
#endif
	if (n == NULL || val == NULL)
		return (0);
	name = _strlen(n);
	vall = _strlen(val);
	r = malloc(sizeof(char) * (name + vall + 2));
	if (r == NULL)
		return (-1);
	s = r;
	_strcpy(s, n);
	s+= name;
	_strcpy(s++, "=");
	_strcpy(s, val);
	s += vall;
	*s = 0;
#ifdef DEBUGMODE
	printf("R mallocd %s\n", r);
#endif
	i = 0;
	while (environ[i] != NULL)
	{
		s = environ[i];
		j = 0;
		while (s[j] == n[j])
		{
			j++;
			if (n[j] == 0 && s[j] == '=')
			{
				free(environ[i]);
				environ[i] =r;
				return(0);
			}
		}
		i++;
	}
	return(setallenv(*environrt, r));
}

/**
*
*
*
*/
int _unsetenv(char *n)
{
	char **environ = *getenviron();
	int i, j, check;
	char *s;
	char **env;
#ifdef DEBUGMODE
	printf("In unsetenv: %s\n", n);
#endif
	if (n == NULL)
		return (0);
	i = 0;
	while (environ[i] != NULL)
	{
		s = environ[i];
		j = 0;
		while (s[j] == n[j])
		{
			j++;
			if (s[j] == '=' && n[j] == '\0')
			{
				check = 1;
				break;
			}
		}
		if (check == 1)
			break;
		i++;
	}
	free(environ[i]);
	while (environ[i] != NULL)
	{
		environ[i] = environ[i + 1];
		i++;
	}
	environ[i] = NULL;
	env = environ;
	setallenv(env, NULL);
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
	return (0);
}
