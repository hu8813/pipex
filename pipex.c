/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:14:29 by huaydin           #+#    #+#             */
/*   Updated: 2022/11/20 06:38:54 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	ft_error(void)
{
	perror("Error: ");
	exit(errno);
}

void	execute_first(int *fd, char **envp, char **argv)
{
	pid_t	pid_1;
	int		file1;
	char	*com1[4];

	com1[0] = "/bin/bash";
	com1[1] = "-c";
	com1[2] = argv[2];
	com1[3] = NULL;
	file1 = open(argv[1], O_RDONLY);
	if (file1 == -1 || !file1 || read(file1, 0, 0) < 0)
		perror("Error: ");
	pid_1 = fork();
	if (pid_1 < 0)
		ft_error();
	if (pid_1 == 0)
	{
		dup2(file1, 0);
		dup2(fd[1], 1);
		close(fd[0]);
		if (execve("/bin/bash", com1, envp) == -1)
			perror("Error: ");
	}
	close(fd[1]);
	waitpid(pid_1, NULL, WNOHANG | WUNTRACED);
}

void	execute_last(int *fd, char **envp, char **argv)
{
	pid_t	pid_2;
	int		file2;
	char	*com2[4];

	com2[0] = "/bin/bash";
	com2[1] = "-c";
	com2[2] = argv[3];
	com2[3] = NULL;
	file2 = open(argv[4], O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (file2 == -1 || !file2 || write(file2, 0, 0) < 0)
		ft_error();
	pid_2 = fork();
	if (pid_2 < 0)
		ft_error();
	if (pid_2 == 0)
	{
		dup2(file2, 1);
		dup2(fd[0], 0);
		if (execve("/bin/bash", com2, envp) == -1)
			perror("Error: ");
	}
	close(fd[0]);
	waitpid(pid_2, NULL, WUNTRACED);
}

void	chec_args(char **argv)
{
	if (!*argv[2] || !*argv[3])
	{
		write(2, "Error, Command empty\n", 22);
		exit(EXIT_FAILURE);
	}
	if (access(argv[1], F_OK | R_OK) != -1)
		return ;
	perror("Error: ");
}

int	main(int argc, char **argv, char **envp)
{
	int	fd[2];

	if (argc != 5 || argv[1][0] == 0 || argv[4][0] == 0)
	{
		write(2, "Bad usage\nTry './pipex file1 cmd1 cmd2 file2'\n", 47);
		exit(EXIT_FAILURE);
	}
	chec_args(argv);
	if (pipe(fd) == -1)
	{
		perror("Error: ");
		exit(EXIT_FAILURE);
	}
	execute_first(fd, envp, argv);
	execute_last(fd, envp, argv);
	exit(0);
}
