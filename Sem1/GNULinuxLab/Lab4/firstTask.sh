echo "firstVariable: ${firstVariable} | secondVariable: ${secondVariable}"

concatenation=$firstVariable$secondVariable
echo "Конкатенация: ${concatenation}"

substring=$(echo $concatenation | cut -c 1-5)
echo "Подстрока: ${substring}"

substring="new string"
echo "Изменёная строка: ${substring}"
