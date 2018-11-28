@echo off
START socketServer.exe
START adminServer.exe
msg %username% "Servidor deve ter sido configurado antes de abrir o software cliente!"