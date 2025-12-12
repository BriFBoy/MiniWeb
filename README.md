
# MiniWeb

This projects is lets you host a website using this backend writen in c.


## Environment Variables

To run this project, you will need to add the following environment variables to your machine. You can add them by using the export command on linux. this will and them to this session

`MINIWEB_SUOURCE` This is the path to the acual website files


## Deployment

To deploy this project run you will need to have a linux or at least a unix based system installed and ready. the first thing is cloneing the repository, which you can do with the following command.

```bash
  git clone https://github.com/BriFBoy/MiniWeb.git
  cd MiniWeb
```
Next you will need to run the dockerfile to build the program from source and create a docker image that you can run to host your website. This can be done with this command
```bash
  docker build -t miniweb:latest .
```
By default the dockerfile will set the MINIWEB_SOURCE env to 
./app this is where the website's files is stored by default. If you would like to change there the program gets the files from, then you can do that my using the -e argument when running the dockerfile. After you have built the image you can run it with this command
```bash
  docker run --name A_NAME_YOU_WANT -p 8080:8080 -d miniweb:latest
```
Now you have a working, simple webserver that can host your website


## Run Locally

Running this project locally is super easy. All you need to do is these 3 tings. First is cloneing the repository

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
