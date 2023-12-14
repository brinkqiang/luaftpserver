echo sudo docker swarm init
sudo docker service create --publish mode=host,target=2121,published=52121,protocol=tcp --name luaftpserver luaftpserver
