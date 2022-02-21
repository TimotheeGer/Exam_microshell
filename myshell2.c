#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void	ft_error(char *str)
{
	int i = 0;
	while(str[i])
		i++;
	write (2, str, i);
}

int	ft_execv(char **av, int i, int fd_tmp, char **env)
{
	av[i] = NULL;
	dup2(fd_tmp, STDIN_FILENO);
	close(fd_tmp);
	execve(av[0], av, env);
	ft_error("error: ");
	ft_error(av[0]);
	ft_error("\n");
	return (1);
}

int	main(int ac, char **av, char **env)
{
	int i = 0;
	int pid = 0;
	int fd[2];
	int fd_tmp;

	fd_tmp = dup(STDIN_FILENO);
	while (av[i] && av[i + 1])
	{
		av = &av[i + 1];
		i = 0;
		while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
			i++;
		if (strcmp(av[0], "cd") == 0)
		{
			if (i != 2)
				ft_error("error: \n");
			if (chdir(av[1]) != 0)
			{
				ft_error("error: ");
				ft_error(av[1]);
				ft_error("\n");
			}
		}
		else if (av != &av[i] && (av[i] == NULL || strcmp(av[i], ";") == 0))
		{
			pid = fork();
			if (pid == 0)
				ft_execv(av, i, fd_tmp, env);
			close(fd_tmp);
			waitpid(-1, NULL, 0);
			fd_tmp = dup(STDIN_FILENO);
		}
		else if (av != &av[i] && (strcmp(av[i], "|") == 0))
		{
			pipe(fd);
			pid = fork();
			if (pid == 0)
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[0]);
				close(fd[1]);
				ft_execv(av, i, fd_tmp, env);
			}
			close(fd[1]);
			close(fd_tmp);
			waitpid(-1, NULL, 0);
			fd_tmp = dup(fd[0]);
			close(fd[0]);
		}
	}
	close(fd_tmp);
	return (0);
}