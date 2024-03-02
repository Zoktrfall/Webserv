#pragma once
# include <iostream>
# include <stdio.h>
# include <vector>
# include <unistd.h>
# include <arpa/inet.h>
# include <sys/socket.h>

# define PORT 8080
# define MAX_BUFFER_SIZE 1024

void	getHostIP(void);
int		server(void);