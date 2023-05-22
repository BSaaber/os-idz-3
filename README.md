# Мовшин Максим Антонович | БПИ213 | ИДЗ-3 | Вариант 12
## Условие задачи
Задача о гостинице – 1. В гостинице 30 номеров, гости гости- ницы снимают номер на одни или несколько суток. Если в гости- нице нет свободных номеров, гости не уходят, а устраиваются на рядом с гостиницей на скамейках и ждут, пока любой номеров не освободится. Создать приложение, моделирующее работу гостиницы. Сервер — это гостиница. Прибывающие гости мо- гут порождаться отдельным клиентом. Другой клиент — это скамейки, образующие очередь ожидающих гостей.

Замечания:
https://t.me/c/1895583371/2267
По предварительному обсуждению модифицировал условие задачи.
Скамейка также будет сервером. Также не вижу объективных причин фиксировать количество номеров, оно будет задаваться входным параметром.
## Решение
Три сущности:
- Люди (клиент)
- Отель (сервер)
- Скамейка (сервер)
Функционал Отеля:
Хранит информацию о количестве свободных комнат. 
- На запрос `free` (освободить комнату): Всегда отвечает `okay`, увеличивая количество свободных комнат.
- На запрос `rent` (занять комнату): При наличии свободных комнат отвечает `okay` и уменьшает количество свободных комнат. При отсутствии - отвечает `fail`.
Функционал Скамейки:
- На запрос `sitt` (сесть на скамейку): Всегда отвечает `okay`, эмулируя приход на скамейку.
Функционал Людей:
Клиент хранит информацию о следующем действии каждого человека, и, если надо, количество дней (итераций цикла), которые он еще будет спать.
В бесконечном цикле каждую итерацию Клиент выполняет следующее `действие` по очереди для каждого человека.
Возможные действия человека:
`rent` - попытаться арендовать комнату (запрос к Отелю). Если вышло -> спать. Если нет -> пойти на скамейку.
`slph` - проспать день в отеле, уменьшив количество оставшихся дней сна на 1. Если еще есть время для сна, то намереваться спать дальше. Иначе -> попытаться сдать комнату на след. день.
`slps` - проспать день на скамейке, уменьшив количество оставшихся дней сна на 1. Если еще есть время для сна, то намереваться спать дальше. Иначе -> попытаться арендовать комнату на след. день.
`free` - сдать комнату (запрос к Отелю). На след день попытаться  вновь арендовать комнату.
`sitt` - пойти спать на скамейку (запрос к Скамейке).

**Остановка** программы осуществляется посредством нажатия комбинации клавиш `Ctrl+C` в каждом из терминалов. Предпочтительно начинать с терминала Клиента.
### Компиляция
```
make all
```
### Запуск
**Начинать** запуск следует с серверов.
1. Отель
```
sudo ./server <server port> <rooms amount>
```
2. Скамейка
```
sudo ./skameika
```
3. Люди
```
sudo ./client <hotel ip> <hotel port> <people amount> <day delay in seconds> <skameika ip> <skameika port>
```
### Пример работы
1. Запуск Отеля
```
sudo ./server 7 5
```
2. Запуск Скамейки
```
sudo ./skameika
```
3. Запуск Людей
```
sudo ./client 127.0.0.1 7 10 2 127.0.0.1 9
```

Логи Отеля:
```
Server IP address = 2.86.0.0. Wait...
Handling client 127.0.0.1
log: free space - 5
log: allocating room. space now - 4
log: connection closed
Handling client 127.0.0.1
log: free space - 4
log: allocating room. space now - 3
log: connection closed
Handling client 127.0.0.1
log: free space - 3
log: allocating room. space now - 2
log: connection closed
Handling client 127.0.0.1
log: free space - 2
log: allocating room. space now - 1
log: connection closed
Handling client 127.0.0.1
log: free space - 1
log: allocating room. space now - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: all rooms are allocated. free space - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: all rooms are allocated. free space - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: all rooms are allocated. free space - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: all rooms are allocated. free space - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: all rooms are allocated. free space - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: freeing room. space now - 1
log: connection closed
Handling client 127.0.0.1
log: free space - 1
log: allocating room. space now - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: freeing room. space now - 1
log: connection closed
Handling client 127.0.0.1
log: free space - 1
log: freeing room. space now - 2
log: connection closed
Handling client 127.0.0.1
log: free space - 2
log: allocating room. space now - 1
log: connection closed
Handling client 127.0.0.1
log: free space - 1
log: allocating room. space now - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: all rooms are allocated. free space - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: freeing room. space now - 1
log: connection closed
Handling client 127.0.0.1
log: free space - 1
log: freeing room. space now - 2
log: connection closed
Handling client 127.0.0.1
log: free space - 2
log: allocating room. space now - 1
log: connection closed
Handling client 127.0.0.1
log: free space - 1
log: allocating room. space now - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: all rooms are allocated. free space - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: all rooms are allocated. free space - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: all rooms are allocated. free space - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: all rooms are allocated. free space - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: freeing room. space now - 1
log: connection closed
Handling client 127.0.0.1
log: free space - 1
log: freeing room. space now - 2
log: connection closed
Handling client 127.0.0.1
log: free space - 2
log: freeing room. space now - 3
log: connection closed
Handling client 127.0.0.1
log: free space - 3
log: freeing room. space now - 4
log: connection closed
Handling client 127.0.0.1
log: free space - 4
log: allocating room. space now - 3
log: connection closed
Handling client 127.0.0.1
log: free space - 3
log: allocating room. space now - 2
log: connection closed
Handling client 127.0.0.1
log: free space - 2
log: allocating room. space now - 1
log: connection closed
Handling client 127.0.0.1
log: free space - 1
log: allocating room. space now - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: all rooms are allocated. free space - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: all rooms are allocated. free space - 0
log: connection closed
Handling client 127.0.0.1
log: free space - 0
log: freeing room. space now - 1
log: connection closed
^C
```

Логи Людей:
```
processing client 0
client tries to rent a room
message, that client 0 recieved: okay
horaay! i rent a room! now i go to sleep for 4 days


processing client 1
client tries to rent a room
message, that client 1 recieved: okay
horaay! i rent a room! now i go to sleep for 2 days


processing client 2
client tries to rent a room
message, that client 2 recieved: okay
horaay! i rent a room! now i go to sleep for 4 days


processing client 3
client tries to rent a room
message, that client 3 recieved: okay
horaay! i rent a room! now i go to sleep for 3 days


processing client 4
client tries to rent a room
message, that client 4 recieved: okay
horaay! i rent a room! now i go to sleep for 3 days


processing client 5
client tries to rent a room
message, that client 5 recieved: fail
oh no, all rooms are allocated! now i go to sleep on skameika


processing client 6
client tries to rent a room
message, that client 6 recieved: fail
oh no, all rooms are allocated! now i go to sleep on skameika


processing client 7
client tries to rent a room
message, that client 7 recieved: fail
oh no, all rooms are allocated! now i go to sleep on skameika


processing client 8
client tries to rent a room
message, that client 8 recieved: fail
oh no, all rooms are allocated! now i go to sleep on skameika


processing client 9
client tries to rent a room
message, that client 9 recieved: fail
oh no, all rooms are allocated! now i go to sleep on skameika


------------------


processing client 0
client sleeps


processing client 1
client sleeps


processing client 2
client sleeps


processing client 3
client sleeps


processing client 4
client sleeps


processing client 5
client goes to skameika
message, that client 5 recieved: okay
i got to the skameika.
I am at skameika now. I go to sleep on skameika for 4 days


processing client 6
client goes to skameika
message, that client 6 recieved: okay
i got to the skameika.
I am at skameika now. I go to sleep on skameika for 3 days


processing client 7
client goes to skameika
message, that client 7 recieved: okay
i got to the skameika.
I am at skameika now. I go to sleep on skameika for 2 days


processing client 8
client goes to skameika
message, that client 8 recieved: okay
i got to the skameika.
I am at skameika now. I go to sleep on skameika for 2 days


processing client 9
client goes to skameika
message, that client 9 recieved: okay
i got to the skameika.
I am at skameika now. I go to sleep on skameika for 2 days


------------------


processing client 0
client sleeps


processing client 1
client sleeps
client woke up
i will free a room tomorrow


processing client 2
client sleeps


processing client 3
client sleeps


processing client 4
client sleeps


processing client 5
client sleeps


processing client 6
client sleeps


processing client 7
client sleeps


processing client 8
client sleeps


processing client 9
client sleeps


------------------


processing client 0
client sleeps


processing client 1
client goes to free a room
message, that client 1 recieved: okay
i freed a room, i will try to rent a room tomorrow

processing client 2
client sleeps


processing client 3
client sleeps
client woke up
i will free a room tomorrow


processing client 4
client sleeps
client woke up
i will free a room tomorrow


processing client 5
client sleeps


processing client 6
client sleeps


processing client 7
client sleeps
client woke up
i will try to rent a room tomorrow


processing client 8
client sleeps
client woke up
i will try to rent a room tomorrow


processing client 9
client sleeps
client woke up
i will try to rent a room tomorrow


------------------


processing client 0
client sleeps
client woke up
i will free a room tomorrow


processing client 1
client tries to rent a room
message, that client 1 recieved: okay
horaay! i rent a room! now i go to sleep for 4 days


processing client 2
client sleeps
client woke up
i will free a room tomorrow


processing client 3
client goes to free a room
message, that client 3 recieved: okay
i freed a room, i will try to rent a room tomorrow

processing client 4
client goes to free a room
message, that client 4 recieved: okay
i freed a room, i will try to rent a room tomorrow

processing client 5
client sleeps


processing client 6
client sleeps
client woke up
i will try to rent a room tomorrow


processing client 7
client tries to rent a room
message, that client 7 recieved: okay
horaay! i rent a room! now i go to sleep for 2 days


processing client 8
client tries to rent a room
message, that client 8 recieved: okay
horaay! i rent a room! now i go to sleep for 1 days


processing client 9
client tries to rent a room
message, that client 9 recieved: fail
oh no, all rooms are allocated! now i go to sleep on skameika


------------------


processing client 0
client goes to free a room
message, that client 0 recieved: okay
i freed a room, i will try to rent a room tomorrow

processing client 1
client sleeps


processing client 2
client goes to free a room
message, that client 2 recieved: okay
i freed a room, i will try to rent a room tomorrow

processing client 3
client tries to rent a room
message, that client 3 recieved: okay
horaay! i rent a room! now i go to sleep for 1 days


processing client 4
client tries to rent a room
message, that client 4 recieved: okay
horaay! i rent a room! now i go to sleep for 1 days


processing client 5
client sleeps
client woke up
i will try to rent a room tomorrow


processing client 6
client tries to rent a room
message, that client 6 recieved: fail
oh no, all rooms are allocated! now i go to sleep on skameika


processing client 7
client sleeps


processing client 8
client sleeps
client woke up
i will free a room tomorrow


processing client 9
client goes to skameika
message, that client 9 recieved: okay
i got to the skameika.
I am at skameika now. I go to sleep on skameika for 1 days


------------------


processing client 0
client tries to rent a room
message, that client 0 recieved: fail
oh no, all rooms are allocated! now i go to sleep on skameika


processing client 1
client sleeps


processing client 2
client tries to rent a room
message, that client 2 recieved: fail
oh no, all rooms are allocated! now i go to sleep on skameika


processing client 3
client sleeps
client woke up
i will free a room tomorrow


processing client 4
client sleeps
client woke up
i will free a room tomorrow


processing client 5
client tries to rent a room
message, that client 5 recieved: fail
oh no, all rooms are allocated! now i go to sleep on skameika


processing client 6
client goes to skameika
message, that client 6 recieved: okay
i got to the skameika.
I am at skameika now. I go to sleep on skameika for 1 days


processing client 7
client sleeps
client woke up
i will free a room tomorrow


processing client 8
client goes to free a room
message, that client 8 recieved: okay
i freed a room, i will try to rent a room tomorrow

processing client 9
client sleeps
client woke up
i will try to rent a room tomorrow


------------------


processing client 0
client goes to skameika
message, that client 0 recieved: okay
i got to the skameika.
I am at skameika now. I go to sleep on skameika for 4 days


processing client 1
client sleeps


processing client 2
client goes to skameika
message, that client 2 recieved: okay
i got to the skameika.
I am at skameika now. I go to sleep on skameika for 4 days


processing client 3
client goes to free a room
message, that client 3 recieved: okay
i freed a room, i will try to rent a room tomorrow

processing client 4
client goes to free a room
message, that client 4 recieved: okay
i freed a room, i will try to rent a room tomorrow

processing client 5
client goes to skameika
message, that client 5 recieved: okay
i got to the skameika.
I am at skameika now. I go to sleep on skameika for 4 days


processing client 6
client sleeps
client woke up
i will try to rent a room tomorrow


processing client 7
client goes to free a room
message, that client 7 recieved: okay
i freed a room, i will try to rent a room tomorrow

processing client 8
client tries to rent a room
message, that client 8 recieved: okay
horaay! i rent a room! now i go to sleep for 1 days


processing client 9
client tries to rent a room
message, that client 9 recieved: okay
horaay! i rent a room! now i go to sleep for 1 days


------------------


processing client 0
client sleeps


processing client 1
client sleeps
client woke up
i will free a room tomorrow


processing client 2
client sleeps


processing client 3
client tries to rent a room
message, that client 3 recieved: okay
horaay! i rent a room! now i go to sleep for 3 days


processing client 4
client tries to rent a room
message, that client 4 recieved: okay
horaay! i rent a room! now i go to sleep for 1 days


processing client 5
client sleeps


processing client 6
client tries to rent a room
message, that client 6 recieved: fail
oh no, all rooms are allocated! now i go to sleep on skameika


processing client 7
client tries to rent a room
message, that client 7 recieved: fail
oh no, all rooms are allocated! now i go to sleep on skameika


processing client 8
client sleeps
client woke up
i will free a room tomorrow


processing client 9
client sleeps
client woke up
i will free a room tomorrow


------------------


processing client 0
client sleeps


processing client 1
client goes to free a room
message, that client 1 recieved: okay
i freed a room, i will try to rent a room tomorrow

^C
```

Логи Скамейки:
```
Server IP address = 234.85.0.0. Wait...
Handling client 127.0.0.1
log: person sits on skameika
log: connection closed
Handling client 127.0.0.1
log: person sits on skameika
log: connection closed
Handling client 127.0.0.1
log: person sits on skameika
log: connection closed
Handling client 127.0.0.1
log: person sits on skameika
log: connection closed
Handling client 127.0.0.1
log: person sits on skameika
log: connection closed
Handling client 127.0.0.1
log: person sits on skameika
log: connection closed
Handling client 127.0.0.1
log: person sits on skameika
log: connection closed
Handling client 127.0.0.1
log: person sits on skameika
log: connection closed
Handling client 127.0.0.1
log: person sits on skameika
log: connection closed
Handling client 127.0.0.1
log: person sits on skameika
log: connection closed
^C
```
