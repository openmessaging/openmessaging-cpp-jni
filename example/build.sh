#!/usr/bin/env bash
c++ -I ../include  -rdynamic -lboost_thread -lboost_filesystem -lboost_chrono -o producer Producer.cpp -ldl