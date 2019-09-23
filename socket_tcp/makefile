all:thr_srv pro_srv tcp_srv tcp_cli
thr_srv:thr_srv.cpp
	g++ $^ -o $@ -lpthread
pro_srv:pro_srv.cpp
	g++ $^ -o $@
tcp_srv:tcp_srv.cpp
	g++ $^ -o $@
tcp_cli:tcp_cli.cpp
	g++ $^ -o $@
