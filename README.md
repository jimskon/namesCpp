## namesCpp
## A C++ web program using a microservice to search for name statistics
## Uses cpp-httplib for the microservice
 - ```https://github.com/yhirose/cpp-httplib```
 
## Install SSL and compression services
 - ```sudo apt-get install libssl-dev```
 - ```sudo apt-get install libz-dev```
 
## Set Javascript IP address to your VM address
 - Edit ```namelookup.js``` so that ```baseUrl``` is your VM's IP address

## Set up App
 - ```sudo mkdir /var/www/html/namesCpp/```
 - ```sudo chown ubuntu /var/www/html/namesCpp/```

## Make and run
 - ```make```
 - ```./namesAPI```

## Go to your VM URL/namesCpp/namelookup.html
