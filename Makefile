all:
	rm -f infinitylights
	g++ -lusb-1.0  main.cpp InfinityPortal.cpp SkylandersPortal.cpp -o infinitylights

lightshow:
	rm -f lightshow
	g++ -lusb-1.0  lightshow.cpp InfinityPortal.cpp SkylandersPortal.cpp -o lightshow

clean:
	rm -f infinitylights lightshow