set target=debug

erase /q logs\*.*

start "server" cmd /c "%target%\netbase_app.exe server 220 > logs\_server.log"

for /L %%i in (1,1,10) do (
  start "client %%i" cmd /c "%target%\netbase_app.exe client 180 > logs\_client_%%i.log"
)