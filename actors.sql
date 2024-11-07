CREATE TABLE actors(
idAc varchar(255),
name varchar(255),
surname varchar(255),
age varchar(255)
);
INSERT INTO actors (idAc, name, surname, age)
VALUES ('1', 'Brad', 'Pit', '60');
INSERT INTO actors (idAc, name, surname, age)
VALUES ('2', 'Angelina', 'Jolie', '55');
INSERT INTO actors (idAc, name, surname, age)
VALUES ('3', 'Will', 'Smith', '55');
INSERT INTO actors (idAc, name, surname, age)
VALUES ('4', 'Jada', 'Smith', '50');
INSERT INTO actors (idAc, name, surname, age)
VALUES ('5', 'Leonardo', 'DiCaprio', '50');
INSERT INTO actors (idAc, name, surname, age)
VALUES ('6', 'Danzel', 'Washington', '65');
INSERT INTO actors (idAc, name, surname, age)
VALUES ('7', 'Jennifer', 'Aniston', '50');
INSERT INTO actors (idAc, name, surname, age)
VALUES ('8', 'Johnny', 'Depp', '60');
INSERT INTO actors (idAc, name, surname, age)
VALUES ('10', 'Ryan', 'Reynolds', '50');
CREATE TABLE roles(
idAct varchar(255),
characterName varchar(255)
);
INSERT INTO roles (idAct, characterName)
VALUES ('2', 'Jane_Smith');
INSERT INTO roles (idAct, characterName)
VALUES ('1', 'John_Smith');
INSERT INTO roles (idAct, characterName)
VALUES ('5', 'Jordan_Bellfort');
INSERT INTO roles (idAct, characterName)
VALUES ('3', 'Muhamed_Ali');
INSERT INTO roles (idAct, characterName)
VALUES ('7', 'Rachel_Green');
INSERT INTO roles (idAct, characterName)
VALUES ('1', 'John_Black');
INSERT INTO roles (idAct, characterName)
VALUES ('2', 'Lara_Croft');
INSERT INTO roles (idAct, characterName)
VALUES ('5', 'Jack_Titanic');
INSERT INTO roles (idAct, characterName)
VALUES ('6', 'Robert_Macol_Equalizer');
INSERT INTO roles (idAct, characterName)
VALUES ('8', 'Edward_Scissorhands');
INSERT INTO roles (idAct, characterName)
VALUES ('6', 'Frank_Lucas');
INSERT INTO roles (idAct, characterName)
VALUES ('8', 'Jack_Sparrow');
CREATE TABLE movies(
idMov varchar(255),
name varchar(255)
);
INSERT INTO movies (idMov, name)
VALUES ('10', 'Titanic');
INSERT INTO movies (idMov, name)
VALUES ('2', 'Deadpool');
INSERT INTO movies (idMov, name)
VALUES ('3', 'Deadpool2');
INSERT INTO movies (idMov, name)
VALUES ('6', 'Deadpool3');
CREATE TABLE producer(
idProd varchar(255),
name varchar(255),
surname varchar(255)
);
INSERT INTO producer (idProd, name, surname)
VALUES ('1', 'Quentin', 'Tarantino');
INSERT INTO producer (idProd, name, surname)
VALUES ('2', 'Christopher', 'Nolan');
