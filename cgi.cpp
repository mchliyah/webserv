/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 07:57:12 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/30 08:28:32 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void execute()
{
	const char*  script_name =  std::string("SCRIPT_NAME=" + "./php-cgi");
	const char*  query =  std::string("QUERY_STRING=" + c.query);
	const char*  cookie =  std::string("HTTP_COOKIE=" + c.getValue("Cookie"));
	const char*  method =  std::string("REQUEST_METHOD=" + c.getValue("Method"));
	const char*  script_filename =  std::string("SCRIPT_FILENAME=" + "./hello.php");
	const char*  protocol =  std::string("SERVER_PROTOCOL=" + c.getValue("Version"));
	char *env[] = {script_name, query, method, script_filename, protocol, cookie, "SERVER_SOFTWARE=WEBSERV", "REDIRECT_STATUS=true", NULL};
	int fd = open("output.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	int pid = fork();
	if (pid == 0)
	{
		dup2(fd, 1);
		dup2(fd, 2);
		execve(script_name, NULL, env);
	}
	else
	{
		waitpid(pid, NULL, 0);
		close(fd);
	}

}