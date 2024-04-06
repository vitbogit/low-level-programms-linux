# 2024 Лабораторные работы по курсу "Операционные системы" C/C++ (RU)

> ❤️ Дисклеймер  
Для меня качество моих работ на GO сейчас приоритетнее лабораторных в вузе на других языках (включая C/C++),  
поэтому, хотя работы и свежие, я не показал здесь свои текущие умения проектировать программы:   
❗ здесь только местами есть паттерны хорошей архитектуры и почти нигде нет покрытия тестами - эти must-have`ы хорошего программиста вы можете посмотреть в моих проектах на GO.   
Однако ценность этих лабораторных есть: она в знакомстве с Linux и освоении
низкоуровневых механизмов работы с ОС (exec, fork, треды, сокеты, примитивы синхронизации).  
В целом написано довольно аккуратно, местами даже вылизано до предела, местами возможны костыли и откровенные CNTRL-C/CNTRL-V.
> 

) Собственный архиватор.

Написать программу архиватор на языке C: 
- На вход программе-архиватору передается папка с файлами;
- Файлы имеют различное расширение (текст, картинка, что угожно);
- Результатом работы программы является один файл, объединяющий все файлы из входной директории, а также содержащий заголовок
с информацией об именах файлов и их размерах;
- Учесть возможность вложенной структуры папок;
- Добавить возможность архивирования в папку «по умолчанию» (файл-архив автоматически помещается в данную папку) и разархивирования в папку «по умолчанию» (в данной папке создается папка с названием архива и в нее отправляются все разархивированные файлы);
- При архивации сжатие можно не производить. 

Написать программу разархиватор, разделяющий архивированный файл обратно на файлы с учетом структуры папок. 

2) Собственный терминал (аналог bash).

Написать программу «терминал», которая анализирует входную строку и при обнаружении ключевых слов «ls», «cat», «nice» и «killall» запускает соответствующие процессы.

В программе реализовать возможность запуска процессов других программ, например, браузера.

Написать обработчики сигналов, например, при получении сигнала CTRL+C завершить запущенный программой процесс.

Реализовать пакетный, одновременный запуск программ.

3) Межпроцессное взаимодействие.

Программа 1. Программа читает заданный файл и выводит его содержимое в поток вывода с применениеМ механизма сокетов домена UNIX дляобмена данными между процессами на одном компьютере.
Программа 2. Программа запускает два экземпляра программы 1 и разными исходными файлами осуществляет с выходными данными программ побитовую операцию XOR и сохраняет результат в файл.

Запустить вторую программу и подать на вход текстовый файл и файл, содержащий случайную последовательность.
Запустить вторую программу с файлом, полученным во втором пункте и файлом, содержащим случайную последовательность.
Сравнить текстовый файл до и после шифрования.

4) Многопоточный фильтр Собела.

Многопоточный фильтр Собела. Программа получает фотографию на вход, делится на pthread и накладывает фильтр Собела. Каждый поток обрабатывает определенное количество строк исходного изображения. Программа фиксирует время своего выполнения. Запустить программу с 1,2,4,8, 16 и 32 потоками, зафиксировать время выполнения для каждой реализации. Сделать выводы.

5) Разделение ресурсов. Организация критической секции.

