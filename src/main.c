/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ufalzone <ufalzone@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:56:16 by ufalzone          #+#    #+#             */
/*   Updated: 2025/02/08 21:02:51 by ufalzone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"
#include <stdio.h>
#include <unistd.h>

/**************************************
 *             PIPEX GUIDE            *
 **************************************/

/* 1️⃣ Ouvrir les fichiers */
/*
   - Ouvre le fichier d’entrée (infile) en lecture.
   - Ouvre (ou crée) le fichier de sortie (outfile) en écriture.
*/

/* 2️⃣ Créer un pipe */
/*
   - Le pipe sert de communication entre cmd1 et cmd2.
   - Il possède deux extrémités :
		- Une **extrémité d'écriture** où cmd1 va écrire.
		- Une **extrémité de lecture** où cmd2 va lire.
*/

/* 3️⃣ Premier fork (cmd1) */
/*
   - **Rediriger l'entrée** : STDIN devient infile (pour lire depuis infile).
   - **Rediriger la sortie** : STDOUT devient l'extrémité d’écriture du pipe.
   - **Fermer les descripteurs inutiles** (infile et pipe non utilisé).
   - **Exécuter cmd1** (avec execve).
*/

/* 4️⃣ Deuxième fork (cmd2) */
/*

	- **Rediriger l'entrée** : STDIN devient l'extrémité de lecture du pipe (pour lire ce que cmd1 a écrit).

	- **Rediriger la sortie** : STDOUT devient outfile (pour écrire le résultat final).
   - **Fermer les descripteurs inutiles**.
   - **Exécuter cmd2** (avec execve).
*/

/* 5️⃣ Attendre la fin des processus */
/*
   - Fermeture des pipes et des fichiers restants.
   - Utilisation de waitpid() pour éviter les processus zombies.
*/


char	*find_command_path(char *cmd, char **env)
{
	char	**paths;
	char	*path_line;
	char	*full_path;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (NULL);
	path_line = env[i] + 5;           // pour skip le PATH=
	paths = ft_split(path_line, ':'); // separe tous les chemins de PATH
	i = 0;
	while (paths[i])
	// on va tester tous les paths pour voir si la commande existe
	{
		full_path = ft_strjoin_three(paths[i], "/", cmd);
		// rajoute a la fin de path un "/" et la commande appropriee
		if (access(full_path, X_OK) == 0)
			return (free_split(paths), full_path);
		free(full_path);
		i++;
	}
	return (free_split(paths), NULL);
}

void	fils_1(char **av, char **env, int *fd, int *fd_pipe)
{
	int		execve_ret;
	char	**cmd_args;
	char	*cmd_path;

	close(fd_pipe[0]);
	close(fd[1]);
	close(0);
	dup2(fd[0], 0);
	close(1);
	dup2(fd_pipe[1], 1);
	cmd_args = ft_split(av[2], ' ');
	if (!cmd_args)
		ft_perror("split1");
	cmd_path = find_command_path(cmd_args[0], env);
	if (!cmd_path)
	{
		free_split(cmd_args);
		exit(127);
	}
	execve_ret = execve(cmd_path, cmd_args, env);
	if (execve_ret == -1)
	{
		free_split(cmd_args);
		ft_perror("execve");
	}
}
void	fils_2(char **av, char **env, int *fd, int *fd_pipe)
{
	int		execve_ret;
	char	**cmd_args;
	char	*cmd_path;

	close(fd_pipe[1]);
	close(fd[0]);
	close(0);
	dup2(fd_pipe[0], 0);
	close(1);
	dup2(fd[1], 1);
	cmd_args = ft_split(av[3], ' ');
	if (!cmd_args)
		ft_perror("split1");
	cmd_path = find_command_path(cmd_args[0], env);
	if (!cmd_path)
	{
		free_split(cmd_args);
		exit(127);
	}
	execve_ret = execve(cmd_path, cmd_args, env);
	if (execve_ret == -1)
	{
		free_split(cmd_args);
		ft_perror("execve");
	}
}

void	init_fd(char **av, int fd[2], int fd_pipe[2])
{
	int	pipe_result;

	fd[0] = open(av[1], O_RDONLY);
	fd[1] = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	pipe_result = pipe(fd_pipe);
	if (pipe_result == 1)
	{
		close(fd[0]);
		close(fd[1]);
		ft_perror("pipe");
	}
}


// exceve si il fonctionne n'execute pas la suite
int	main(int ac, char **av, char **env)
{
	if (ac < 5)
		ft_perror("Nombre d'arguments");
	if (!av[1][0] || !av[2][0] || !av[3][0] || !av[4][0])
		ft_perror("Arguments vides non autorises.");
	return (pipex(av, env));
}
