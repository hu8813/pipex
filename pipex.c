/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: huaydin <huaydin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/17 13:14:29 by huaydin           #+#    #+#             */
/*   Updated: 2022/11/17 23:18:49 by huaydin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**create_command(char cmd[])
{
	char	**join_command;

	if (!*cmd || cmd[0] == 0)
	{
		write(2, "Command not found: \n", 20);
		exit(EXIT_FAILURE);
	}
	join_command = malloc(sizeof * join_command * 4);
	join_command[0] = "/bin/bash";
	join_command[1] = "-c";
	join_command[2] = cmd;
	join_command[3] = NULL;
	return (join_command);
}

void	execute_first(int *fd, char **command1, char **envp, char **argv)
{
	pid_t	pid_1;
	int		file1;

	file1 = open(argv[1], O_RDONLY);
	if (file1 == -1 || !file1 || read(file1, 0, 0) < 0)
		perror("Error: ");
	pid_1 = fork();
	if (pid_1 < 0)
	{
		perror("Error: ");
		exit(errno);
	}
	if (pid_1 == 0)
	{
		dup2(file1, 0);
		dup2(fd[1], 1);
		close(fd[0]);
		if (execve("/bin/bash", command1, envp) == -1)
			perror("Error: ");
	}
	close(fd[1]);
	waitpid(pid_1, NULL, WNOHANG | WUNTRACED);
	free(command1);
}

void	execute_last(int *fd, char **command2, char **envp, char **argv)
{
	pid_t	pid_2;
	int		file2;

	file2 = open(argv[4], O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (file2 == -1 || !file2 || write(file2, 0, 0) < 0)
	{
		perror("Error: ");
		exit(errno);
	}
	pid_2 = fork();
	if (pid_2 < 0)
	{
		perror("Error: ");
		exit(errno);
	}
	if (pid_2 == 0)
	{
		dup2(file2, 1);
		dup2(fd[0], 0);
		if (execve("/bin/bash", command2, envp) == -1)
			perror("Error: ");
	}
	close(fd[0]);
	waitpid(pid_2, NULL, WUNTRACED);
	free(command2);
}

void	check_files(char **argv)
{
	if (access(argv[1], F_OK | R_OK) != -1 && access(argv[4], F_OK) != -1)
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
	check_files(argv);
	if (pipe(fd) == -1)
	{
		perror("Error: ");
		exit(EXIT_FAILURE);
	}
	execute_first(fd, create_command(argv[2]), envp, argv);
	execute_last(fd, create_command(argv[3]), envp, argv);
	exit(0);
}
