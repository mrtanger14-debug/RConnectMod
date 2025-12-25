# Elden Ring: Nightreign — Seamless Co-op Extension

## Описание:
Расширение предназначено для игры Elden Ring: Nightreign с модом Seamless Co-op. Оно расширяет функциональность Seamless Co-op но не модифицирует его, реализуя подбор случайных игроков для совместной игры.

---

## Возможности:
* **Подключение к обычным игрокам Seamless-Coop**, которые не используют данное расширение (удачное подключение не гарантировано; одна попытка занимает 5–30 секунд; поиск случайный).
* **Подключение к сессиям пользователей расширения** (быстрый поиск, гарантированное подключение при наличии активных пользователей).
* **Создание сессий** для ожидания подключений пользователей расширения.

> [!IMPORTANT]
> **Важная информация:**
> На данный момент поддерживаются: версия игры 1.03.2 и версия Seamless-Coop 1.0.9.

---

## Управление:
Переключение функций осуществляется путем активации жестикуляций.

* **«Указать вперёд»** — поиск среди обычных игроков.
* **«Щелчок пальцами»** — поиск среди пользователей расширения.
* **«Постой»** — отключение поиска.

**Примечание:** Жесты рекомендуется добавить в избранное. Переключение режимов возможно везде, но поиск работает только в хаб-локации.

---

## Использование:

### Поиск сессии:
Находясь в хаб-локации (Круглый стол), примените одну из указанных выше анимаций. Если в конфигурационном файле установлен параметр `console=1`, вы увидите запись о действии в консоли. Просто ждите. Подробности можно отслеживать в окне консоли.

### Создание сессии:
Используя стандартные методы Seamless-Coop. Для этого зайдите в меню Круглого стола, перейдите во вкладку «Настройки подбора игроков», выберите нужное количество пользователей и нажмите «Позвать участников» (выскочит Steam-меню с приглашениями, но приглашать никого не нужно). Это запустит создание лобби. Больше ничего делать не нужно; расширение просто установит в ваше лобби новую метку, по которой и будет происходить поиск. Дальше нужно просто ожидать подключений.

---

## Ограничения:
* Отсутствует графический интерфейс (управление через анимации).
* Поиск обычных игроков может быть долгим и не надежным.
* Для поиска пользователей расширения необходимо наличие созданных ими активных сессий.

---

## Предупреждение:
Исполняемый файл `r_nrsc_launcher.exe` запускает стандартный исполняемый файл Seamless Coop — `nrsc_launcher.exe`, после чего ожидает появления процесса `nightreign.exe`.

Когда процесс `nightreign.exe` обнаружен, выполняется инъекция библиотеки `RConnectMod.dll` в данный процесс. Далее `RConnectMod.dll` осуществляет поиск определённых сигнатур в памяти процесса `nightreign.exe` для получения указателей на необходимые игровые данные.

**Подобное поведение может быть расценено антивирусными программами как вредоносное или потенциально опасное программное обеспечение.**

---

## Инструкция по установке:
1. Скачать архив.
2. Распаковать содержимое в корневую папку игры, рядом с `nightreign.exe`.
3. Запустить исполняемый файл `r_nrsc_launcher.exe`.

## Инструкция по удалению:
удалить файлы относительно корнивой папки:
* `r_nrsc_launcher.exe`
* `SeamlessCoop/RConnectMod.dll`
* `SeamlessCoop/RConnectMod_settings.ini`

---

## Комментарий автора:
На самом деле это та ещё залупа.
Коннект к обычным типам — вообще шляпа.
Симлес кооп не делает никаких пометок: в рейде парни или в хабе, из-за чего приходится коннектиться на авось.
Мало того, если вам повезло найти сессию, где парни собираются в катку, эти критины могут просто испугаться и ливнуть из игры — достаточно бесячее событие.
Таким образом выходит, что такой вид коннекта — шляпа, а коннект к пользователям мода бесполезен по причине отсутствия этих самых пользователей. Чтобы это работало, нужно, чтобы эти пользователи хотя бы были. Мод, кста, ориентирован на пиратов в основном, потому как лицензионщикам такое вообще не нужно.

---

## Компиляция:
Компилировать следует исключительно через Microsoft Build Tools x64 Native.
При использовании других компиляторов инжектор не внедряет DLL в процесс, а сама DLL библиотека крашится при взаимодействии с методами Steam API.
Я слабо знаком с C++ по этому не понимаю причин.
[так же не забудьте скачать SteamworksAPI](https://partner.steamgames.com/downloads/steamworks_sdk_163.zip "прямая ссылка на скачивание SteamworksSDK")

---

# Elden Ring: Nightreign — Seamless Co-op Extension

## Description
This extension is designed for **Elden Ring: Nightreign** using the **Seamless Co-op** mod. It expands the functionality of Seamless Co-op without modifying it, enabling matchmaking with random players for cooperative play.

---

## Features
* **Connect to standard Seamless Co-op players** who do not have this extension installed (connection is not guaranteed; each attempt takes 5–30 seconds; search is randomized).
* **Connect to extension users** (fast search, guaranteed connection if active users are available).
* **Host sessions** to wait for connections from other extension users.

> [!IMPORTANT]
> **Important Information:**
> Currently supported: Game version **1.03.2** and Seamless Co-op version **1.0.9**.

---

## Controls
Functions are toggled by performing specific in-game gestures.

* **"Point Forward"** — Search for standard Seamless Co-op players.
* **"Finger Snap"** — Search for extension users.
* **"Wait"** — Disable search.

**Note:** It is recommended to add these gestures to your favorites. While modes can be toggled anywhere, the search process only functions within the hub location.

---

## Usage

### Finding a Session:
While in the hub location (Roundtable Hold), perform one of the animations mentioned above. If the `console=1` parameter is set in the configuration file, the action will be logged in the console. Simply wait; you can track progress via the console window.

### Hosting a Session:
Use standard Seamless Co-op methods. Go to the Roundtable Hold menu, navigate to the "Matchmaking Settings" tab, select the number of players, and click "Invite Participants" (the Steam invite menu will appear, but you do not need to send any invites). This initializes the lobby. The extension will automatically apply a new tag to your lobby for others to find. Simply wait for players to join.

---

## Limitations
* No Graphical User Interface (controlled via animations).
* Searching for standard players may be slow and unreliable.
* Finding extension users requires active sessions created by other users.

---

## Warning
The executable `r_nrsc_launcher.exe` runs the standard Seamless Co-op launcher (`nrsc_launcher.exe`) and waits for the `nightreign.exe` process to appear.

Once `nightreign.exe` is detected, the `RConnectMod.dll` library is injected into the process. `RConnectMod.dll` then searches for specific signatures in the process memory to obtain pointers to required game data.

**This behavior may be flagged by antivirus software as malicious or potentially unwanted software (PUP).**

---

## Installation:
1. Download the archive.
2. Extract the contents into the game's root folder, next to `nightreign.exe`.
3. Run `r_nrsc_launcher.exe`.

## Uninstallation:
Delete the following files from the root folder:
* `r_nrsc_launcher.exe`
* `SeamlessCoop/RConnectMod.dll`
* `SeamlessCoop/RConnectMod_settings.ini`

## Author comment:
In fact, this is total bullshit.
Connecting to random players is complete trash.
Seamless co-op doesn’t make any markings showing whether players are in a raid or in the hub, so you have to connect blindly.
On top of that, even if you’re lucky enough to find a session where players are getting ready to play, those idiots can just get scared and leave the game — a pretty annoying situation.
As a result, this type of connection is trash, and connecting to mod users is useless due to the lack of those very users. For this to work, those users would at least need to exist. The mod, by the way, is mainly aimed at pirates, since licensed players don’t need this at all.

---

## Compilation:
You should compile exclusively using Microsoft Build Tools x64 Native.
When using other compilers, the injector fails to inject the DLL into the process, and the DLL itself crashes when interacting with Steam API methods.
I am not very familiar with C++, so I don't understand the reasons why.
[also don't forget to download the SteamworksAPI](https://partner.steamgames.com/downloads/steamworks_sdk_163.zip "direct link to download SteamworksSDK")
