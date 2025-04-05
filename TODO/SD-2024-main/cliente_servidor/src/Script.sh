#!/bin/bash

# Caminho da pasta onde os ficheiros .class serão removidos
# Usa "." para a pasta atual ou substitui por outro caminho
DIR="."

# Remover todos os ficheiros .class dentro da pasta
find "$DIR" -type f -name "*.class" -delete

echo "Todos os ficheiros .class foram removidos da pasta $DIR."
