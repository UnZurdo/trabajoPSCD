#! /bin/bash
# Script con la finalidad de lanzar tres clientes que funcionan autom�ticamente

echo "IP seleccionada: $1"
echo "Puerto seleccionado: $2"
echo "Lanzando clientes..."

# https://www.w3schools.com/w3css/img_fjords.jpg
xterm -e ./bin/Cliente $1 $2 Alberto http://www.gettyimages.es/gi-resources/images/Embed/new/embed2.jpg &
xterm -e ./bin/Cliente $1 $2 Miguel https://i.pinimg.com/originals/99/a8/b3/99a8b3766f813918c235dda4039f411d.jpg &
xterm -e ./bin/Cliente $1 $2 Jorge http://www.thinkstockphotos.com/ts-resources/images/home/TS_AnonHP_462882495_01.jpg &
