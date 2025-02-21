
//silver_chain_scope_start
//mannaged by silver chain
#include "../../imports/imports.fdeclare.h"
//silver_chain_scope_end


static int private_BearHttpsRequest_host_connect(BearHttpsResponse *self,const char *host,int port){

    struct addrinfo hints, *si, *p;
	Universal_ZeroMemory(&hints, sizeof(hints));
	int main_file_descriptor;

	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	char port_in_str[10] ={0};
	sprintf(port_in_str,"%d",port);
	int err = Universal_getaddrinfo(host, port_in_str, &hints, &si);

	if (err != 0) {
	   BearHttpsResponse_set_error_msg(self,gai_strerror(err));
	   return -1;
	}
	main_file_descriptor = -1;
	for (p = si; p != NULL; p = p->ai_next) {
		struct sockaddr *sa;
		void *addr;
		char tmp[INET6_ADDRSTRLEN + 50];

		sa = (struct sockaddr *)p->ai_addr;
		if (sa->sa_family == AF_INET) {
			addr = &((struct sockaddr_in *)sa)->sin_addr;
		} else if (sa->sa_family == AF_INET6) {
			addr = &((struct sockaddr_in6 *)sa)->sin6_addr;
		} else {
			addr = NULL;
		}
		if (addr != NULL) {
			Universal_inet_ntop(p->ai_family, addr, tmp, sizeof tmp);
		} else {
			sprintf(tmp, "<unknown family: %d>",(int)sa->sa_family);
		}

		main_file_descriptor = Universal_socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (main_file_descriptor < 0) {
			continue;
		}
		if (Universal_connect(main_file_descriptor, p->ai_addr, p->ai_addrlen) < 0) {
			Universal_close(main_file_descriptor);
			continue;
		}
		break;
	}

	if (p == NULL) {
		Universal_freeaddrinfo(si);
		BearHttpsResponse_set_error_msg(self,"ERROR: failed to connect\n");
		return -1;
	}
	Universal_freeaddrinfo(si);

	return main_file_descriptor;
}
