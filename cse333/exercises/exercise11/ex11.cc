// Copyright © 2023 Sebatian Liu.
//
// Name: Sebastian Liu
// CSE Email Address: ll57@cs.washington.edu

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

#include "./SocketUtil.h"

#define BUF_SIZE 1024
#define STDOUT_FD 1

using std::cerr;
using std::endl;

void Usage();

// Create a listening socket, accept a connection from a client,
// and write all the data the client sends to stdout.
int main(int argc, char **argv) {
  int sock_family, listen_fd, client_fd, read_res, write_res;
  unsigned char readbuf[BUF_SIZE];

  if (argc != 2) {
    Usage();
  }

  // Create the listening socket.
  listen_fd = Listen(argv[1], &sock_family);
  if (listen_fd == -1) {
    cerr << "Failed listening to socket " << endl;
    Usage();
  }

  // Loop forever, accepting a connection from a client.
  while (1) {
    struct sockaddr_storage caddr;
    socklen_t caddr_len = sizeof(caddr);
    client_fd = accept(listen_fd, reinterpret_cast<struct sockaddr *>(&caddr),
                       &caddr_len);
    if (client_fd < 0) {
      if ((errno == EAGAIN) || (errno == EINTR) || (errno == EWOULDBLOCK)) {
        continue;
      }
      cerr << "Failure on accept: " << strerror(errno) << endl;
      Usage();
    }
    break;
  }

  // Handle client, reading data from client_fd and writing it to stdout.
  while (1) {
    // Read from the file.
    read_res = WrappedRead(client_fd, readbuf, BUF_SIZE);
    if (read_res <= 0) {
      // EOF or error.
      break;
    }

    // Write to the socket.
    write_res = WrappedWrite(STDOUT_FD, readbuf, read_res);
    if (write_res != read_res) {
      // Error.
      break;
    }
  }

  // clean up
  close(client_fd);
  close(listen_fd);
  return EXIT_SUCCESS;
}

void Usage() {
  cerr << "usage: ./ex11 <PORT>" << endl;
  exit(EXIT_FAILURE);
}
