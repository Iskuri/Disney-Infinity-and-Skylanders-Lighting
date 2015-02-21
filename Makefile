all:
	rm -f infinitylights
	g++ -lusb-1.0  main.cpp InfinityPortal.cpp SkylandersPortal.cpp -o infinitylights


clean:
	rm -f infinitylights