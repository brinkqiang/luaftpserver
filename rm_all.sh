
sudo docker service rm $(sudo docker service ls | awk {'print $2'} | grep -v NAME)

sudo docker stop $(sudo docker ps -aq | awk {'print $1'} | grep -v CONTAINER)
sudo docker rm -f $(sudo docker ps -aq | awk {'print $1'} | grep -v CONTAINER)
sudo docker rmi -f $(sudo docker images -aq | awk {'print $1'} )
sudo docker builder prune --all --force
