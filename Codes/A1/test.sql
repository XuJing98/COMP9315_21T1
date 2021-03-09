create table mySets (id integer primary key, iset intSet);
insert into mySets values (1,'{1 ,2,3}');
insert into mySets values (2,'{1,1,1}');
insert into mySets values (3, '{5, 2 , 10}');
insert into mySets values (4, '{}');
select id, iset, (# iset) as card from mySets order by id;
select id, iset, (1?iset) as con from mySets order by id;

