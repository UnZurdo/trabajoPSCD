#! /bin/bash
# Script con la finalidad de lanzar tres clientes que funcionan autom�ticamente

echo "IP seleccionada: $1"
echo "Puerto seleccionado: $2"
echo "Lanzando clientes..."

xterm -e ./bin/Cliente $1 $2 Alberto https://www.w3schools.com/w3css/img_fjords.jpg &
xterm -e ./bin/Cliente $1 $2 Miguel https://cdn.pixabay.com/photo/2013/04/06/11/50/image-editing-101040_960_720.jpg &
xterm -e ./bin/Cliente $1 $2 Jorge  https://static.guim.co.uk/sys-images/Guardian/Pix/pictures/2014/4/11/1397210130748/Spring-Lamb.-Image-shot-2-011.jpg &
