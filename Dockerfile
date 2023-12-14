
FROM ubuntu:latest

# Set Env

# Copy application files
COPY . /home/myapp/luaftp
WORKDIR /home/myapp/luaftp

# Install dependencies

RUN bash build.sh
WORKDIR /home/myapp/luaftp/bin/relwithdebinfo/root
WORKDIR /home/myapp/luaftp/bin/relwithdebinfo/user
WORKDIR /home/myapp/luaftp/bin/relwithdebinfo/anonymous
WORKDIR /home/myapp/luaftp/bin/relwithdebinfo

# Start the application
CMD ["./lua", "../script/main.lua"]