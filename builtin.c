#include "shell.h"

/**
 * _myexit - Exit the shell with a given exit status.
 * @info: Structure containing potential arguments.
 *
 * If info.argv[0] is not "exit", return 0.
 * If there is an exit argument, convert it to an integer and exit with that value.
 * If the exit argument is not a valid integer, set info->status to 2 and return 1.
 *
 * Return: The exit status to be used for the shell termination.
 */
int _myexit(info_t *info)
{
    int exitcheck;

    if (info->argv[1]) /* If there is an exit argument */
    {
        exitcheck = _erratoi(info->argv[1]);
        if (exitcheck == -1)
        {
            info->status = 2;
            print_error(info, "Illegal number: ");
            _eputs(info->argv[1]);
            _eputchar('\n');
            return 1;
        }
        info->err_num = _erratoi(info->argv[1]);
        return -2;
    }
    info->err_num = -1;
    return -2;
}

/**
 * _mycd - Change the current directory of the process.
 * @info: Structure containing potential arguments.
 *
 * If no argument is given, change to the home directory (HOME) or root if HOME is not set.
 * If the argument is "-", change to the previous working directory (OLDPWD).
 * If a valid directory path is given as an argument, change to that directory.
 * If the change is successful, update the PWD and OLDPWD environment variables.
 *
 * Return: Always 0.
 */
int _mycd(info_t *info)
{
    char *s, *dir, buffer[1024];
    int chdir_ret;

    s = getcwd(buffer, 1024);
    if (!s)
        _puts("TODO: >>getcwd failure emsg here<<\n");

    if (!info->argv[1])
    {
        dir = _getenv(info, "HOME=");
        if (!dir)
            chdir_ret = /* TODO: what should this be? */
            chdir((dir = _getenv(info, "PWD=")) ? dir : "/");
        else
            chdir_ret = chdir(dir);
    }
    else if (_strcmp(info->argv[1], "-") == 0)
    {
        if (!_getenv(info, "OLDPWD="))
        {
            _puts(s);
            _putchar('\n');
            return 1;
        }
        _puts(_getenv(info, "OLDPWD="));
        _putchar('\n');
        chdir_ret = /* TODO: what should this be? */
        chdir((dir = _getenv(info, "OLDPWD=")) ? dir : "/");
    }
    else
        chdir_ret = chdir(info->argv[1]);

    if (chdir_ret == -1)
    {
        print_error(info, "can't cd to ");
        _eputs(info->argv[1]);
        _eputchar('\n');
    }
    else
    {
        _setenv(info, "OLDPWD", _getenv(info, "PWD="));
        _setenv(info, "PWD", getcwd(buffer, 1024));
    }
    return 0;
}

/**
 * _myhelp - Display help message (function not implemented).
 * @info: Structure containing potential arguments.
 *
 * Return: Always 0.
 */
int _myhelp(info_t *info)
{
    char **arg_array;
    arg_array = info->argv;
    _puts("help call works. Function not yet implemented \n");
    if (0)
        _puts(*arg_array); /* temp att_unused workaround */
    return 0;
}
