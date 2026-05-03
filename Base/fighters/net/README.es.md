# Un ejemplo de programa cliente/servidor

Un ejemplo de un juego multi-jugador usando SDL3_Net.

# Cómo ejecutar estos ejemplos

Suponemos el proyecto se llama TPV2 y está ubicado en `C:\hlocal\TPV2`. Añade un ejemplo al proyecto; ten en cuenta que, dado que todos contienen un método `main`, solo podrás incluir uno a la vez para evitar conflictos de compilación.


- Para ejecutar el servidor, abra una consola y escriba


```
  cd c:
  cd c:\hlocal\TPV2
  set PATH=SDL\bin;%PATH%
  bin\x64\Debug\TPV2.exe server 2000
```

- Para ejecutar el cliente, abra una consola y escriba

```
  cd c:
  cd c:\hlocal\TPV2
  set PATH=SDL\bin;%PATH%
  bin\x64\Debug\TPV2.exe client localhost 2000
```
  
- Para obtener información de uso ejecute:


```
  cd c:
  cd c:\hlocal\TPV2
  set PATH=SDL\bin;%PATH%
  bin\x64\Debug\TPV2.exe
```

También puedes crear archivos llamados `server.BAT` y `client.BAT` con los comandos anteriores; así, solo tendrás que hacer clic en el archivo desde el explorador de archivos para ejecutarlos.


# Cómo abrir una consola

- Pulsa `Windows+R` (o abre el menú `Inicio` y selecciona `Ejecutar`)
- Escribe `cmd` en el campo de texto y haz clic en `OK` (o pulsa `ENTER`)  


Mas información : [https://www.howtogeek.com/235101/10-ways-to-open-the-command-prompt-in-windows-10/](https://www.howtogeek.com/235101/10-ways-to-open-the-command-prompt-in-windows-10/).

