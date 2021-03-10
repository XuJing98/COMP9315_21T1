create table mySets (id integer primary key, iset intSet);
insert into mySets values (1,'{1 ,2,3}');
insert into mySets values (2,'{1,1,1}');
insert into mySets values (3, '{5, 2 , 10}');
insert into mySets values (4, '{5, 2 , 0001, 99999999, 666666, 100000, 11, 222, 19, 222222, 1000000000, 4, 12321312}');
insert into mySets values (6, '{5,2}');
insert into mySets values (5, '{}');
select * from mySets;
insert into mySets values (7, '{1, {2,3}, 4}');



