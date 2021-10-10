/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 22:16:06 by eyohn             #+#    #+#             */
/*   Updated: 2021/10/10 17:21:10 by eyohn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Program name		webserv
** Makefile			yes
** External func	Everything in C++ 98. htons, htonl, ntohs, ntohl,
**					select, poll, epoll (epoll_create, epoll_ctl,
**					epoll_wait), kqueue (kqueue, kevent), socket,
**					accept, listen, send, recv, bind, connect,
**					inet_addr, setsockopt, getsockname, fcntl.
** Libft auth.		no
** Description		Write an HTTP server in C++ 98
**
** • You can use every macro and define like FD_SET, FD_CLR, FD_ISSET, FD_ZERO				+
** (understanding what they do and how they do it is very useful.)
** • You must write an HTTP server in C++ 98.												+
** • If you need more C functions, you can use them but always prefer C++.					+
** • The C++ standard must be C++ 98. Your project must compile with it.					+
** • No external library, no Boost, etc...													+
** • Try to always use the most "C++" code possible (for example use <cstring> over			+
** <string.h>).
** • Your server must be compatible with the web browser of your choice.					+
** // Ваш сервер должен быть совместим с выбранным вами веб-браузером.
** • We will consider that Nginx is HTTP 1.1 compliant and may be used to compare			+
** headers and answer behaviors.
** // Мы будем считать, что Нджинкс совместим с НТТР 1.1 и может использоваться для
** // для сравнения заголовков и поведения ответов.
** • In the subject and the scale we will mention poll but you can use equivalent like		+
** select, kqueue, epoll.
** • It must be non-blocking and use only 1 poll (or equivalent) for all the IO between		+
** the client and the server (listens includes).
** // В теме и шкале мы будем упоминать опрос, но вы можете использовать его эквиваленты,
** // такие как select, kqueue, epoll.
** // Он должен быть неблокирующим и использовать только 1 опрос (или эквивалент) для всех
** // операций ввода-вывода между клиентом и сервером (включая прослушивание).
** • poll (or equivalent) should check read and write at the same time.						+
** // Пулл или эквивалент следует проверять чтение и запись одновременно
** • Your server should never block and the client should be bounce properly if necessary.	+
** // Ваш сервер никогда не должен блокироваться и клиент при необходимости должен правильно
** // отказать
** • You should never do a read operation or a write operation without going through		+
** poll (or equivalent).
** // Ты не должен делать операцию чтения или записи без получения правильного пула
** • Checking the value of errno is strictly forbidden after a read or a write operation.	+
** // Проверка значения еррно после операции чтения или записи запрещено
** • A request to your server should never hang forever.									+
** // Запрос к вашему серверу никогда не должен висеть вечно
** • You server should have default error pages if none are provided.						+
** // Твой сервер должен иметь стандартную страницу ошибки если таковой не предусмотрено
** • Your program should not leak and should never crash, (even when out of memory			-
** if all the initialization is done)
** // Твой программа нее должна иметь утечек и падений даже если памяти нехватает
** // после выполения инициализации
** • You can’t use fork for something else than CGI (like php or python etc...)				+
** // Ты не можешь использовать форк для кроме как для сги (типа пайтон или пхп ...)
** • You can’t execve another webserver...													+
** // Ты не можешь запускать другой вебсервер
** • Your program should have a config file in argument or use a default path.				+
** // Твой программа должна иметь конфигфайл в качестве аргумента или с путем по умолчанию
** • You don’t need to use poll (or equivalent) before reading your config file.			+
** // Тебе не нужно использовать пул до чтения конфигурации
** • You should be able to serve a fully static website.									+
** // Ты должен уметь обслужить полностью статичный вебсайт
** • Client should be able to upload files.													+
** // Клиент должен уметь загружать файлы
** • Your HTTP response status codes must be accurate.										+
** // Коды состояния ответа НТТР должны быть точными
** • You need at least GET, POST, and DELETE methods.										+
** // Тебе нужны как минимум эти методы
** • Stress tests your server it must stay available at all cost.							-
** // Стресс тест должен устоять любой ценой
** • Your server can listen on multiple ports (See config file).							+
** // Твой сервер должен слушать несколько портов
**
** !!! We’ve let you use fcntl because mac os X doesn’t implement write the					+
** same way as other Unix OS.
** You must use non-blocking FD to have a result similar to other OS.						+
** // Фснтл разрешены так как макос не поддерживает запись так же как и другие уникс
** // Ты должен использовать неблокирующий ФД что бы иметь результат как на других ОС
**
** !!! Because you are using non-blocking FD, you could use read/recv or					+
** write/send functions without polling (or equivalent) and your server
** would be not blocking. But we don’t want that.
** Again trying to read/recv or write/send in any FD without going
** through a poll (or equivalent) will give you a mark equal to 0 and
** the end of the evaluation.
** // Так как ты используешь еблокирующий ФД, ты можешь использовать
** // райт/сенд функции без пулов и твой сервер не будет заблокирован
** // Но мы этого не хотим.
** // Использование рид/рес или райт/сенд в любых ФД без прохождения пула					+
** // приведет к оценке 0 и завершению оценки.
**
** !!! You can only use fcntl as follow: fcntl(fd, F_SETFL, O_NONBLOCK);					+
** Any other flags are forbiddens
** // Ты можешь использовать фснтл только так ...
** // любые другие флаги запрещены.
**
** BONUS
** • If the Mandatory part is not perfect don’t even think about bonuses					-
** // Если обязательная часть не идеальная то не думайте о бонусах
** • Support cookies and Session management (prepare quick examples).						-
** // Поддержка куки файлов и управление сессиями (нужны быстрые примеры)
** • Handle multiple CGI.																	+
** // Обработка нескольких сиджи
*/

//	1. запуск тестера															- 05.10-не работает
//	6. добавить отправку информации в логфайл после реализации всего функционала
//	11. проверить сервер на утечку дескрипторов
//	14. нужен ли нам цикл чтения большого количества запросов что бы попорядку
//		потом на них отвечать
//	20. избавиться от состояния EPOLLOUT
//	23. add check http:// path for valid
//	24. проверить все пункты сабжа
//	25. check body size with POST request
//	26. add use nameserver
//	27. add use body_size
//	+ 2. геттер в сервере на фавикон
//	+ 3. файл для фавикона
//	+ 4. обработать все краши на функциях
//	- 5. удалять сокеты при поступлении сигнала на закрытие
//	+ 7. добавить обработку переменных из тела запроса при приеме POST метода
//		1 + get body
//		2 + write body in _variables
//	+ 8. решить вопрос размера запроса
//	- 9. вопрос переменных окружения для сиджи
//	+ 12. разобраться как должен отрабатывать дефолтный сервер
//	+ 10. добавить обработку сиджи переданного в качестве аргумента
//		1 + get file_name
//		2 + check file_name for valid
//			a. + add supported format CGI file in config
//			b. + add handler for supported CGI files in config
//			c. + add checker in handler args
//		3 + execute CGI file
//		4 + exit server
//		5 - correct execute engine for websockets (работает и так)
//	- 13. добавить отправку закрытия соединения
//	- 15. добавить возможность держать несколько серверов на одной паре хост:порт
//	+ 16. добавить механизм отправки нужного кода ошибки
//			1 + add new class
//			2 + rewrite handler fd with new class
//	+ 17. добавить реализацию пайтон
//			1 + correct config_file
//			2 + check handler CGI scripts
//			3 + add execute file passed in args
//			4 + add execute file in request from client
//	+ 18. реализовать автоиндекс
//	+ 19. реализовать проверку версии http
//	+ 20. реализовать отправку заголовка content-leigth
//	+ 21. sendfile vs send
//	+ 22. add post method
//		- + fix bag in sendfile after POST + CGI

#include "../includes/headers.hpp"

bool	exit_flag;

static int		ft_check_socket(t_vars* vars, int fd)
{
	for (int i = 0; i < static_cast<int>(vars->sockets->size()); ++i)
	{
		if (fd == vars->sockets->operator[](i).getTcp_sockfd())
			return (1);
	}
	return (0);
}

int		main(int argc, char **argv, char **envp)
{
#ifdef DEBUG
	std::cout << "main start" << std::endl;
#endif
	// step 1: Inicialise data
	t_vars		vars;
	int			nfds;
	int			ret;

	// step 2: Parse config file
	ft_init_data(&vars, argc, argv, envp);

	// step 3: Create epoll fd
	if ((vars.epoll_fd = epoll_create(EPOLL_QUEUE_LEN)) < 0)
	{
		std::cerr << "ERROR in main: Epoll create error" << std::endl;
		ft_exit(&vars);
	}

	// step 4: Create socket and add in epoll queue
	for (unsigned long int i = 0; i < vars.servers->size(); ++i)
	{
		vars.sockets->push_back(Socket(&vars.servers->operator[](i)));

		// init sock_fd in server
		vars.servers->operator[](i).setSockFd(vars.sockets->operator[](i).getTcp_sockfd());

		vars.ev.events = EPOLLIN | EPOLLOUT;
		vars.ev.data.fd = vars.sockets->operator[](i).getTcp_sockfd();
		if (epoll_ctl(vars.epoll_fd, EPOLL_CTL_ADD, vars.sockets->operator[](i).getTcp_sockfd(), &vars.ev) == -1)
		{
			std::cerr << "ERROR in main: Epoll_ctl add error" << std::endl;
			ft_exit(&vars);
		}
	}
	
	// step 5: Write in log_file
	ft_write_in_log_file(&vars, "Server start");

	// step 6: wait action in socket and fd
	while (1)
	{
		nfds = epoll_wait(vars.epoll_fd, vars.events, EPOLL_QUEUE_LEN, TIMER_FOR_LISTEN);
		if (nfds == -1)
		{
			std::cerr << "ERROR in main: Epoll_wait error" << std::endl;
			ft_exit(&vars);
		}
		for (int i = 0; i < nfds; ++i)
		{
			if (vars.events[i].events & EPOLLIN || vars.events[i].events & EPOLLOUT)
				ft_handle_epoll_action(&vars, vars.events[i].data.fd, vars.events[i].events);
			else if (vars.events[i].events & EPOLLERR && !ft_check_socket(&vars, vars.events[i].data.fd))
			{
				// Headliners resp(std::string("HTTP/1.1"), std::string("408"));
				// resp.setCloseConnection(false);
				// resp.sendHeadliners(vars.events[i].data.fd);
				std::cerr << "EPOLLERR: fd = " << vars.events[i].data.fd << std::endl;
				if (epoll_ctl(vars.epoll_fd, EPOLL_CTL_DEL, vars.events[i].data.fd, &vars.ev) == -1)
				{
					std::cerr << "ERROR in main: Epoll_ctl del error" << std::endl;
					ft_exit(&vars);
				}
				if ((ret = close(vars.events[i].data.fd)) == -1)
					std::cerr << "ERROR in main: close fd error" << std::endl;
				// delete element from request container
				(vars.request_container->operator[](vars.events[i].data.fd))->~Response_2();
				// delete vars.request_container->operator[](vars.events[i].data.fd);
				vars.request_container->erase(vars.events[i].data.fd);
				// std::cerr << "Close fine (main)" << std::endl;
			}
			else
				std::cerr << "ERROR: UNHANDLED ACTION" << std::endl;
		}
	}

	// step 4: Create thread and start listen ports
	// for (long unsigned int i = 0; i < vars.sockets->size(); ++i)
	// {
	// 	vars.threads.emplace_back(std::thread(ft_in_thread, std::ref(vars), i));
	// 	sem_wait(vars.sema);
	// 	sleep(1);
	// }


	// step 6: Wait feedback from threads
	// sem_wait(vars.sema);
	// exit_flag = true;
	// for (long unsigned int i = 0; i < vars.threads.size(); ++i)
	// {
	// 	vars.threads.operator[](i).join();
	// }


#ifdef DEBUG
	std::cout << "main end" << std::endl;
#endif
	ft_exit(&vars);
	return (0);
}



// static void set_nonblocking(int fd) {
// 	int flags = fcntl(fd, F_GETFL, 0);
// 	if (flags == -1) {
// 		perror("fcntl()");
// 		return;
// 	}
// 	if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
// 		perror("fcntl()");
// 	}
// }

// int main(int argc, char **argv) {
// 	// create the server socket
// 	int sock = socket(AF_INET, SOCK_STREAM, 0);
// 	if (sock == -1) {
// 		perror("socket()");
// 		return 1;
// 	}
// 	int enable = 1;
// 	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) ==
// 		-1) {
// 		perror("setsockopt()");
// 		return 1;
// 	}

//   // bind
//   struct sockaddr_in addr;
//   memset(&addr, 0, sizeof(addr));
//   addr.sin_family = AF_INET;
//   addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
//   addr.sin_port = htons(PORT);
//   if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
// 	perror("bind()");
// 	return 1;
//   }

//   // make it nonblocking, and then listen
//   set_nonblocking(sock);
//   if (listen(sock, SOMAXCONN) < 0) {
// 	perror("listen()");
// 	return 1;
//   }

//   // create the epoll socket
//   int epoll_fd = epoll_create1(0);
//   if (epoll_fd == -1) {
// 	perror("epoll_create1()");
// 	return 1;
//   }

//   // mark the server socket for reading, and become edge-triggered
//   struct epoll_event event;
//   memset(&event, 0, sizeof(event));
//   event.data.fd = sock;
//   event.events = EPOLLIN | EPOLLET;
//   if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &event) == -1) {
// 	perror("epoll_ctl()");
// 	return 1;
//   }

//   struct epoll_event *events = calloc(MAXEVENTS, sizeof(event));
//   for (;;) {
// 	int nevents = epoll_wait(epoll_fd, events, MAXEVENTS, -1);
// 	if (nevents == -1) {
// 	  perror("epoll_wait()");
// 	  return 1;
// 	}
// 	for (int i = 0; i < nevents; i++) {
// 	  if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) ||
// 		  (!(events[i].events & EPOLLIN))) {
// 		// error case
// 		fprintf(stderr, "epoll error\n");
// 		close(events[i].data.fd);
// 		continue;
// 	  } else if (events[i].data.fd == sock) {
// 		// server socket; call accept as many times as we can
// 		for (;;) {
// 		  struct sockaddr in_addr;
// 		  socklen_t in_addr_len = sizeof(in_addr);
// 		  int client = accept(sock, &in_addr, &in_addr_len);
// 		  if (client == -1) {
// 			if (errno == EAGAIN || errno == EWOULDBLOCK) {
// 			  // we processed all of the connections
// 			  break;
// 			} else {
// 			  perror("accept()");
// 			  return 1;
// 			}
// 		  } else {
// 			printf("accepted new connection on fd %d\n", client);
// 			set_nonblocking(client);
// 			event.data.fd = client;
// 			event.events = EPOLLIN | EPOLLET;
// 			if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &event) == -1) {
// 			  perror("epoll_ctl()");
// 			  return 1;
// 			}
// 		  }
// 		}
// 	  } else {
// 		// client socket; read as much data as we can
// 		char buf[1024];
// 		for (;;) {
// 		  ssize_t nbytes = read(events[i].data.fd, buf, sizeof(buf));
// 		  if (nbytes == -1) {
// 			if (errno == EAGAIN || errno == EWOULDBLOCK) {
// 			  printf("finished reading data from client\n");
// 			  break;
// 			} else {
// 			  perror("read()");
// 			  return 1;
// 			}
// 		  } else if (nbytes == 0) {
// 			printf("finished with %d\n", events[i].data.fd);
// 			close(events[i].data.fd);
// 			break;
// 		  } else {
// 			fwrite(buf, sizeof(char), nbytes, stdout);
// 		  }
// 		}
// 	  }
// 	}
//   }
//   return 0;
// }