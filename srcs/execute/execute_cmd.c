#include "minishell.h"

extern int	g_exit_code;

void	execute_command(t_info *info, int depth)
{
	int	status;
	int	ret;

	info->cmd_sequence = depth;
	info->pipex.pid[depth] = fork();
	if (info->pipex.pid[depth] == -1)
		error();
	if (info->pipex.pid[depth] > 0)
	{
		if (depth == info->n_cmd - 1)//마지막 명령어이면 if문 진입
		{
			close_pipeline(info);//부모에서 파이프라인 다 닫음(자식엔 영향 없음)
			while (wait(&status) != -1)//모든 자식 프로세스가 끝날 때 까지 대기
			{
				;
			}
			g_exit_code = WEXITSTATUS(status);
			return ;//status값을 exitcode전역변수에 넣어주기
		}
		execute_command(info, depth + 1);//부모에서 execute_command() 함수 재귀 호출
	}
	if (info->pipex.pid[depth] == 0)
	{
		ret = execute_execve(info, depth);
		exit(ret);
	}
}

void	execute_command_main(t_info *info)
{
	make_pipeline(info);//파이프라인 생성
	info->pipex.pid = (int *)malloc(sizeof(int) * info->n_cmd);
	info->cmd_sequence = 0;
	get_cmd_list(info);
	if (is_builtin_command(info) && (info->n_cmd == 1))
	{
		g_exit_code = execute_execve(info, 0);
		return ;
	}
	execute_command(info, 0);
}
