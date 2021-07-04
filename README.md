# webserver

The aim of this project is to make you write your own HTTP server. You will be able to test it with a real browser. 

HTTP is one of the most used protocol on internet. 

Knowing its arcane will be useful, even if you won't be working on a website.


**Subject:**
- Your server must be compatible with the web browser of your choice.
- We will consider that Nginx is HTTP 1.1 compliant and may be used to compare headers and answer behaviors.
- It must be non-blocking and use only 1 poll (or equivalent) for all the IO between the client and the server (listens includes).
- Poll (or equivalent) should check read and write at the same time.
- Your server should never block and the client should be bounce properly if necessary.
- You should never do a read operation or a write operation without going through poll (or equivalent).
- Checking the value of errno is strictly forbidden after a read or a write operation.
- A request to your server should never hang forever.
- You server should have default error pages if none are provided.
-  Your program should not leak and should never crash, (even when out of memory if all the initialization is done)
- You can’t use fork for something else than CGI (like php or python etc...)
- You can’t execve another webserver...
- Your program should have a config file in argument or use a default path.
- You don’t need to use poll (or equivalent) before reading your config file.
- You should be able to serve a fully static website.
- Client should be able to upload files.
- Your HTTP response status codes must be accurate.
- You need at least GET, POST, and DELETE methods.
- Stress tests your server it must stay available at all cost.
- Your server can listen on multiple ports (See config file).

In this config file we should be able to: 
  - choose the port and host of each "server"
  - setup the server_names or not
  - The first server for a host:port will be the default for this host:port (meaning it will answer to all request that doesn’t belong to an other server)
  - setup default error pages
  - limit client body size
  - setup routes with one or multiple of the following rules/configuration (routes wont be using regexp):
    ∗ define a list of accepted HTTP Methods for the route
    ∗ define an HTTP redirection.
    ∗ define a directory or a file from where the file should be search (for example if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet)
    ∗ turn on or off directory listing
    ∗ default file to answer if the request is a directory
    ∗ execute CGI based on certain file extension (for example .php)
  -  Your program should call the cgi with the file requested as first argument
  - the cgi should be run in the correct directory for relativ path file access
  - your server should work with one CGI (php-cgi, python...)
    ∗ make the route able to accept uploaded files and configure where it should be saved
    
 
**Usage and Testing:**

Running make will make the library libasm.a

```
git clone https://github.com/webservuss/webserver.git
cd webserver
make
```

To test simply run the executable test

```
./test
```
and type the following in browser (recommended to use with chrome)
```
127.0.0.1
```

and for a specific function e.g. strlen
