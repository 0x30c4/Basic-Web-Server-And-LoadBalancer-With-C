# A simple Web Server and Loadbalancer.
## Inspired by nignx.

# Prerequisite

* linux
* gcc (v10+)
* make
* git

# Build 

To build first clone and then use ```make``` to build a stand alone application.

And the final binary will be in the ```./bin``` directory.

```
$ git clone https://github.com/0x30c4/Basic-Web-Server-And-LoadBalancer-With-C.git 
$ cd Basic-Web-Server-And-LoadBalancer-With-C
$ make
```

# Run

There are to modes the server can run. 
First the static web server mode. For that you need to setup a conf file like this.

```
www_path: ./tests/test_files;
listen_port: 3344;
mime_file: ./mime.types;
```

The ```www_path``` is the static web servers root directory.
```listen_port``` is the servers listening port.
And ```mime_file``` is for the ```Content-Type``` header. This files looks something like this.

```
text/html html htm shtml;
text/css css;
text/xml xml;
image/gif gif;
image/jpeg jpeg jpg;
application/javascript js;
application/atom+xml atom;
application/rss+xml rss;
```
As you can see the file contains ```Content-Type``` header and the corresponding file extension.

