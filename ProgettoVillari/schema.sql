drop database progettoVillari;
create database progettoVillari;
use progettoVillari;

create table tbl_group
(	
	id int auto_increment primary key,
	id_parent int,

	foreign key (id_parent)
		references tbl_group(id)
		on update no action
		on delete no action
);

create table tbl_user 
(
	id int auto_increment primary key,
	id_group int not null,

	foreign key (id_group) 
		references tbl_group(id)
		on update no action
		on delete no action
);