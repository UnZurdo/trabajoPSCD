#! /bin/bash
# Script con la finalidad de lanzar tres clientes que funcionan autom�ticamente

echo "IP seleccionada: $1"
echo "Puerto seleccionado: $2"
echo "Lanzando clientes..."

# https://www.w3schools.com/w3css/img_fjords.jpg
xterm -e ./bin/Cliente $1 $2 Alberto http://assets2.motherboard.tv/content-images/contentimage/no-slug/7703bcea71c8096ea7c18addc28e8a15.jpg &
xterm -e ./bin/Cliente $1 $2 Miguel http://1.bp.blogspot.com/-iS3Rk8DF7qA/UIbHvnp5CtI/AAAAAAAAAEs/M4GJtJTUDWM/s1600/zurdo+4.jpg &
xterm -e ./bin/Cliente $1 $2 Jorge  http://assets2.motherboard.tv/content-images/contentimage/no-slug/7703bcea71c8096ea7c18addc28e8a15.jpg &
