
# MiniWeb

MiniWeb is a lightweight C‑based web server designed for hosting static websites with no configuration needed. It’s fast, simple, and easy to deploy using Docker or by building locally.


## Environment Variables

To run MiniWeb, you will need to add the following environment variables to your machine. You can add them by using the export command on Linux. this will add them to this session

`MINIWEB_SOURCE` This is the path to the actual website files
## Prerequisites

Before you build and run MiniWeb, make sure your system has the following:

- **Linux, macOS, or WSL**
  MiniWeb is designed for Unix‑based systems. Windows users MUST use WSL or an equivalent.

- **GCC**
  Required to compile the MiniWeb source code.

- **make**
  Used to build the project using the provided Makefile.

- **Docker (optional)**
  Only needed if you plan to run MiniWeb inside a Docker container.


## Deployment

The first step is cloning the repository, which you can do with the following command

```bash
  git clone https://github.com/BriFBoy/MiniWeb.git
  cd MiniWeb
```
Next, you will need to run the Dockerfile to build the program from source and create a docker image that you can run to host your website. This can be done with this command
```bash
  docker build -t miniweb:latest .
```
By default, the Dockerfile will set the MINIWEB_SOURCE env to ./app this is where the website's files are stored by default. If you would like to change where the program gets the files from, then you can do that by using the -e argument when running the Dockerfile. After you have built the image you can run it with this command
```bash
  docker run --name A_NAME_YOU_WANT -p 8080:8080 -d miniweb:latest
```
Now you have a working, simple webserver that can host your website


## Run Locally

Running this project locally is super easy. All you need to do is these 3 steps. First is cloning the repository

```bash
  git clone https://github.com/BriFBoy/MiniWeb.git
```

Cd into the directory of the project

```bash
  cd MiniWeb
```

Build the program with make

```bash
  make build
```

And at the end run the program

```bash
  ./bin/program
```


## Authors

- [@BriFBoy](https://www.github.com/BriFBoy)
- [@ETalberg](https://www.github.com/ETalberg)
