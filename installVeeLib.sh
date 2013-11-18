#!/bin/bash

(
	cd ./VeeLib/
	mkdir lib
	cd lib
	cmake .. && make -j4 && sudo make install -j4
)