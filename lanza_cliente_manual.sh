#! /bin/bash
# Script con la finalidad de lanzar tres clientes que funcionan autom�ticamente

echo "IP seleccionada: $1"
echo "Puerto seleccionado: $2"
echo "URL seleccionada: $3"
echo "Lanzando cliente..."

# https://www.w3schools.com/w3css/img_fjords.jpg
./bin/Cliente $1 $2 $3

