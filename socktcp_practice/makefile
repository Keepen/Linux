all:tcp_cli thread_srv
thread_srv:thread_srv.cpp
	g++ $^ -o $@ -lpthread
pro_srv:pro_srv.cpp
	g++ $^ -o $@
tcp_cli:tcp_cli.cpp
	g++ $^ -o $@
tcp_srv:tcp_srv.cpp
	g++ $^ -o $@
