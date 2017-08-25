<h2>Install Dependencies:</h2>
sudo apt-get install liblapack-dev<br>
sudo apt-get install libblas-dev<br>
sudo apt-get install libboost-dev<br>
sudo apt-get install libarmadillo-dev<br>
sudo apt-get install libboost-all-dev<br>

<h2>Install MLPack:</h2>
sudo apt-get install libmlpack-dev

<h2>Compile:</h2>
g++ -std=c++11 k-means.cpp -o k-means_exec -larmadillo -lmlpack -lboost_serialization

