
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

const int SERVER_PORT = 5000;


int main(int argc, char *argv[]) {

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	int sock;
	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("could not create socket\n");
		return 1;
	}

	if ((bind(sock, (struct sockaddr *)&server_address, sizeof(server_address))) < 0) {
		printf("could not bind socket\n");
		return 1;
	}

	sockaddr_storage in_addr;
	socklen_t addr_len = sizeof(sockaddr_in); // addr_len = sizeof(sockaddr_in6);
	char buffer[1024];

	for(;;) {

		int len = ::recvfrom(sock, buffer, sizeof(buffer), 0, (sockaddr *) &in_addr, &addr_len);
		if (len==-1) {
			printf("could not read from socket %s\n", strerror(errno));
			return 1;
		} else if (len == sizeof(buffer))
			{
				printf("read buffer overflow\n");
				continue;
			} else
				buffer[len] = '\0';
		printf("received: '%s' from client %s\n", buffer, inet_ntoa(((sockaddr_in*)&in_addr)->sin_addr));
		::sendto(sock, buffer, len, 0, (struct sockaddr *)&in_addr, addr_len);
	}

	return 0;
}
