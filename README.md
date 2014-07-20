MOG1-GameServer
===============

My first attempt at creating an multiplayer game with a authoritative server. This is the model logic part.

This repo is just a part of a larger system that is currently made up of four parts:

1) GameServer - The game data and logic. Communicates with other programs through a local "Unix" socket.

2) GameManager - A program that can be used to send commands to the data server to add game content, stop the server, admin players, etc.

3) WebServer - Has several roles. It serves the game client, authenticates users, and acts as an interface between clients and the GameServer

4) WebClient - The game client. It is an HTML5/CSS3 interface that is used to provide gameplay inputs to the game, as well as something pretty-ish to look at.
