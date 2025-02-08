/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 20:50:37 by ufalzone          #+#    #+#             */
/*   Updated: 2025/02/08 21:02:47 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"


int	pipex(char **av, char **env)
{
	int		status;
	int		fd[2];
	int		fd_pipe[2];
	pid_t	pid1;
	pid_t	pid2;

	init_fd(av, fd, fd_pipe);
	pid1 = fork();
	if (pid1 < 0)
		ft_perror("pid1");
	else if (pid1 == 0)
		fils_1(av, env, fd, fd_pipe);
	else
	{
		close(fd_pipe[1]);
		pid2 = fork();
		if (pid2 < 0)
			ft_perror("pid2");
		else if (pid2 == 0)
			fils_2(av, env, fd, fd_pipe);
		else
		{
			close(fd_pipe[0]);
			waitpid(pid1, NULL, 0);
			waitpid(pid2, &status, 0);
			return (WEXITSTATUS(status));
		}
	}
}
