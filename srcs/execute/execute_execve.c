#include "minishell.h"

/*
** 파일이 존재하지 않거나 디렉토리 파일이면 에러메세지 출력 후 종료
*/
void	is_directory(char *cmd)
{
	struct stat	file_stat;
	int			file_type;

	if (stat(cmd, &file_stat) == ERROR)
	{
		error_message(cmd, NULL, strerror(errno));
		exit(CODE_NO_FILE_OR_DIR);
	}
	file_type = file_stat.st_mode & S_IFMT;
	if (file_type == S_IFDIR)
	{
		error_message(cmd, NULL, MSG_IS_A_DIR);
		exit(CODE_IS_A_DIR);
	}
}

/*
** 커맨드를 경로로 입력했을 때 커맨드를 그대로 리턴
** 아니면 while() 돌면서 PATH 환경변수를 참조해서 PATH 경로에 명령어 있으면 리턴
*/
char	*get_cmd_path(char **env_path, t_info *info)
{
	int			idx;
	char		*cmd;
	char		*path_of_cmd;
	struct stat	file_stat;

	idx = 0;
	path_of_cmd = NULL;
	cmd = info->cmd_str[0];
	if (cmd == NULL)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		is_directory(cmd);
		return (cmd);
	}
	while (env_path && env_path[idx])
	{
		path_of_cmd = ft_strjoin(env_path[idx], cmd);
		if (stat(path_of_cmd, &file_stat) == 0)
			return (path_of_cmd);
		free(path_of_cmd);
		path_of_cmd = NULL;
		idx++;
	}
	return (path_of_cmd);
}

int	is_builtin_command(t_info *info)
{
	char	*cmd;
	int		cmd_len;

	if (info->cmd_lst[info->cmd_sequence].text == NULL)
		return (FALSE);
	cmd = info->cmd_lst[info->cmd_sequence].text->str;
	cmd_len = ft_strlen(cmd);
	if (cmd_len <= 0)
		return (FALSE);
	if (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "echo")
		|| is_register_variable(cmd))
		return (TRUE);
	return (FALSE);
}

/*
** 1. get_cmd_list() 함수로 info->cmd_str에 명령어, 옵션, 아규먼트 저장
** 2. get_cmd_path() 함수로 명령어가 위치한 절대경로 받아오기
** 3. switch_stdio() 함수로 표준입출력 변경
** 4. 빌트인 명령어이면 빌트인 함수 실행
** 5. 일반 명령어이면 execve() 함수로 명령어 실행
*/
int	execute_execve(t_info *info, int depth)
{
	int		fd[2];
	char	*cmd_path;

	get_cmd_list(info);
	cmd_path = get_cmd_path(info->env_path, info);
	if (switch_stdio(info, fd))
		return (TRUE);
	if (is_builtin_command(info))
		builtin(info->cmd_str, info, fd);
	else if (info->cmd_str[0])
	{
		kill(0, SIGUSR1);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execve(cmd_path, info->cmd_str, info->env_str);
		error_message(info->cmd_str[0], NULL, MSG_CMD_NOT_FOUND);
		exit(CODE_CMD_NOT_FOUND);
	}
	free(cmd_path);
	return (NORMAL);
}
