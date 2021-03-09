create table mySets (id integer primary key, iset intSet);
insert into mySets values (1,'{1 ,2,3}');
insert into mySets values (2,'{1,1,1}');
insert into mySets values (3, '{5, 2 , 10}');
insert into mySets values (4, '{5, 2 , 0001, 99999999, 666666, 100000000}');
insert into mySets values (6, '{5,2}');
insert into mySets values (5, '{}');
select id, iset, (# iset) as card from mySets order by id;
select a.*, b.* from mySets a, mySets b
where (b.iset @< a.iset) and a.id != b.id;

select a.*, b.* from mySets a, mySets b
where (b.iset >@ a.iset) and a.id != b.id;
delete from mySets where iset = '{5,10,2}';
select * from mySets;
delete from mySets where iset <> '{2,3,1}';
select * from mySets;


