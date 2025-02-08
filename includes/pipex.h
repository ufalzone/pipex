/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:56:36 by ufalzone          #+#    #+#             */
/*   Updated: 2025/02/08 21:01:40 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
#define PIPEX_H

#include "../libft/includes/libft.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void	ft_perror(char *str);
int	pipex(char **av, char **env);
void	fils_1(char **av, char **env, int *fd, int *fd_pipe);
void	fils_2(char **av, char **env, int *fd, int *fd_pipe);
void	init_fd(char **av, int fd[2], int fd_pipe[2]);
int		pid1_exec(char **av, char **env, int *fd, int *fd_pipe);
int		pid2_exec(char **av, char **env, int *fd, int *fd_pipe);

#endif
