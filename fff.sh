 sudo apt-get install build-essential gcc make cmake cmake-gui cmake-curses-gui
 sudo apt-get install libssl-dev 
 sudo apt-get install doxygen graphviz

 git clone https://github.com/eclipse/paho.mqtt.c.git
 cd paho.mqtt.c
 git checkout v1.3.8

 cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON \
    -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON
 sudo cmake --build build/ --target install
 sudo ldconfig

 cmake -Bbuild -H. -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_STATIC=ON \
    -DPAHO_WITH_SSL=ON -DPAHO_HIGH_PERFORMANCE=ON \
    -DCMAKE_INSTALL_PREFIX=./build/_install

 git clone https://github.com/eclipse/paho.mqtt.cpp
 cd paho.mqtt.cpp

 cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON \
    -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE
 sudo cmake --build build/ --target install
 sudo ldconfig

 cmake -Bbuild -H. -DPAHO_BUILD_DOCUMENTATION=ON -DPAHO_BUILD_SAMPLES=ON \
    -DPAHO_BUILD_STATIC=ON \
    -DCMAKE_PREFIX_PATH=$HOME/mqtt/paho.mqtt.c/build/_install

 CXX=clang++ cmake ..

 cmake -DCMAKE_CXX_COMPILER=clang++

 cmake -Bbuild -H. -DPAHO_WITH_SSL=ON -DPAHO_ENABLE_TESTING=OFF -DPAHO_BUILD_DEB_PACKAGE=ON
 cmake --build build
 (cd build && cpack)

