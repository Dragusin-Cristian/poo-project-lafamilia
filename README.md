
##  Hello, 
### we are Dragusin Cristian, Boros Stefan and Diaconu Andrei, and this is our database team project. For this presentation, all the commands are inside the CommandsFiles folder, so we can just copy and paste the commands from the steps below: (Obviously you can have your own files and paths) ($ is the indicator for a command, don't copy it):
---
---

## If we get right into it, 
* ### first we should create a table, just like in a normal database. We can do that by using the command:

### $ projectPOO.exe ./CommandsFiles/CreateTableCommand.txt
*(Open the file and present the arguments passed)*

---

* ### we can see the structure of that table by using the command:

### $ projectPOO.exe ./CommandsFiles/DisplayTableCommand.txt
*(Open the file and present the arguments passed)*

---

* ### then, we can insert some values: 
### $ projectPOO.exe ./CommandsFiles/populateTable.txt
*(Open the file and present the arguments passed)*

---

* ### now, we may want to display ALL the data inside: 
### $ projectPOO.exe ./CommandsFiles/SelectAllFromTableCommand.txt
*(Open the file and present the arguments passed, add a WHERE clause)*

---

* ### now, we may want to display some of the data fields inside: 
### $ projectPOO.exe ./CommandsFiles/SelectFromTableCommand.txt
*(Open the file and present the arguments passed, add a WHERE clause)*

---

* ### now, we may want to drop the table: 
### $ projectPOO.exe ./CommandsFiles/DropTableCommand.txt
*(Open the file and present the arguments passed)*

---

* ### all that can be done much quicker by using all the files in a single command: 
*(You may have to drop the table first, if you created it)*
### $ projectPOO.exe ./CommandsFiles/CreateTableCommand.txt ./CommandsFiles/populateTable.txt ./CommandsFiles/SelectAllFromTableCommand.txt

---
* ### or putting all the commands inside a single file: 
### $ projectPOO.exe ./CommandsFiles/MixedCommands.txt
*(Open the file and present the arguments passed)*

---
---

### *The implementation for updating/deleting tha data and altering the table with update or adding/removing indexes is missing.*

---
### Followed these criteria when build the commands validation: [click here](https://docs.google.com/document/u/0/d/e/2PACX-1vQQbvjkD_XKvMmf9Fhv69GM0iGMbpZUYIJDF4BwZvlO0gh3ykiG84ygpwOQYyRdLvjD3SSFA5lWBhwZ/pub)
