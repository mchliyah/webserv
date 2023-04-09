/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 01:13:29 by slahrach          #+#    #+#             */
/*   Updated: 2023/04/09 01:13:35 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


void client::execute()
{
	std::string script = std::string("/Users/slahrach/Desktop/after/script.cgi");
	std::string script_name = std::string("SCRIPT_NAME=") + script;
	std::string q = std::string ("QUERY_STRING=") + query;
	std::string cookie = std::string ("HTTP_COOKIE=") + getValue("Cookie");
	std::string method = std::string ("REQUEST_METHOD=") + getValue("Method");
	std::string script_filename = std::string ("SCRIPT_FILENAME=") + std::string("/Users/slahrach/Desktop/after/hello.php");
	std::string protocol = std::string ("SERVER_PROTOCOL=") + getValue("version");
	std::string software = std::string ("SERVER_SOFTWARE=WEBSERV");
	std::string redirect = std::string ("REDIRECT_STATUS=true");
	std::string uri = std::string("REQUEST_URI=") + getValue("URL");
	std::string host = std::string("HTTP_HOST=") + getValue("Host");
	std::string port = std::string("SERVER_PORT=") + getPort();
	std::string gateway = std::string("GATEWAY_INTERFACE=CGI/1.1");
	char *const env[] = {(char *)script_name.c_str(), (char *)q.c_str(), (char *)cookie.c_str(), (char *)method.c_str(), (char *)script_filename.c_str(),
		(char *)protocol.c_str(), (char *)software.c_str(), (char *)redirect.c_str(), (char *)uri.c_str(), (char *)host.c_str(), (char *)port.c_str(),(char *)gateway.c_str(), NULL};
	char *const args[] = {(char *)script.c_str(), nullptr};
	int fd = open("output.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
	int pid = fork();
	if (pid == 0)
	{
		close(socket_fd);
		dup2(fd, 1);
		execve(args[0], args, env);
		std::cerr << "execve failed" << std::endl;
		exit(1);
	}
	else
	{
		waitpid(pid, NULL, 0);
		dup2(STDIN_FILENO, 0);
		dup2(STDOUT_FILENO, 1);
		close(fd);
	}

}