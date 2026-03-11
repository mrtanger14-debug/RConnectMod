# Elden Ring: Nightreign — Seamless Co-op Extension

---

## Language
- [RU](#описание)
- [EN](#description)

---

## Описание:
Расширение предназначено для игры Elden Ring: Nightreign с модом Seamless Co-op. Оно расширяет функциональность Seamless Co-op но не модифицирует его, реализуя подбор случайных игроков для совместной игры.

---

## Возможности:
* **Подключение к обычным игрокам Seamless-Coop**, которые не используют данное расширение (удачное подключение не гарантировано; одна попытка занимает 5–30 секунд; поиск случайный).
* **Подключение к сессиям пользователей расширения** (быстрый поиск, гарантированное подключение при наличии активных пользователей).
* **Создание сессий** для ожидания подключений пользователей расширения.

> [!IMPORTANT]
> **Важная информация:**
> На данный момент поддерживаются: версия игры **1.03.4** и версия Seamless-Coop **1.1.1**.

---

## Управление:
Переключение функций осуществляется путем активации жестикуляций.

* **«Тёплое приветствие»** — поиск недавно созданных сессий.
* **«Щелчок пальцами»** — поиск среди пользователей расширения.
* **«Постой»** — отключение поиска.

**Эффективность режимов поиска**
* **«Тёплое приветствие»** — [7/10] (Показывает хорошие результаты при большом количестве игроков.)
* **«Щелчок пальцами»** — [8/10] сейчас [0/10] (Гарантированное подключение, но не работает без пользователей этого мода.)

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

## Компиляция:
Компилировать следует исключительно через Microsoft Build Tools x64 Native.
При использовании других компиляторов инжектор не внедряет DLL в процесс, а сама DLL библиотека крашится при взаимодействии с методами Steam API.
Я слабо знаком с C++ по этому не понимаю причин.
[так же не забудьте скачать SteamworksSDK](https://partner.steamgames.com/downloads/steamworks_sdk_163.zip "прямая ссылка на скачивание SteamworksSDK")

---

## Примечание
Касательно пиратской версии игры:
если в папке с игрой находятся файлы OnlineFix.ini или SteamFix.ini, то для входа в лобби используется ID приложения, указанный в строке [Main].fakeAppId.

---

## Description
This extension is designed for **Elden Ring: Nightreign** using the **Seamless Co-op** mod. It expands the functionality of Seamless Co-op without modifying it, enabling matchmaking with random players for cooperative play.

---

## Features
* **Connect to standard Seamless Co-op players** who do not have this extension installed (connection is not guaranteed; each attempt takes 5–30 seconds; search is randomized).
* **Connect to extension users** (fast search, guaranteed connection if active users are available).
* **Host sessions** to wait for connections from other extension users.

> [!IMPORTANT]
> **Important Information:**
> Currently supported: Game version **1.03.4** and Seamless Co-op version **1.1.1**.

---

## Controls
Functions are toggled by performing specific in-game gestures.

* **"Warm Welcome"** — searching for recently created sessions.
* **"Finger Snap"** — Search for extension users.
* **"Wait"** — Disable search.

**Search Modes Efficiency**
* **"Warm Welcome"** — [7/10] (Shows good results with a large number of players.)
* **"Snap of Fingers"** — [8/10] now [0/10] (Guaranteed connection, but does not work without users of this mod.)

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

---

## Compilation:
You should compile exclusively using Microsoft Build Tools x64 Native.
When using other compilers, the injector fails to inject the DLL into the process, and the DLL itself crashes when interacting with Steam API methods.
I am not very familiar with C++, so I don't understand the reasons why.
[also don't forget to download the SteamworksSDK](https://partner.steamgames.com/downloads/steamworks_sdk_163.zip "direct link to download SteamworksSDK")

---

## Note
Regarding the pirated version of the game:
if the game folder contains OnlineFix.ini or SteamFix.ini files, the application ID specified in [Main].fakeAppId is used to enter the lobby.

