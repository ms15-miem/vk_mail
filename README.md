## vk_mail

### Кросспостер с гмылоящика на стенку группы ВКонтакте.

Утилита проверяет новые сообщения на почтовом ящике по imap и постит их на стену группы ВКонтакте.

Разумеется, исходный код легко можно поправить для постинга не в группу, а куда-нибудь ещё. При необходимости, создавайте тикет.

## Как использовать

###

Зависимости: libqt4-dev, libvmime-dev

### Запуск

*   git clone https://github.com/ms15-miem/vk_mail.git

*   cd vk_mail && qmake-qt4 && make

*   Запустить

### Завершение работы

Для завершения работы программы выполните команду:

*   Windows&nbsp;(all exemplars): taskkill /F /IM <exe_файл>

*   Linux (all exemplars):

    > killall \. <exe_файл>
*   Linux (alternate)

    > pkill <exe_файл>
*   Linux (for username)

    > killall <exe_файл> -u username

### Сброс авторизации

Для сброса авторизации vk удалите ключ access_token в ini файле
