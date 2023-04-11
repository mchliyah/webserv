/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi_response.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 03:23:26 by mchliyah          #+#    #+#             */
/*   Updated: 2023/04/11 14:08:54 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include <sys/wait.h>
#include <signal.h>

void client::cgi_response(response &res, std::string& file_path, bool php)
{
	std::string script;
	if (php)
		script = "scripts/php-cgi";
	else
		script = ("scripts/python");
	std::string script_name = std::string("SCRIPT_NAME=") + script;
	std::string q = std::string ("QUERY_STRING=") + query;
	std::string cookie = std::string ("HTTP_COOKIE=") + getValue("Cookie");
	std::string method = std::string ("REQUEST_METHOD=") + getValue("Method");
	std::string script_filename = std::string ("SCRIPT_FILENAME=") + file_path;
	std::string protocol = std::string ("SERVER_PROTOCOL=") + getValue("version");
	std::string software = std::string ("SERVER_SOFTWARE=WEBSERV");
	std::string redirect = std::string ("REDIRECT_STATUS=true");
	std::string uri = std::string("REQUEST_URI=") + getValue("URL");
	std::string host = std::string("HTTP_HOST=") + getValue("Host");
	std::string port = std::string("SERVER_PORT=") + getPort();
	std::string gateway = std::string("GATEWAY_INTERFACE=CGI/1.1");
	char *const env[] = {(char *)script_name.c_str(), (char *)q.c_str(), (char *)cookie.c_str(), (char *)method.c_str(), (char *)script_filename.c_str(),
		(char *)protocol.c_str(), (char *)software.c_str(), (char *)redirect.c_str(), (char *)uri.c_str(), (char *)host.c_str(), (char *)port.c_str(),(char *)gateway.c_str(), NULL};
	char *const args[] = {(char *)script.c_str(), (char *)file_path.c_str(), NULL};
	if (first_time)
	{
		first_time = false;
		int fd = open("output.html", O_RDWR | O_CREAT | O_TRUNC, 0666);
		int pid = fork();
		if (pid < 0)
		{
			res.set_status_code("500");
			errorResponse(res);
			return;
		}
		if (pid == 0)
		{
			close(socket_fd);
			dup2(fd, 1);
			execve(args[0], args, env);
			exit(500);
		}
		else
		{
			int count = 0;
			int status;
			while (1)
			{
				int result = waitpid(pid, &status, WNOHANG);
				if (result != 0 && WIFEXITED(status))
					break;
				if (count > 5)
				{
					res.set_status_code("408");//modify later
					kill(pid, SIGKILL);//we should exit the child process
					errorResponse(res);
					dup2(STDIN_FILENO, 0);
					dup2(STDOUT_FILENO, 1);
					close(fd);
					return;
				}
				count++;
				sleep(1);
			}
			std::istringstream stream;
			dup2(STDIN_FILENO, 0);
			dup2(STDOUT_FILENO, 1);
			close(fd);
			status = WEXITSTATUS(status);
			if (status == 500)
			{
				res.set_status_code("500");
				errorResponse(res);
				return;
			}
			else
			{
				res.set_status_code("200");
				if (file.is_open())
					file.close();
				file.open("output.html");
				struct stat buf;
				std::stringstream stream;
				if (!file.is_open() || stat("output.html", &buf) == -1)
				{
					res.set_status_code("500");
					errorResponse(res);
					return;
				}
				std::string header;
				header = "";
				if (php)
				{
					std::cout << "is php" << std::endl;
					std::string line;
					std::getline(file, line);
					while (line != "\r")
					{
						header += line + "\n";
						std::getline(file, line);
					}
					stream << buf.st_size - header.size() - 2;
				}
				else
				{
					std::cout << "is python" << std::endl;
					res.set_content_type("Content-Type: text/html\r\n");
					stream << buf.st_size;
				}
				res.set_content_length("Content-Length: " + stream.str() + "\r\n");
				res.set_header("HTTP/1.1 200 OK\r\n" + res.get_date() + res.get_content_type() + res.get_content_length() + header + "\r\n");
				std::cout << "heder size : " << res.get_header().size() << std::endl;
				sent_bytes = res.get_header().size();
				// std::cout << "wa safi 3afaak :\n" << res.get_header() << std::endl;
			}
		}
	}
	else
		readFile(res);
}