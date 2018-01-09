#! /bin/bash
# Script con la finalidad de lanzar tres clientes que funcionan autom�ticamente

echo "IP seleccionada: $1"
echo "Puerto seleccionado: $2"
echo "Lanzando clientes..."

xterm -e ./bin/Cliente $1 $2 Alberto https://www.w3schools.com/w3css/img_fjords.jpg &
xterm -e ./bin/Cliente $1 $2 Miguel http://1.bp.blogspot.com/-iS3Rk8DF7qA/UIbHvnp5CtI/AAAAAAAAAEs/M4GJtJTUDWM/s1600/zurdo+4.jpg &
xterm -e ./bin/Cliente $1 $2 Jorge  https://static.guim.co.uk/sys-images/Guardian/Pix/pictures/2014/4/11/1397210130748/Spring-Lamb.-Image-shot-2-011.jpg &
