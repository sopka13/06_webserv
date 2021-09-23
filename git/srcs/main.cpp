/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eyohn <sopka13@mail.ru>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/11 22:16:06 by eyohn             #+#    #+#             */
/*   Updated: 2021/09/23 22:48:48 by eyohn            ###   ########.fr       */
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
** • You can use every macro and define like FD_SET, FD_CLR, FD_ISSET, FD_ZERO
** (understanding what they do and how they do it is very useful.)
** • You must write an HTTP server in C++ 98.
** • If you need more C functions, you can use them but always prefer C++.
** • The C++ standard must be C++ 98. Your project must compile with it.
** • No external library, no Boost, etc...
** • Try to always use the most "C++" code possible (for example use <cstring> over
** <string.h>).
** • Your server must be compatible with the web browser of your choice.
** // Ваш сервер должен быть совместим с выбранным вами веб-браузером.
** • We will consider that Nginx is HTTP 1.1 compliant and may be used to compare
** headers and answer behaviors.
** // Мы будем считать, что Нджинкс совместим с НТТР 1.1 и может использоваться для
** // для сравнения заголовков и поведения ответов.
** • In the subject and the scale we will mention poll but you can use equivalent like
** select, kqueue, epoll.
** • It must be non-blocking and use only 1 poll (or equivalent) for all the IO between
** the client and the server (listens includes).
** // В теме и шкале мы будем упоминать опрос, но вы можете использовать его эквиваленты,
** // такие как select, kqueue, epoll.
** // Он должен быть неблокирующим и использовать только 1 опрос (или эквивалент) для всех
** // операций ввода-вывода между клиентом и сервером (включая прослушивание). 
** • poll (or equivalent) should check read and write at the same time.
** // Пулл или эквивалент следует проверять чтение и запись одновременно
** • Your server should never block and the client should be bounce properly if necessary.
** // Ваш сервер никогда не должен блокироваться и клиент при необходимости должен правильно отказать
** • You should never do a read operation or a write operation without going through
** poll (or equivalent).
** // Ты не должен делать операцию чтения или записи без получения правильного пула
** • Checking the value of errno is strictly forbidden after a read or a write operation.
** // Проверка значения еррно после операции чтения или записи
** • A request to your server should never hang forever.
** // Запрос к вашему серверу никогда не должен висеть вечно
** • You server should have default error pages if none are provided.
** // Твой сервер должен иметь стандартную страницу ошибки если таковой не предусмотрено
** • Your program should not leak and should never crash, (even when out of memory
** if all the initialization is done)
** // Твой программа нее должна иметь утечек и падений даже если памяти нехватает
** // после выполения инициализации
** • You can’t use fork for something else than CGI (like php or python etc...)
** // Ты не можешь использовать форк для кроме как для сги (типа пайтон или пхп ...)
** • You can’t execve another webserver...
** // Ты не можешь запускать другой вебсервер
** • Your program should have a config file in argument or use a default path.
** // Твой программа должна иметь конфигфайл в качестве аргумента или с путем по умолчанию
** • You don’t need to use poll (or equivalent) before reading your config file.
** // Тебе не нужно использовать пул до чтения конфигурации
** • You should be able to serve a fully static website.
** // Ты должен уметь обслужить полностью статичный вебсайт
** • Client should be able to upload files.
** // Клиент должен уметь загружать файлы
** • Your HTTP response status codes must be accurate.
** // Коды состояния ответа НТТР должны быть точными
** • You need at least GET, POST, and DELETE methods.
** // Тебе нужны как минимум эти методы
** • Stress tests your server it must stay available at all cost.
** // Стресс тест должен устоять любой ценой
** • Your server can listen on multiple ports (See config file).
** // Твой сервер должен слушать несколько портов
**
** !!! We’ve let you use fcntl because mac os X doesn’t implement write the
** same way as other Unix OS.
** You must use non-blocking FD to have a result similar to other OS.
** // Фснтл разрешены так как макос не поддерживает запись так же как и другие уникс
** // Ты должен использовать неблокирующий ФД что бы иметь результат как на других ОС
**
** !!! Because you are using non-blocking FD, you could use read/recv or
** write/send functions without polling (or equivalent) and your server
** would be not blocking. But we don’t want that.
** Again trying to read/recv or write/send in any FD without going
** through a poll (or equivalent) will give you a mark equal to 0 and
** the end of the evaluation.
** // Так как ты используешь еблокирующий ФД, ты можешь использовать
** // райт/сенд функции без пулов и твой сервер не будет заблокирован
** // Но мы этого не хотим.
** // Использование рид/рес или райт/сенд в любых ФД без прохождения пула
** // приведет к оценке 0 и завершению оценки.
**
** !!! You can only use fcntl as follow: fcntl(fd, F_SETFL, O_NONBLOCK);
** Any other flags are forbidden
** // Ты можешь использовать фснтл только так ...
** // любые другие флаги запрещены.
**
** BONUS
** • If the Mandatory part is not perfect don’t even think about bonuses
** // Если обязательная часть не идеальная то не думайте о бонусах
** • Support cookies and Session management (prepare quick examples).
** // Поддержка куки файлов и управление сессиями (нужны быстрые примеры)
** • Handle multiple CGI.
** // Обработка нескольких сиджи
*/

//	1. запуск тестера															- 19.09-не работает
//	6. добавить отправку информации в логфайл после реализации всего функционала
//	7. добавить обработку переменных из тела запроса при приеме POST метода		- I'm here
//		1 get body
//		2 write body in _variables
//	8. решить вопрос размера запроса для POST метода
//	9. вопрос переменных окружения для сиджи
//	11. проверить сервер на утечку дескрипторов
//	14. нужен ли нам цикл чтения большого количества запросов что бы попорядку
//		потом на них отвечать
//	15. добавить возможность держать несколько серверов на одной паре хост:порт
//	16. добавить механизм отправки нужного кода ошибки							- and here
//			1 + add new class
//			2 rewrite handler fd with new class
//	17. реализовать автоиндекс
//	+ 2. геттер в сервере на фавикон
//	+ 3. файл для фавикона
//	+ 4. обработать все краши на функциях
//	- 5. удалять сокеты при поступлении сигнала на закрытие
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
//	+ 17. добавить реализацию пайтон
//			1 + correct config_file
//			2 + check handler CGI scripts
//			3 + add execute file passed in args
//			4 + add execute file in request from client

#include "../includes/headers.hpp"

bool	exit_flag;

int		main(int argc, char **argv, char **envp)
{
#ifdef DEBUG
	std::cout << "main start" << std::endl;
#endif
	// step 1: Inicialise data
	t_vars		vars;
	int			nfds;

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
		vars.sockets->emplace_back(Socket(&vars.servers->operator[](i)));

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
			if (vars.events[i].events & EPOLLIN)
				ft_handle_epoll_action(&vars, vars.events[i].data.fd);
			if (vars.events[i].events & EPOLLOUT)
			{
				// Headliners resp(std::string("HTTP/1.1"), std::string("408"));
				// resp.setCloseConnection(false);
				// resp.sendHeadliners(vars.events[i].data.fd);
				std::cout << "EPOLLOUT: Need handle. fd = " << vars.events[i].data.fd << std::endl;
				if (epoll_ctl(vars.epoll_fd, EPOLL_CTL_DEL, vars.events[i].data.fd, &vars.ev) == -1)
				{
					std::cerr << "ERROR in ft_handle_epoll_action: Epoll_ctl del error" << std::endl;
					ft_exit(&vars);
				}
				close(vars.events[i].data.fd);
			}
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