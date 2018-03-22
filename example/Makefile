all:producer pullConsumer pushConsumer
producer:Producer.cpp
	c++ -I ../include  -rdynamic -o producer Producer.cpp -ldl -Wl,-rpath,\$$ORIGIN/..
pullConsumer:PullConsumer.cpp
	c++ -I ../include  -rdynamic -o pullConsumer PullConsumer.cpp -ldl -Wl,-rpath,\$$ORIGIN/..
pushConsumer:PushConsumer.cpp
	c++ -I ../include  -rdynamic -o pushConsumer PushConsumer.cpp -ldl -Wl,-rpath,\$$ORIGIN/..

clean:
	rm -f producer pushConsumer pullConsumer
